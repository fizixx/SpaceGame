// Copyright (c) 2015, Tiaan Louw
//
// Permission to use, copy, modify, and/or distribute this software for any
// purpose with or without fee is hereby granted, provided that the above
// copyright notice and this permission notice appear in all copies.
//
// THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
// REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
// INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
// LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
// OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
// PERFORMANCE OF THIS SOFTWARE.

#include "universe/universe_view.h"

#include "universe/link.h"
#include "universe/objects/object.h"
#include "universe/universe.h"

UniverseView::UniverseView(ui::Context* context, Universe* universe)
  : ui::View(context), m_universe(universe) {
  // Set up the mouse position shape.

  m_mousePosShape.setRadius(25.f);
  m_mousePosShape.setOrigin(m_mousePosShape.getGlobalBounds().width / 2.f,
                            m_mousePosShape.getGlobalBounds().height / 2.f);
  m_mousePosShape.setFillColor(sf::Color{0, 0, 255, 255});

  // Set up the hover shape.

  // No fill color.
  m_hoverShape.setFillColor(sf::Color{0, 0, 0, 0});
  m_hoverShape.setOutlineThickness(3.f);
  m_hoverShape.setOutlineColor(sf::Color{0, 255, 0, 255});

  // Set up the selected shape.

  // No fill color.
  m_selectedShape.setFillColor(sf::Color{0, 0, 0, 0});
  m_selectedShape.setOutlineThickness(3.f);
  m_selectedShape.setOutlineColor(sf::Color{0, 255, 255, 255});
}

UniverseView::~UniverseView() {
}

void UniverseView::startPlacingObject(std::unique_ptr<Object> object) {
  m_ghostObject = std::move(object);

  // Create the ghost link between the closest link object and the ghost object.
  Object* closestLinkObject =
      m_universe->getClosestLinkObject(m_ghostObject->getPos());
  m_ghostLink = std::make_unique<Link>(m_universe, closestLinkObject,
                                       m_ghostObject.get());
}

bool UniverseView::onMousePressed(sf::Event& event) {
  ui::View::onMousePressed(event);

  m_camera.onMousePressed(event);
  return true;
}

bool UniverseView::onMouseDragged(sf::Event& event) {
  ui::View::onMouseDragged(event);

  m_camera.onMouseDragged(event);

  // Update the last mouse position in the view and send it to the real
  // onMouseMoved.
  m_viewMousePos = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};
  onMouseMovedInternal(m_camera.mousePosToUniversePos(m_viewMousePos));

  return true;
}

void UniverseView::onMouseMoved(sf::Event& event) {
  ui::View::onMouseMoved(event);

  // Update the last mouse position in the view and send it to the real
  // onMouseMoved.
  m_viewMousePos = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};
  onMouseMovedInternal(m_camera.mousePosToUniversePos(m_viewMousePos));
}

void UniverseView::onMouseReleased(sf::Event& event) {
  ui::View::onMouseReleased(event);

  m_camera.onMouseReleased(event);

  // If we clicked, then set the selected object as the current hover
  // object.
  if (event.mouseButton.button == sf::Mouse::Left && m_hoverObject) {
    m_selectedObject = m_hoverObject;
  }
}

void UniverseView::onMouseWheel(sf::Event& event) {
  ui::View::onMouseWheel(event);

  m_camera.onMouseWheel(event);
}

void UniverseView::tick(float adjustment) {
  m_camera.tick(adjustment);

  // The camera might have updated it's position during the tick, so we check if
  // we have a new mouse position.
  sf::Vector2f mousePos = m_camera.mousePosToUniversePos(m_viewMousePos);
  onMouseMovedInternal(mousePos);

  const float kHoverBorderSize = 2.f;

  // Update the position of the hover shape.
  if (m_hoverObject) {
    sf::FloatRect bounds = m_hoverObject->getBounds();
    bounds.left -= kHoverBorderSize;
    bounds.top -= kHoverBorderSize;
    bounds.width += kHoverBorderSize * 2.f;
    bounds.height += kHoverBorderSize * 2.f;
    m_hoverShape.setPosition(bounds.left, bounds.top);
    m_hoverShape.setSize(sf::Vector2f{bounds.width, bounds.height});
  }

  // Update the position of the selected shape.
  if (m_selectedObject) {
    sf::FloatRect bounds = m_selectedObject->getBounds();
    bounds.left -= kHoverBorderSize;
    bounds.top -= kHoverBorderSize;
    bounds.width += kHoverBorderSize * 2.f;
    bounds.height += kHoverBorderSize * 2.f;
    m_selectedShape.setPosition(bounds.left, bounds.top);
    m_selectedShape.setSize(sf::Vector2f{bounds.width, bounds.height});
  }
}

void UniverseView::layout(const sf::IntRect& rect) {
  View::layout(rect);

  m_camera.layout(rect);
}

void UniverseView::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  // Store the original view state.
  sf::View origView = target.getView();

  // Set the new view to our camera view.
  target.setView(m_camera.getView());

  // Render the links first.
  for (const auto& link : m_universe->m_links) {
    target.draw(*link);
  }

  // Render the ghost link with the rest of the links.
  if (m_ghostLink) {
    target.draw(*m_ghostLink);
  }

  // Render the objects.
  for (const auto& object : m_universe->m_objects) {
    target.draw(*object);
  }

  // Render the ghost object and link over the existing objects.
  if (m_ghostObject) {
    target.draw(*m_ghostObject);
  }

  // Render the camera target.
  target.draw(m_camera);

  // Draw the mouse position.
  target.draw(m_mousePosShape);

  // Reset the target view.
  target.setView(origView);
}

void UniverseView::onMouseMovedInternal(const sf::Vector2f& mousePos) {
  m_universeMousePos = mousePos;
  m_mousePosShape.setPosition(m_universeMousePos);

  // Update the hover object now that we have a new mouse position.
  updateHoverObject();

  // Move the ghost object to the new mouse position.
  if (m_ghostObject) {
    m_ghostObject->moveTo(m_universeMousePos);
  }

  // Find a new link source and update the link.
  if (m_ghostLink) {
    Object* closestObject =
        m_universe->getClosestLinkObject(m_universeMousePos);
    m_ghostLink->setSource(closestObject);
  }
}

void UniverseView::updateHoverObject() {
  // We have to iterate through the list in reverse.
  for (auto it = std::rbegin(m_universe->m_objects),
            eit = std::rend(m_universe->m_objects);
       it != eit; ++it) {
    sf::FloatRect bounds{(*it)->getBounds()};
    if (bounds.contains(m_universeMousePos)) {
      m_hoverObject = *it;
      return;
    }
  }

  m_hoverObject = nullptr;
}

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

UniverseView::UniverseView(el::Context* context, Universe* universe)
  : el::View(context), m_universe(universe), m_hud{this} {
// Set up the mouse position shape.

#if SHOW_UNIVERSE_MOUSE_POS
  m_mousePosShape.setRadius(25.f);
  m_mousePosShape.setOrigin(m_mousePosShape.getGlobalBounds().width / 2.f,
                            m_mousePosShape.getGlobalBounds().height / 2.f);
  m_mousePosShape.setFillColor(sf::Color{0, 0, 255, 255});
#endif  // SHOW_UNIVERSE_MOUSE_POS
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

void UniverseView::stopPlacingObject(bool place) {
  if (place) {
    m_universe->addObject(std::move(m_ghostObject));
    m_universe->addLink(m_ghostLink->getSource(),
                        m_ghostLink->getDestination());
  } else {
    m_ghostObject.reset();
  }
  m_ghostLink.reset();
}

bool UniverseView::onMousePressed(sf::Event& event) {
  el::View::onMousePressed(event);

  m_hudIsHandlingMouseInput = m_hud.onMousePressed(event);

  // If the hud isn't handling mouse presses, then we pass it on to the camera.
  if (!m_hudIsHandlingMouseInput) {
    m_camera.onMousePressed(event);
  }

  return true;
}

bool UniverseView::onMouseDragged(sf::Event& event) {
  el::View::onMouseDragged(event);

  if (!m_hudIsHandlingMouseInput) {
    m_camera.onMouseMoved(event);
  }

  // Update the last mouse position in the view and send it to the real
  // onMouseMoved.
  m_viewMousePos = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};
  onMouseMovedInternal(m_camera.mousePosToUniversePos(m_viewMousePos));

  return true;
}

void UniverseView::onMouseMoved(sf::Event& event) {
  el::View::onMouseMoved(event);

  // Update the last mouse position in the view and send it to the real
  // onMouseMoved.
  m_viewMousePos = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};
  onMouseMovedInternal(m_camera.mousePosToUniversePos(m_viewMousePos));
}

void UniverseView::onMouseReleased(sf::Event& event) {
  el::View::onMouseReleased(event);

  if (m_ghostObject) {
    // Place the object if we press the left button, otherwise cancel the
    // placement.
    stopPlacingObject(event.mouseButton.button == sf::Mouse::Left);

    // Don't pass the event on to the camera.
    return;
  }

  // If we didn't handle the event, then we check if the hud asked to receive
  // the mouse release.  Otherwise we pass the event to the camera.
  if (m_hudIsHandlingMouseInput) {
    m_hud.onMouseReleased(event);
  } else {
    m_camera.onMouseReleased(event);
  }
}

void UniverseView::onMouseWheel(sf::Event& event) {
  el::View::onMouseWheel(event);

  m_camera.onMouseWheel(event);
}

void UniverseView::tick(float adjustment) {
  m_camera.tick(adjustment);
  m_hud.tick(adjustment);

  // The camera might have updated it's position during the tick, so we check if
  // we have a new mouse position.
  sf::Vector2f mousePos = m_camera.mousePosToUniversePos(m_viewMousePos);
  onMouseMovedInternal(mousePos);
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

  // Render all the debugging stuff.

  // Render the camera target.
  target.draw(m_camera);

#if SHOW_UNIVERSE_MOUSE_POS
  // Draw the mouse position.
  target.draw(m_mousePosShape);
#endif

  // Reset the target view.
  target.setView(origView);

  // Render the hud after we reset the view.
  target.draw(m_hud, states);
}

void UniverseView::onMouseMovedInternal(const sf::Vector2f& universeMousePos) {
  // Tell the HUD that we changed the universe mouse position.
  m_hud.updateUniverseMousePos(universeMousePos);

#if SHOW_UNIVERSE_MOUSE_POS
  m_mousePosShape.setPosition(universeMousePos);
#endif

  // Move the ghost object to the new mouse position.
  if (m_ghostObject) {
    m_ghostObject->moveTo(universeMousePos);
  }

  // Find a new link source and update the link.
  if (m_ghostLink) {
    Object* closestObject = m_universe->getClosestLinkObject(universeMousePos);
    m_ghostLink->setSource(closestObject);
  }
}

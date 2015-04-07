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

#include "universe/universe.h"

#include <algorithm>

#include <base/logging.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/link.h"
#include "universe/objects/command_center.h"
#include "universe/objects/power_generator.h"

Universe::Universe(const sf::Vector2f& viewportSize)
  : m_camera(viewportSize) {
  // Set up the background texture and sprite.
  if (!m_backgroundTexture.loadFromFile(
      "C:\\Workspace\\SpaceGame\\res\\images\\star_field_grid.png")) {
    LOG(Error) << "Could not load universe background image.";
  } else {
    m_backgroundTexture.setRepeated(true);
    m_backgroundTexture.setSmooth(true);
  }

  m_backgroundShape.setPrimitiveType(sf::TrianglesFan);
  m_backgroundShape.append(
      sf::Vertex{sf::Vector2f{-5120.f, -5120.f}, sf::Vector2f{0.f, 0.f}});
  m_backgroundShape.append(
      sf::Vertex{sf::Vector2f{5120.f, -5120.f}, sf::Vector2f{10240.f, 0.f}});
  m_backgroundShape.append(
      sf::Vertex{sf::Vector2f{5120.f, 5120.f}, sf::Vector2f{10240.f, 10240.f}});
  m_backgroundShape.append(
      sf::Vertex{sf::Vector2f{-5120.f, 5120.f}, sf::Vector2f{0.f, 10240.f}});

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

  // Create a dummy universe.

  auto commandCenter = std::make_unique<CommandCenter>(this);
  commandCenter->moveTo(sf::Vector2f{0.f, 0.f});
  addObject(commandCenter.release());

  auto powerGenerator1 = std::make_unique<PowerGenerator>(this);
  powerGenerator1->moveTo(sf::Vector2f{500.f, 250.f});
  addObject(powerGenerator1.release());

  auto powerGenerator2 = std::make_unique<PowerGenerator>(this);
  powerGenerator2->moveTo(sf::Vector2f{-450.f, 50.f});
  addObject(powerGenerator2.release());

  addLink(m_objects[0], m_objects[1]);
  addLink(m_objects[0], m_objects[2]);
}

Universe::~Universe() {
}

void Universe::addObject(Object* object) {
  m_objects.push_back(object);
}

void Universe::removeObject(Object* object) {
  auto it = std::find(std::begin(m_objects), std::end(m_objects), object);
  if (it == std::end(m_objects))
    return;

  m_objects.erase(it);
}

void Universe::addLink(Object* source, Object* destination) {
  m_links.emplace_back(new Link{this, source, destination});
}

void Universe::removeLink(Object* any) {
  // Find links the either the source or destination as the specified object.
  auto it =
      std::find_if(std::begin(m_links), std::end(m_links), [&](Link* link) {
        return link->getSource() == any || link->getDestination() == any;
      });

  if (it == std::end(m_links))
    return;

  m_links.erase(it);
}

void Universe::handleInput(sf::Event& event) {
  m_camera.handleInput(event);

  switch (event.type) {
    case sf::Event::MouseMoved:
      m_mousePos = m_camera.mousePosToUniversePos(
          sf::Vector2f{static_cast<float>(event.mouseMove.x),
                       static_cast<float>(event.mouseMove.y)});
      m_mousePosShape.setPosition(m_mousePos);
      updateHoverObject();
      break;

    case sf::Event::MouseButtonPressed:
      // If we clicked, then set the selected object as the current hover
      // object.
      if (event.mouseButton.button == sf::Mouse::Left && m_hoverObject) {
        m_selectedObject = m_hoverObject;
      }
      break;
  }
}

void Universe::tick(float adjustment) {
  m_camera.tick(adjustment);

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

void Universe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Store the original view state.
  sf::View origView = target.getView();

  sf::Vector2f viewportSize{static_cast<float>(target.getSize().x),
                            static_cast<float>(target.getSize().y)};

  // Set the new view to our camera view.
  target.setView(m_camera.getView());

  // Draw the background.
  sf::RenderStates withTexture{states};
  withTexture.texture = &m_backgroundTexture;
  target.draw(m_backgroundShape, withTexture);

  // Render the links first.
  for (const auto& link : m_links) {
    target.draw(*link);
  }

  // Render the objects.
  for (const auto& object : m_objects) {
    target.draw(*object);
  }

  // Render the hover shape.
  if (m_hoverObject) {
    target.draw(m_hoverShape);
  }

  // Render the selected shape.
  if (m_selectedObject) {
    target.draw(m_selectedShape);
  }

  // Render the camera target.
  target.draw(m_camera);

  // Draw the mouse position.
  // target.draw(m_mousePosShape);

  // Reset the target view.
  target.setView(origView);
}

void Universe::updateHoverObject() {
  // We have to iterate through the list in reverse.
  for (auto it = std::rbegin(m_objects), eit = std::rend(m_objects); it != eit;
       ++it) {
    sf::FloatRect bounds{(*it)->getBounds()};
    if (bounds.contains(m_mousePos)) {
      m_hoverObject = *it;
      return;
    }
  }

  m_hoverObject = nullptr;
}

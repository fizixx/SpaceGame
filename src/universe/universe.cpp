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
  : m_camera(viewportSize), m_mousePosShape(25.f) {
  m_mousePosShape.setOrigin(m_mousePosShape.getGlobalBounds().width / 2.f,
                            m_mousePosShape.getGlobalBounds().height / 2.f);
  m_mousePosShape.setFillColor(sf::Color{0, 0, 255, 255});

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
      m_mousePosShape.setPosition(mousePosToUniversePos(
          sf::Vector2f{static_cast<float>(event.mouseMove.x),
                       static_cast<float>(event.mouseMove.y)}));
      break;
  }
}

void Universe::tick(float adjustment) {
  m_camera.tick(adjustment);
}

void Universe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Store the original view state.
  sf::View origView = target.getView();

  sf::Vector2f viewportSize{static_cast<float>(target.getSize().x),
                            static_cast<float>(target.getSize().y)};

  // Set the new view to our camera view.
  target.setView(m_camera.calculateCameraView());

  // Render the links first.
  for (const auto& link : m_links) {
    target.draw(*link);
  }

  // Render the objects.
  for (const auto& object : m_objects) {
    target.draw(*object);
  }

  // Render the camera target.
  target.draw(m_camera);

  target.draw(m_mousePosShape);

  // Reset the target view.
  target.setView(origView);
}

sf::Vector2f Universe::mousePosToUniversePos(
    const sf::Vector2f& mousePos) const {
  sf::View cameraView{m_camera.calculateCameraView()};

  float width = static_cast<float>(cameraView.getSize().x);
  float height = static_cast<float>(cameraView.getSize().y);
  const sf::FloatRect& viewport = cameraView.getViewport();

  sf::IntRect adjViewport{static_cast<int>(0.5f + width * viewport.left),
                          static_cast<int>(0.5f + height * viewport.top),
                          static_cast<int>(0.5f + width * viewport.width),
                          static_cast<int>(0.5f + height * viewport.height)};

  sf::Vector2f normalized;
  normalized.x =
      -1.f + 2.f * (mousePos.x - adjViewport.left) / adjViewport.width;
  normalized.y =
      1.f - 2.f * (mousePos.y - adjViewport.top) / adjViewport.height;

  // Then transform by the inverse of the view matrix
  return cameraView.getInverseTransform().transformPoint(normalized);
}

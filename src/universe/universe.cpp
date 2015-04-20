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
#include <memory>

#include <nucleus/logging.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/link.h"
#include "universe/objects/command_center.h"
#include "universe/objects/power_generator.h"

Universe::Universe(const sf::Vector2f& viewportSize) {
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
  // Delete all the links we own.
  for (auto& link : m_links) {
    delete link;
  }

  // Delete all the objects we own.
  for (auto& object : m_objects) {
    delete object;
  }
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

void Universe::removeLinksConnectedTo(Object* object) {
  // Find links that are either sources or destinations of the specified object.
  auto it = std::find_if(
      std::begin(m_links), std::end(m_links), [object](Link* link) {
        return link->getSource() == object || link->getDestination() == object;
      });

  if (it == std::end(m_links))
    return;

  m_links.erase(it);
}

void Universe::handleInput(sf::Event& event) {
}

void Universe::tick(float adjustment) {
}

void Universe::draw(sf::RenderTarget& target, sf::RenderStates states) const {
}

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

#include "universe/objects/asteroid.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"

DEFINE_OBJECT(Asteroid, "Power Generator");

Asteroid::Asteroid(Universe* universe, const sf::Vector2f& pos,
                   int32_t initialMinerals)
  : Object(universe, ObjectType::Asteroid, pos), m_minerals(initialMinerals) {
  m_shape.setRadius(getRadiusForMinerals(m_minerals));
  m_shape.setFillColor(sf::Color{127, 127, 0, 255});
  sf::FloatRect bounds = m_shape.getLocalBounds();
  m_shape.setOrigin(sf::Vector2f{bounds.width / 2.f, bounds.height / 2.f});
}

Asteroid::~Asteroid() {
}

void Asteroid::setMiniralCount(int32_t mineralCount) {
  m_minerals = mineralCount;
  m_shape.setRadius(getRadiusForMinerals(m_minerals));
}

int32_t Asteroid::mine(int32_t amount) {
  int32_t amountMined = std::min(m_minerals, amount);

  m_minerals -= amount;

  if (m_minerals < 0) {
    // This asteroid is done, pop it.
    m_universe->removeObject(this);
  }

  // Don't do anything to the asteroid here, because it is already deleted.

  return amountMined;
}

sf::FloatRect Asteroid::getBounds() const {
  sf::FloatRect bounds = m_shape.getGlobalBounds();
  bounds.left += m_pos.x;
  bounds.top += m_pos.y;
  return bounds;
}

void Asteroid::tick(float adjustment) {
}

void Asteroid::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
  states.transform.translate(m_pos);
  target.draw(m_shape, states);
}

float Asteroid::getRadiusForMinerals(int32_t minerals) {
  return 25.f + 0.05f * static_cast<float>(minerals);
}

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

Asteroid::Asteroid(Universe* universe, int32_t initialMinerals)
  : Object(universe, ObjectType::Asteroid), m_minerals(initialMinerals),
    m_shape(getRadiusForMinerals(m_minerals)) {
  m_shape.setFillColor(sf::Color{127, 127, 0, 255});
  m_shape.setOrigin(m_shape.getGlobalBounds().width / 2.f,
                    m_shape.getGlobalBounds().height / 2.f);
}

Asteroid::~Asteroid() {
}

void Asteroid::setMiniralCount(int32_t mineralCount) {
  m_minerals = mineralCount;
  m_shape.setRadius(getRadiusForMinerals(m_minerals));
}

void Asteroid::moveTo(const sf::Vector2f& pos) {
  Object::moveTo(pos);

  m_shape.setPosition(pos);
}

sf::FloatRect Asteroid::getBounds() const {
  return m_shape.getGlobalBounds();
}

void Asteroid::tick(float adjustment) {

}

void Asteroid::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
  target.draw(m_shape);
}

float Asteroid::getRadiusForMinerals(int32_t minerals) {
  return 25.f + 0.005f * static_cast<float>(minerals);
}

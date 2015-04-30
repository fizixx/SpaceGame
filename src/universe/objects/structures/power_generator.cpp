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

#include "universe/objects/structures/power_generator.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"

DEFINE_STRUCTURE(PowerGenerator, "Power Generator", 500, 1000);

PowerGenerator::PowerGenerator(Universe* universe)
  : Structure(universe, ObjectType::PowerGenerator, 1), m_shape(50.f) {
  m_shape.setFillColor(sf::Color{255, 255, 0, 255});
  m_shape.setOrigin(m_shape.getGlobalBounds().width / 2.f,
                    m_shape.getGlobalBounds().height / 2.f);
}

PowerGenerator::~PowerGenerator() {
}

void PowerGenerator::shot(Projectile* projectile) {
  Structure::shot(projectile);

  m_shape.setFillColor(
      sf::Color{255, 255, 0, static_cast<sf::Uint8>(255 * m_hitPoints / 500)});
}

void PowerGenerator::moveTo(const sf::Vector2f& pos) {
  Object::moveTo(pos);

  m_shape.setPosition(pos);
}

sf::FloatRect PowerGenerator::getBounds() const {
  return m_shape.getGlobalBounds();
}

void PowerGenerator::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
  target.draw(m_shape);
}

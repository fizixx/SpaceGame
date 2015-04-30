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

#include "universe/objects/projectiles/bullet.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"
#include "utils/math.h"
#include "utils/stream_operators.h"

Bullet::Bullet(Universe* universe, const sf::Vector2f& pos, float direction,
               float speed)
  : Projectile(universe, ObjectType::Bullet, pos, direction, speed),
    m_originalPos(pos) {
  // Set up the circle shape.
  m_shape.setFillColor(sf::Color{255, 0, 0, 255});
  m_shape.setSize(sf::Vector2f{25.f, 5.f});
  m_shape.setOrigin(sf::Vector2f{-15.f, 2.5f});
  m_shape.setRotation(direction);
}

Bullet::~Bullet() {
}

void Bullet::moveTo(const sf::Vector2f& pos) {
  Projectile::moveTo(pos);

  m_shape.setPosition(pos);
}

sf::FloatRect Bullet::getBounds() const {
  return m_shape.getLocalBounds();
}

void Bullet::tick(float adjustment) {
  Projectile::tick(adjustment);

  m_shape.setPosition(m_pos);

  // If the distance from our current position to the original position is more
  // than our range, then just die.
  if (distanceBetween(m_pos, m_originalPos) > 1500.f) {
    m_universe->removeObject(this);
  }
}

void Bullet::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(m_shape, states);
}

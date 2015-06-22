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

#include "universe/universe.h"
#include "utils/math.h"
#include "utils/stream_operators.h"

Bullet::Bullet(Universe* universe, const ca::Vec2& pos, f32 direction,
               f32 speed)
  : Projectile(universe, ObjectType::Bullet, pos), m_direction(direction),
    m_speed(speed), m_originalPos(pos) {
  // Set up the circle shape.
#if 0
  m_shape.setFillColor(sf::Color{255, 0, 0, 255});
  m_shape.setSize(sf::Vector2f{25.f, 5.f});
  m_shape.setOrigin(sf::Vector2f{-15.f, 2.5f});
  m_shape.setRotation(direction);
#endif  // 0
}

Bullet::~Bullet() {
}

ca::Rect<f32> Bullet::getBounds() const {
#if 0
  sf::FloatRect bounds = m_shape.getGlobalBounds();
  bounds.left += m_pos.x;
  bounds.top += m_pos.y;
  return bounds;
#endif  // 0
  return ca::Rect<f32>{};
}

void Bullet::tick(float adjustment) {
  // Advance the bullet by it's speed in the direction it's travelling.
  m_pos = ca::Vec2{m_pos.x + std::cos(degToRad(m_direction)) * m_speed,
                   m_pos.y + std::sin(degToRad(m_direction)) * m_speed};

  // Show the bullet die after this update?
  bool shouldDie = false;

  // Check if we collided with a structure.
  std::vector<Object*> possibles;
  m_universe->findObjectsInRadius(Object::objectTypesForStructures(), m_pos,
                                  10.f, &possibles);
  if (!possibles.empty()) {
    // Just collide with the first one.
    possibles[0]->shot(this);
    shouldDie = true;
  }

  // If the distance from our current position to the original position is more
  // than our range, then just die.
  if (distanceBetween(m_pos, m_originalPos) > 1500.f) {
    shouldDie = true;
  }

  if (shouldDie) {
    m_universe->removeObject(this);
  }
}

void Bullet::render(ca::Canvas* canvas) const {
#if 0
  states.transform.translate(m_pos);
  target.draw(m_shape, states);
#endif  // 0
}

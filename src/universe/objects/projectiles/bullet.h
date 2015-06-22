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

#ifndef UNIVERSE_OBJECTS_PROJECTILES_BULLET_H_
#define UNIVERSE_OBJECTS_PROJECTILES_BULLET_H_

#include "universe/objects/projectiles/projectile.h"

class Bullet : public Projectile {
public:
  Bullet(Universe* universe, const ca::Vec2& pos, f32 direction, f32 speed);
  ~Bullet() override;

  // Override: Projectile
  i32 getDamageAmount() const override { return 50; }
  ca::Rect<f32> getBounds() const override;
  void tick(float adjustment) override;
  void render(ca::Canvas* canvas) const override;

private:
  // The direction we are traveling in.
  float m_direction{0.f};

  // The speed we are travelling.
  float m_speed{0.f};

  // The original position we started to travel from.
  ca::Vec2 m_originalPos;

  // The shape we use to represent the bullet.
#if 0
  sf::RectangleShape m_shape;
#endif  // 0

  DISALLOW_COPY_AND_ASSIGN(Bullet);
};

#endif  // UNIVERSE_OBJECTS_PROJECTILES_BULLET_H_

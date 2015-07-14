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

#ifndef UNIVERSE_OBJECTS_PROJECTILES_PROJECTILE_H_
#define UNIVERSE_OBJECTS_PROJECTILES_PROJECTILE_H_

#include "universe/objects/object.h"

class Projectile : public Object {
public:
  Projectile(Universe* universe, ObjectType objectType,
             const ca::Vec2& launchPos, f32 direction, f32 speed);
  ~Projectile() override;

  virtual i32 getDamageAmount() const = 0;

protected:
  // The direction the projectile travelling.
  f32 m_direction{0.f};

  // The speed the projectile is traveling at.
  f32 m_speed{0.f};

  // The position the projectile was launched from.
  ca::Vec2 m_launchPos;

private:
  DISALLOW_COPY_AND_ASSIGN(Projectile);
};

#endif  // UNIVERSE_OBJECTS_PROJECTILES_PROJECTILE_H_

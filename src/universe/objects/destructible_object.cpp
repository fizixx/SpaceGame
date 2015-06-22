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

#include "universe/objects/destructible_object.h"

#include "universe/objects/projectiles/projectile.h"
#include "universe/universe.h"

DestructibleObject::DestructibleObject(Universe* universe, ObjectType type,
                                       const ca::Vec2& pos, i32 hitPoints)
  : Object(universe, type, pos), m_hitPoints(hitPoints) {
}

DestructibleObject::~DestructibleObject() {
}

void DestructibleObject::shot(Projectile* projectile) {
  // We have been shot so remove some hitpoints.
  m_hitPoints -= projectile->getDamageAmount();

#if 0
  // For now we don't destroy the projectile here, because each projectile is
  // destroying itself on collision.

  // Destroy the projectile.
  m_universe->removeObject(projectile);
#endif  // 0

  // Check if we died.
  if (m_hitPoints < 0.f) {
    m_universe->removeObject(this);
  }

  // WARNING: Don't do anything after this point, because we're deleting this.
}

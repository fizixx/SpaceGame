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

#include "universe/objects/structures/structure.h"

#include "universe/universe.h"
#include "universe/objects/projectiles/projectile.h"

DEFINE_STRUCTURE(Structure, "Structure", 0, 0);

Structure::Structure(Universe* universe, ObjectType objectType, int32_t hitPoints)
  : Object(universe, objectType), m_hitPoints(hitPoints) {
}

Structure::~Structure() {
}

void Structure::shot(Projectile* projectile) {
  // We have been shot so remove some hitpoints.
  m_hitPoints -= projectile->getDamageAmount();

  m_universe->removeObject(projectile);

  // WARNING: Don't do anything after this point, because we're deleting this.

  // Check if we died.
  if (m_hitPoints < 0.f) {
    m_universe->removeObject(this);
  }
}

void Structure::tick(float adjustment) {
  m_universe->adjustPower(getPowerCost());
}

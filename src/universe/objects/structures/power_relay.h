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

#ifndef UNIVERSE_OBJECTS_STRUCTURES_POWER_RELAY_H_
#define UNIVERSE_OBJECTS_STRUCTURES_POWER_RELAY_H_

#include "universe/objects/structures/structure.h"

class PowerRelay : public Structure {
  DECLARE_STRUCTURE(PowerRelay);

public:
  PowerRelay(Universe* universe, const ca::Vec2& pos);
  ~PowerRelay() override;

  // Override: Object
  void shot(Projectile* projectile) override;
  ca::Rect<f32> getBounds() const override;
  void render(ca::Canvas* canvas) const override;

private:
  // The shape we use to render the power generator.
#if 0
  sf::CircleShape m_shape;
#endif  // 0

  DISALLOW_IMPLICIT_CONSTRUCTORS(PowerRelay);
};

#endif  // UNIVERSE_OBJECTS_STRUCTURES_POWER_RELAY_H_

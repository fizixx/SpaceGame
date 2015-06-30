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

#ifndef UNIVERSE_OBJECTS_ASTEROID_H_
#define UNIVERSE_OBJECTS_ASTEROID_H_

#include "canvas/rendering/sprite.h"

#include "universe/objects/object.h"

class Asteroid : public Object {
  DECLARE_OBJECT(Asteroid);

public:
  Asteroid(Universe* universe, const ca::Vec2& pos, i32 startingMinerals);
  ~Asteroid() override;

  i32 getMineralCount() const { return m_minerals; }
  void setMiniralCount(i32 mineralCount);

  // Mine the asteroid.
  i32 mine(i32 amount);

  // Override: Object
  void tick(float adjustment) override;

private:
  // The amount of minerals we have.
  i32 m_minerals;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Asteroid);
};

#endif  // UNIVERSE_OBJECTS_ASTEROID_H_

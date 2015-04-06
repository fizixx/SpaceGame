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

#ifndef UNIVERSE_OBJECTS_POWER_GENERATOR_H_
#define UNIVERSE_OBJECTS_POWER_GENERATOR_H_

#include <SFML/Graphics/CircleShape.hpp>

#include "universe/objects/object.h"

class PowerGenerator : public Object {
public:
  explicit PowerGenerator(Universe* universe);
  virtual ~PowerGenerator() override;

  // Override: Object
  virtual void moveTo(const sf::Vector2f& pos) override;
  virtual bool inBounds(const sf::Vector2f& pos) const override;
  virtual void tick(float adjustment) override;
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

private:
  sf::CircleShape m_shape;

  DISALLOW_IMPLICIT_CONSTRUCTORS(PowerGenerator);
};

#endif  // UNIVERSE_OBJECTS_POWER_GENERATOR_H_

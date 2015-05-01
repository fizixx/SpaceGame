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

#include <vector>

#include <SFML/Graphics/CircleShape.hpp>

#include "universe/objects/object.h"

class Asteroid : public Object {
  DECLARE_OBJECT(Asteroid);

public:
  Asteroid(Universe* universe, const sf::Vector2f& pos,
           int32_t startingMinerals);
  ~Asteroid() override;

  int32_t getMineralCount() const { return m_minerals; }
  void setMiniralCount(int32_t mineralCount);

  // Mine the asteroid.
  int32_t mine(int32_t amount);

  // Override: Object
  sf::FloatRect getBounds() const override;
  void tick(float adjustment) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  // Get the radius of the shape we are rendering based on the mineral amount.
  float getRadiusForMinerals(int32_t minerals);

  // The amount of minerals we have.
  int32_t m_minerals;

  // The shape we use to render the power generator.
  sf::CircleShape m_shape;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Asteroid);
};

#endif  // UNIVERSE_OBJECTS_ASTEROID_H_

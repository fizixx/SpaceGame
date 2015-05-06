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

#ifndef UNIVERSE_OBJECTS_STRUCTURES_MINER_H_
#define UNIVERSE_OBJECTS_STRUCTURES_MINER_H_

#include <memory>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "universe/objects/structures/structure.h"
#include "universe/observers.h"

class Asteroid;

class Miner : public Structure {
  DECLARE_STRUCTURE(Miner);

public:
  Miner(Universe* universe, const sf::Vector2f& pos);
  virtual ~Miner() override;

  // Override: Object
  void moveTo(const sf::Vector2f& pos) override;
  sf::FloatRect getBounds() const override;
  void tick(float adjustment) override;
  void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

private:
  // A class representing a laser to an asteroid.
  class Laser : public sf::Drawable {
  public:
    Laser(Universe* universe, Miner* miner, Asteroid* asteroid);
    ~Laser();

    // Get the asteroid we are mining.
    Asteroid* getAsteroid() const { return m_asteroid; }

    // Override: sf::Drawable
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

  private:
    // The universe we belong in.
    Universe* m_universe;

    // The miner we are lasing from.
    Miner* m_miner;

    // The asteroid we are mining.
    Asteroid* m_asteroid;

    // The shape we use to render the laser.
    sf::RectangleShape m_shape;

    DISALLOW_IMPLICIT_CONSTRUCTORS(Laser);
  };

  // Called when the universe removed an object.
  void onObjectRemoved(Object* object);

  // Recreate all the lasers pointing to valid minable asteroids.
  void recreateLasers();

  // Mine all the asteroids we have lasers on.
  void mineAsteroids();

  // The time passed since the last time we mined all the asteroids.
  float m_lastMinedAsteroid{0.f};

  // Lasers to asteroids.
  std::vector<std::unique_ptr<Laser>> m_lasers;

  // Id for the removed object slot.
  size_t m_removedObjectId;

  // The shape we use to render the power generator.
  sf::CircleShape m_shape;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Miner);
};

#endif  // UNIVERSE_OBJECTS_STRUCTURES_H_

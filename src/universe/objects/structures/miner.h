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
#include <vector>

#include "universe/objects/structures/structure.h"
#include "universe/observers.h"

class Asteroid;

class Miner : public Structure {
  DECLARE_STRUCTURE(Miner);

public:
  Miner(Universe* universe, const ca::Vec2& pos);
  virtual ~Miner() override;

  // Override: Object
  void moveTo(const ca::Vec2& pos) override;
  void tick(f32 adjustment) override;
  void render(ca::Canvas* canvas, const ca::Mat4& transform) const override;

private:
  // A class representing a laser to an asteroid.
  class Laser {
  public:
    Laser(Universe* universe, Miner* miner, Asteroid* asteroid);
    ~Laser();

    // Get the asteroid we are mining.
    Asteroid* getAsteroid() const { return m_asteroid; }

    // Override: sf::Drawable
    void render(ca::Canvas* canvas) const;

  private:
    // The universe we belong in.
    Universe* m_universe;

    // The miner we are lasing from.
    Miner* m_miner;

    // The asteroid we are mining.
    Asteroid* m_asteroid;

    // The shape we use to render the laser.
#if 0
    sf::RectangleShape m_shape;
#endif  // 0

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
  usize m_removedObjectId;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Miner);
};

#endif  // UNIVERSE_OBJECTS_STRUCTURES_H_

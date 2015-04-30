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

#ifndef UNIVERSE_OBJECTS_UNITS_ENEMY_SHIP_H_
#define UNIVERSE_OBJECTS_UNITS_ENEMY_SHIP_H_

#include "universe/objects/units/unit.h"

#include <SFML/Graphics/VertexArray.hpp>

class EnemyShip : public Unit {
public:
  explicit EnemyShip(Universe* universe);
  ~EnemyShip() override;

  // Override: Unit
  sf::FloatRect getBounds() const override;
  void tick(float adjustment) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  enum class Task {
    Nothing,
    Travel,
    Attacking,
  };

  // Create the shape that we use to show the engagement envelope of the ship.
  void createEngagementRangeShape();

  // The shape we use to render the ship.
  sf::VertexArray m_shape;

  // A shape used to show the engagement envelope of the ship.
  sf::VertexArray m_engagementRangeShape;

  // This ship's current task.
  Task m_task{Task::Nothing};

  // The direction the ship is traveling in.
  float m_direction{0.f};

  // The current speed that we are traveling at.
  float m_speed{0.f};

  // The current target that we are travelling towards.
  sf::Vector2f m_travelTargetPos;

  DISALLOW_IMPLICIT_CONSTRUCTORS(EnemyShip);
};

#endif  // UNIVERSE_OBJECTS_UNITS_ENEMY_SHIP_H_

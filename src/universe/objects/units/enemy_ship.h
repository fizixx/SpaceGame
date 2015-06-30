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

#include "canvas/rendering/text.h"
#include "nucleus/config.h"

#include "particles/particle_emitter.h"

class EnemyShip : public Unit {
public:
  EnemyShip(Universe* universe, const ca::Vec2& pos);
  ~EnemyShip() override;

  // Set the current target for the ship.
  void setTarget(Object* target);

  // Override: Unit
  void shot(Projectile* projectile) override;
  void tick(f32 adjustment) override;

private:
  enum class Task {
    Nothing,
    Travel,
    Attacking,
    Egress,
  };

  // From our current position, select the best target to attack.
  Object* selectBestTarget();

  // Create the shape that we use to show the engagement envelope of the ship.
  void createEngagementRangeShape();

  // Shoot a projectile at the target.
  void shoot();

  // Called when the universe removed an object.
  void onObjectRemoved(Object* object);

  // Factory function to create a smoke particle.
  Particle* createSmokeParticle(ParticleEmitter* emitter, const ca::Vec2& pos);

// A shape used to show the engagement envelope of the ship.
#if 0
  sf::VertexArray m_engagementRangeShape;
#endif  // 0

  // Particle emitter to render the smoke trail.
  ParticleEmitter m_smokeEmitter;

  // This ship's current task.
  Task m_task{Task::Nothing};

  // The direction the ship is traveling in.
  float m_direction{0.f};

  // The current speed that we are traveling at.
  float m_speed{0.f};

  // The object we are attacking.
  Object* m_target;

  // The current target that we are travelling towards.
  ca::Vec2 m_travelTargetPos;

  // Ticks since the last time we fired a shot.
  float m_timeSinceLastShot{0.f};

#if BUILD(DEBUG)
  // Info text that we print out with the ship.
  ca::Text m_infoText;
#endif  // BUILD(DEBUG)

  int stepper{0};

  size_t objectRemovedId;

  DISALLOW_IMPLICIT_CONSTRUCTORS(EnemyShip);
};

#endif  // UNIVERSE_OBJECTS_UNITS_ENEMY_SHIP_H_

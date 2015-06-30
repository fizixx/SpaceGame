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

#ifndef UNIVERSE_OBJECTS_STRUCTURES_TURRET_H_
#define UNIVERSE_OBJECTS_STRUCTURES_TURRET_H_

#include <array>

#include "universe/objects/structures/structure.h"
#include "universe/observers.h"

class Missile;

class Turret : public Structure {
  DECLARE_STRUCTURE(Turret);

public:
  Turret(Universe* universe, const ca::Vec2& pos);
  ~Turret() override;

  // Override: Object
  void shot(Projectile* projectile) override;
  void moveTo(const ca::Vec2& pos) override;
  void tick(float adjustment) override;

private:
  enum class Task {
    Idle,
    Attacking,
  };

  // Find the best target to shoot at.
  Object* findBestTarget();

  // Shoot the gun.
  void shoot();

  // Called when the universe removed an object.
  void onObjectRemoved(Object* object);

  // The direction the turret is facing.
  float m_turretDirection{0.f};

  // The current target we are shooting at.
  Object* m_target{nullptr};

  // The current task we are performing.
  Task m_task{Task::Idle};

  // The time that has passed since the last shot was fired.
  float m_timeSinceLastShot{0.f};

  // We have 3 missiles.
  std::array<Missile*, 3> m_missiles;

  // The RemovedObject slot id.
  size_t m_removedObjectSlotId;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Turret);
};

#endif  // UNIVERSE_OBJECTS_STRUCTURES_TURRET_H_

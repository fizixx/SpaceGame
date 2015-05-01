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

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include "universe/objects/structures/structure.h"
#include "universe/observers.h"

class Missile;

class Turret : public Structure, public RemoveObjectObserver {
  DECLARE_STRUCTURE(Turret);

public:
  Turret(Universe* universe, const sf::Vector2f& pos);
  ~Turret() override;

  // Override: Object
  void shot(Projectile* projectile) override;
  void moveTo(const sf::Vector2f& pos) override;
  sf::FloatRect getBounds() const override;
  void tick(float adjustment) override;
  void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

  // Override: RemoveObjectObserver
  void onRemovingObject(Object* object) override;
  void onObjectRemoved(Object* object) override;

private:
  enum class Task {
    Idle,
    Attacking,
  };

  // Find the best target to shoot at.
  Object* findBestTarget();

  // Move the missiles into their positions on the rail.
  void turnRail(float degrees);

  // Shoot the gun.
  void shoot();

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

  // The shape we use to render the base of the turret.
  sf::CircleShape m_baseShape;

  // The shape we use to render the turret.
  sf::RectangleShape m_launcherRailShape;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Turret);
};

#endif  // UNIVERSE_OBJECTS_STRUCTURES_TURRET_H_

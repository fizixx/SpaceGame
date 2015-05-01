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

#include "universe/objects/structures/turret.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/objects/projectiles/missile.h"
#include "universe/universe.h"
#include "utils/math.h"

DEFINE_STRUCTURE(Turret, "Turret", 750, 1000);

namespace {

const float kMaxAttachRange = 2500.f;

}  // namespace

Turret::Turret(Universe* universe, const sf::Vector2f& pos)
  : Structure(universe, ObjectType::Turret, pos, 500) {
  // Set up the base shape.
  {
    m_baseShape.setRadius(50.f);
    m_baseShape.setFillColor(sf::Color{0, 255, 0, 255});
    sf::FloatRect bounds = m_baseShape.getLocalBounds();
    m_baseShape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  }

  // Set up the turret shape.
  {
    m_launcherRailShape.setSize(sf::Vector2f{20.f, 40.f});
    m_launcherRailShape.setFillColor(sf::Color{0, 127, 0, 255});
    sf::FloatRect bounds = m_launcherRailShape.getLocalBounds();
    m_launcherRailShape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  }

  // Create our 3 missiles.
  for (auto& missile : m_missiles) {
    missile = static_cast<Missile*>(m_universe->addObject(
        std::make_unique<Missile>(m_universe, m_pos, m_turretDirection)));
  }

  m_universe->addRemoveObjectObserver(this);
}

Turret::~Turret() {
  m_universe->removeRemoveObjectObserver(this);

  // If we die and we have missiles on the rail, then our missiles die too.
  for (auto& missile : m_missiles) {
    if (missile && !missile->isLaunched()) {
      m_universe->removeObject(missile);
    }
  }
}

void Turret::shot(Projectile* projectile) {
  Structure::shot(projectile);

  m_baseShape.setFillColor(
      sf::Color{0, 255, 0, static_cast<sf::Uint8>(255 * m_hitPoints / 500)});
}

void Turret::moveTo(const sf::Vector2f& pos) {
  Structure::moveTo(pos);
  for (auto& missile : m_missiles) {
    missile->moveTo(pos);
  }
}

sf::FloatRect Turret::getBounds() const {
  sf::FloatRect bounds = m_baseShape.getGlobalBounds();
  bounds.left += m_pos.x;
  bounds.top += m_pos.y;
  return bounds;
}

void Turret::tick(float adjustment) {
  Structure::tick(adjustment);

  if (m_task == Task::Idle) {
    // Turn the rails as if they are searching for a target.
    turnRail(m_turretDirection + 1.f * adjustment);

    m_target = findBestTarget();
    if (m_target) {
      m_task = Task::Attacking;
    }
  }

  if (m_task == Task::Attacking) {
    float directionToTarget = directionBetween(m_pos, m_target->getPos());

    // Snap the turret to the target for now.  In future we should have a max
    // turn radius.
    turnRail(directionToTarget);

    if (m_timeSinceLastShot > 100.f) {
      shoot();
      m_timeSinceLastShot = 0.f;
    } else {
      m_timeSinceLastShot += adjustment;
    }
  }
}

void Turret::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform.translate(m_pos);
  target.draw(m_baseShape, states);
  target.draw(m_launcherRailShape, states);
}

void Turret::onRemovingObject(Object* object) {
}

void Turret::onObjectRemoved(Object* object) {
  // If the object is one of our missiles, then we should create a missile in
  // it's place.
  for (size_t i = 0; i < m_missiles.size(); ++i) {
    if (m_missiles[i] == object) {
      m_missiles[i] = static_cast<Missile*>(m_universe->addObject(
          std::make_unique<Missile>(m_universe, m_pos, m_turretDirection)));
    }
  }

  // If the object that is about to removed is our target, then we need a new
  // target.
  if (object == m_target) {
    m_target = nullptr;

    // Go back to idle so that we can select a new target.
    m_task = Task::Idle;
  }
}

Object* Turret::findBestTarget() {
  // Just find the closest enemy ship for now.
  return m_universe->findClosestObjectOfType(m_pos, ObjectType::EnemyShip,
                                             kMaxAttachRange);
}

void Turret::turnRail(float direction) {
  m_turretDirection = direction;
  m_launcherRailShape.setRotation(m_turretDirection);

  for (size_t i = 0; i < m_missiles.size(); ++i) {
    // If the missile has been launched, we don't have control over it any more.
    if (m_missiles[i]->isLaunched()) {
      continue;
    }

    sf::Transform transform;
    transform.rotate(direction);
    m_missiles[i]->moveTo(
        m_pos +
        transform.transformPoint(
            0.f, static_cast<float>(static_cast<int32_t>(i) - 1) * 15.f));
    m_missiles[i]->setDirection(direction);
  }
}

void Turret::shoot() {
  // DCHECK(m_target) << "To launch a missile we need a target.";

  // Find a missile that we can launch.
  Missile* ready = nullptr;
  for (size_t i = 0; i < m_missiles.size(); ++i) {
    if (m_missiles[i] && !m_missiles[i]->isLaunched()) {
      ready = m_missiles[i];
      break;
    }
  }

  if (ready) {
    ready->launchAt(m_target);
  }
}

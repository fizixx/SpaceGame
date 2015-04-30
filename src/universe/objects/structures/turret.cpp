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

#include "universe/objects/projectiles/bullet.h"
#include "universe/universe.h"
#include "utils/math.h"

DEFINE_STRUCTURE(Turret, "Turret", 750, 1000);

namespace {

const float kMaxAttachRange = 2500.f;

}  // namespace

Turret::Turret(Universe* universe)
  : Structure(universe, ObjectType::Turret, 500) {
  // Set up the base shape.
  m_baseShape.setRadius(50.f);
  m_baseShape.setFillColor(sf::Color{0, 255, 0, 255});
  m_baseShape.setOrigin(m_baseShape.getGlobalBounds().width / 2.f,
                        m_baseShape.getGlobalBounds().height / 2.f);

  // Set up the turret shape.
  m_turretShape.setSize(sf::Vector2f{70.f, 5.f});
  m_turretShape.setFillColor(sf::Color{0, 127, 0, 255});
  m_turretShape.setOrigin(sf::Vector2f{-35.0f, 2.5f});

  m_universe->addRemoveObjectObserver(this);
}

Turret::~Turret() {
  m_universe->removeRemoveObjectObserver(this);
}

void Turret::shot(Projectile* projectile) {
  Structure::shot(projectile);

  m_baseShape.setFillColor(
      sf::Color{0, 255, 0, static_cast<sf::Uint8>(255 * m_hitPoints / 500)});
}

void Turret::moveTo(const sf::Vector2f& pos) {
  Object::moveTo(pos);

  m_baseShape.setPosition(pos);
  m_turretShape.setPosition(pos);
}

sf::FloatRect Turret::getBounds() const {
  return m_baseShape.getGlobalBounds();
}

void Turret::tick(float adjustment) {
  Structure::tick(adjustment);

  if (m_task == Task::Idle) {
    // If we're idle, we act like a search radar until we find a target.
    m_turretDirection += 1.f * adjustment;
    m_turretShape.setRotation(m_turretDirection);

    m_target = findBestTarget();
    if (m_target) {
      m_task = Task::Attacking;
    }
  }

  if (m_task == Task::Attacking) {
    float directionToTarget = directionBetween(m_pos, m_target->getPos());

    // Snap the turret to the target for now.  In future we should have a max
    // turn radius.
    m_turretDirection = directionToTarget;
    m_turretShape.setRotation(m_turretDirection);

    if (m_timeSinceLastShot > 100.f) {
      shoot();
      m_timeSinceLastShot = 0.f;
    } else {
      m_timeSinceLastShot += adjustment;
    }
  }
}

void Turret::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(m_baseShape);
  target.draw(m_turretShape);
}

void Turret::onObjectRemoved(Object* object) {
  // If the object that is about to removed is our target, then we need a new
  // target.
  if (object == m_target) {
    m_target = nullptr;

    // Go back to idle so that we can select a new target.
    m_task == Task::Idle;
  }
}

Object* Turret::findBestTarget() {
  // Just find the closest enemy ship for now.
  return m_universe->findClosestObjectOfType(m_pos, ObjectType::EnemyShip,
                                             kMaxAttachRange);
}

void Turret::shoot() {
  auto bullet =
      std::make_unique<Bullet>(m_universe, m_pos, m_turretDirection, 10.f);
  m_universe->addObject(std::move(bullet));
}

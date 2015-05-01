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

#include "universe/objects/projectiles/missile.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"
#include "utils/math.h"

namespace {

static const float kMaxTurnRadius = 5.f;
static const float kMaxSpeed = 10.f;

}  // namespace

Missile::Missile(Universe* universe, sf::Vector2f& pos, float direction)
  : Projectile(universe, ObjectType::Missile, pos), m_direction(direction) {
  // Set up the shape.
  m_shape.setPrimitiveType(sf::Triangles);
  m_shape.append(sf::Vertex{sf::Vector2f{0.f, 0.f}, sf::Color{255, 0, 0, 255}});
  m_shape.append(
      sf::Vertex{sf::Vector2f{-10.f, 5.f}, sf::Color{255, 0, 0, 255}});
  m_shape.append(
      sf::Vertex{sf::Vector2f{-10.f, -5.f}, sf::Color{255, 0, 0, 255}});

  m_universe->addRemoveObjectObserver(this);
}

Missile::~Missile() {
  m_universe->removeRemoveObjectObserver(this);
}

void Missile::launchAt(Object* target) {
  // Target acquired...
  m_target = target;

  // ...LAUNCH!
  m_task = Task::Launching;
}

void Missile::setDirection(float direction) {
  m_direction = direction;
}

int32_t Missile::getDamageAmount() const {
  return 50;
}

sf::FloatRect Missile::getBounds() const {
  return m_shape.getBounds();
}

void Missile::tick(float adjustment) {
  // For now if we're launching, then just go to tracking immediately.
  if (m_task == Task::Launching) {
    m_task = Task::Tracking;
  }

  // If we have a target, get to it.
  if (m_task == Task::Tracking) {
    // Calculate the direction to the target.
    const float directionToTarget = directionBetween(m_pos, m_target->getPos());

    // If we are not pointing directly towards the target, then we must turn.
    if (m_direction != directionToTarget) {
      const float leftDiff =
          wrap(360.f - directionToTarget + m_direction, 0.f, 360.f);
      const float rightDiff = wrap(directionToTarget - m_direction, 0.f, 360.f);

      // Figure out which way to turn.
      const float turnSide =
          (leftDiff < rightDiff) ? -kMaxTurnRadius : kMaxTurnRadius;

      // Adjust the direction towards the target.
      m_direction = wrap(m_direction + turnSide, 0.f, 360.f);

      // If the direction is within kMaxTurnRadius of the target direction, then
      // snap it directly to the target, to avoid oscillation.
      if (std::abs(wrap(m_direction - directionToTarget, 0.f, 360.f)) <
          kMaxTurnRadius) {
        m_direction = directionToTarget;
      }
    }

    m_speed = kMaxSpeed;

    // Adjust the position of the missile accordingly.
    sf::Vector2f delta{std::cos(degToRad(m_direction)) * m_speed,
                       std::sin(degToRad(m_direction)) * m_speed};
    m_pos += delta;

    bool shouldRemove = false;

    // See if we collided with any enemy ships.
    std::vector<Object*> enemyShips;
    m_universe->findObjectsInRadius(ObjectType::EnemyShip, m_pos, 10.f,
                                    &enemyShips);
    if (!enemyShips.empty()) {
      m_target->shot(this);
      shouldRemove = true;
    }

    // Increase the time passed since we were launched.
    if (m_timeSinceLaunch > 250.f) {
      shouldRemove = true;
      m_timeSinceLaunch = 0.f;
    } else {
      m_timeSinceLaunch += adjustment;
    }

    if (shouldRemove) {
      m_universe->removeObject(this);
    }
  }
}

void Missile::onRemovingObject(Object* object) {
}

void Missile::onObjectRemoved(Object* object) {
  // If the object is our target, then we self-destruct.
  if (object == m_target) {
    m_universe->removeObject(this);
  }
}

void Missile::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform.translate(m_pos);
  states.transform.rotate(m_direction);
  target.draw(m_shape, states);
}

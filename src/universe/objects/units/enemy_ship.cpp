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

#include "universe/objects/units/enemy_ship.h"

#include <functional>
#include <sstream>

#include "nucleus/logging.h"

#include "universe/objects/projectiles/bullet.h"
#include "universe/universe.h"
#include "utils/math.h"
#include "utils/stream_operators.h"
#include "particles/particle.h"

namespace {

const f32 kMaxTurnRadius = 1.5f;
const f32 kMaxTravelSpeed = 5.f;
const f32 kMaxAttackSpeed = 3.f;
const f32 kMaxEngagementRange = 750.f;

}  // namespace

EnemyShip::EnemyShip(Universe* universe, const ca::Vec2& pos)
  : Unit(universe, ObjectType::EnemyShip, pos, 250),
    m_smokeEmitter(std::bind(&EnemyShip::createSmokeParticle, this,
                             std::placeholders::_1, std::placeholders::_2)) {
  m_renderComponent = std::make_unique<SpriteRenderComponent>(
      universe->getResourceManager(), ResourceManager::Texture::Unknown);

#if BUILD(DEBUG) && 0
  // Set up the info text.
  sf::Font* font =
      universe->getResourceManager()->getFont(ResourceManager::Font::Default);
  if (font) {
    m_infoText.setFont(*font);
    m_infoText.setColor(sf::Color{255, 255, 255, 255});
    m_infoText.setCharacterSize(20);
    m_infoText.setString("Nothing to say");
  }
#endif  // BUILD(DEBUG)

  createEngagementRangeShape();

  objectRemovedId = m_universe->getObjectRemovedSignal().connect(
      nu::slot(&EnemyShip::onObjectRemoved, this));
}

EnemyShip::~EnemyShip() {
  m_universe->getObjectRemovedSignal().disconnect(objectRemovedId);
}

void EnemyShip::setTarget(Object* target) {
  m_target = target;
  m_task = Task::Nothing;
}

void EnemyShip::shot(Projectile* projectile) {
  Unit::shot(projectile);
}

void EnemyShip::tick(f32 adjustment) {
  // Move the emitter into place and tick it.
  m_smokeEmitter.setPos(m_pos);
  m_smokeEmitter.tick(adjustment);

  // If we're doing nothing, then decide where we want to go and then go there.
  if (m_task == Task::Nothing) {
    // We have nothing to do, so select the best target to attack and travel
    // there.
    m_task = Task::Travel;
    m_target = selectBestTarget();
    if (m_target) {
      m_travelTargetPos = m_target->getPos();
    }
  }

  // If we don't have a target, we don't know what to do.
  if (!m_target) {
    return;
  }

  // If we are traveling, update the direction and speed we are traveling in.
  if (m_task == Task::Travel) {
    // Calculate the direction to the target.
    const f32 directionToTarget = directionBetween(m_pos, m_travelTargetPos);

    // If we are not pointing directly towards the target, then we must turn.
    if (m_direction != directionToTarget) {
      const f32 leftDiff =
          wrap(360.f - directionToTarget + m_direction, 0.f, 360.f);
      const f32 rightDiff = wrap(directionToTarget - m_direction, 0.f, 360.f);

      // Figure out which way to turn.
      const f32 turnSide =
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

    m_speed = kMaxTravelSpeed;

    // If we have speed, update our position.
    m_pos = ca::Vec2{m_pos.x + std::cos(degToRad(m_direction)) * m_speed,
                     m_pos.y + std::sin(degToRad(m_direction)) * m_speed};

    // If we are heading directly towards the target and the target comes into
    // range, then we start our attack run.
    if (m_direction == directionToTarget) {
      const f32 distanceToTarget = distanceBetween(m_pos, m_travelTargetPos);
      if (distanceToTarget < kMaxEngagementRange) {
        m_task = Task::Attacking;
        // Shoot as soon as we're attacking.
        shoot();
      }
    }
  }

  if (m_task == Task::Attacking) {
    // We don't move any more, but we only travel forward.

    // If we are allowed to attack, then do it.
    if (m_timeSinceLastShot > 100.f) {
      shoot();
      m_timeSinceLastShot = 0.f;
    } else {
      m_timeSinceLastShot += adjustment;
    }

    m_speed = kMaxAttackSpeed;

    // If we have speed, update our position.
    m_pos = ca::Vec2{m_pos.x + std::cos(degToRad(m_direction)) * m_speed,
                     m_pos.y + std::sin(degToRad(m_direction)) * m_speed};

    const f32 directionToTarget = directionBetween(m_pos, m_travelTargetPos);
    if (std::abs(directionToTarget - m_direction) > kMaxTurnRadius) {
      // m_direction += (std::rand() % 2 == 0) ? 30.f : -30.f;
      m_task = Task::Egress;
    }
  }

  if (m_task == Task::Egress) {
    m_speed = kMaxTravelSpeed;
    m_direction = wrap(m_direction -= kMaxTurnRadius / 3.f, 0.f, 360.f);
    // If we have speed, update our position.
    m_pos = ca::Vec2{m_pos.x + std::cos(degToRad(m_direction)) * m_speed,
                     m_pos.y + std::sin(degToRad(m_direction)) * m_speed};

    const f32 distanceToTarget = distanceBetween(m_pos, m_travelTargetPos);
    if (distanceToTarget > kMaxEngagementRange * 1.5f) {
      m_task = Task::Nothing;
    }
  }

#if BUILD(DEBUG)
  {
    std::ostringstream ss;

    switch (m_task) {
      case Task::Nothing:
        ss << "Nothing";
        break;

      case Task::Travel:
        ss << "Travel";
        break;

      case Task::Attacking:
        ss << "Attacking";
        break;

      case Task::Egress:
        ss << "Egress";
        break;

      default:
        ss << "Unknown";
        break;
    }

    ss << '\n' << m_direction << " ("
       << directionBetween(m_pos, m_travelTargetPos) << ")\n"
       << distanceBetween(m_pos, m_travelTargetPos);

#if 0
    m_infoText.setString(ss.str());
    m_infoText.setPosition(m_pos);
#endif  // 0
  }
#endif
}

Object* EnemyShip::selectBestTarget() {
  Object* bestTarget = nullptr;

  // First find miners and then power generators to destroy, so that they can't
  // gather resources any more.
  bestTarget = m_universe->findClosestObjectOfType(m_pos, ObjectType::Miner);
  if (bestTarget) {
    return bestTarget;
  }

  bestTarget =
      m_universe->findClosestObjectOfType(m_pos, ObjectType::PowerRelay);
  if (bestTarget) {
    return bestTarget;
  }

  // If we can't find a resource gathering structure, start targeting defences.

  bestTarget = m_universe->findClosestObjectOfType(m_pos, ObjectType::Turret);
  if (bestTarget) {
    return bestTarget;
  }

  // As a last resort, attach the command center.
  return m_universe->findClosestObjectOfType(m_pos, ObjectType::CommandCenter);
}

void EnemyShip::createEngagementRangeShape() {
  const f32 kSpread = 45.f;
  const int kSteps = 9;
  static const ca::Color color{255, 0, 0, 127};

#if 0
  m_engagementRangeShape.setPrimitiveType(sf::TrianglesFan);
  m_engagementRangeShape.resize(2 + kSteps);

  m_engagementRangeShape[0].position = sf::Vector2f(75.f, 0.f);
  m_engagementRangeShape[0].color = color;

  size_t i = 1;
  const f32 spreadStep = kSpread / static_cast<f32>(kSteps);
  const f32 half = kSpread / -2.f;
  for (f32 degrees = 0.f; degrees <= kSpread; degrees += spreadStep, ++i) {
    m_engagementRangeShape[i].position.x =
        std::cos(degToRad(half + degrees)) * kMaxEngagementRange;
    m_engagementRangeShape[i].position.y =
        std::sin(degToRad(half + degrees)) * kMaxEngagementRange;
    m_engagementRangeShape[i].color = color;
  }
#endif  // 0
}

void EnemyShip::shoot() {
  auto bullet =
      std::make_unique<Bullet>(m_universe, m_pos, m_direction, m_speed * 2.f);
  m_universe->addObject(std::move(bullet));
}

void EnemyShip::onObjectRemoved(Object* object) {
  // If our target was removed, then we should do something else.
  if (object == m_target) {
    m_target = nullptr;
    m_task = Task::Nothing;
  }
}

Particle* EnemyShip::createSmokeParticle(ParticleEmitter* emitter,
                                         const ca::Vec2& pos) {
  auto particle = std::make_unique<Particle>(emitter, pos);
  return particle.release();
}

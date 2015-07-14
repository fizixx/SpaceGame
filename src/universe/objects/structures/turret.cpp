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

#include "universe/objects/projectiles/missile.h"
#include "universe/universe.h"
#include "utils/math.h"

DEFINE_STRUCTURE(Turret, "Turret", 750, 1000);

namespace {

const float kMaxAttachRange = 2500.f;

}  // namespace

Turret::Turret(Universe* universe, const ca::Vec2& pos)
  : Structure(universe, ObjectType::Turret, pos, 500) {
  m_renderComponent = std::make_unique<SpriteRenderComponent>(
      universe->getResourceManager(), ResourceManager::Texture::Unknown);

  // Create our 3 missiles.
  for (auto& missile : m_missiles) {
    missile = static_cast<Missile*>(m_universe->addObject(
        std::make_unique<Missile>(m_universe, m_pos, m_turretDirection)));
  }

  m_removedObjectSlotId = m_universe->getObjectRemovedSignal().connect(
      nu::slot(&Turret::onObjectRemoved, this));
}

Turret::~Turret() {
  m_universe->getObjectRemovedSignal().disconnect(m_removedObjectSlotId);

  // If we die and we have missiles on the rail, then our missiles die too.
  for (auto& missile : m_missiles) {
    if (missile && !missile->isLaunched()) {
      m_universe->removeObject(missile);
    }
  }
}

void Turret::shot(Projectile* projectile) {
  Structure::shot(projectile);
}

void Turret::moveTo(const ca::Vec2& pos) {
  Structure::moveTo(pos);
  for (auto& missile : m_missiles) {
    missile->moveTo(pos);
  }
}

void Turret::tick(f32 adjustment) {
  Structure::tick(adjustment);

  if (m_task == Task::Idle) {
    // TODO(tiaanl): Turn the rail so it looks like it's searching for a target.

    m_target = findBestTarget();
    if (m_target) {
      m_task = Task::Attacking;
    }
  }

  if (m_task == Task::Attacking) {
    // TODO(tiaanl): Turn the rail toward the target we're attacking.
    
    if (m_timeSinceLastShot > 100.f) {
      shoot();
      m_timeSinceLastShot = 0.f;
    } else {
      m_timeSinceLastShot += adjustment;
    }
  }
}

Object* Turret::findBestTarget() {
  // Just find the closest enemy ship for now.
  return m_universe->findClosestObjectOfType(m_pos, ObjectType::EnemyShip,
                                             kMaxAttachRange);
}

void Turret::shoot() {
  // DCHECK(m_target) << "To launch a missile we need a target.";

  // Find a missile that we can launch.
  Missile* ready = nullptr;
  for (usize i = 0; i < m_missiles.size(); ++i) {
    if (m_missiles[i] && !m_missiles[i]->isLaunched()) {
      ready = m_missiles[i];
      break;
    }
  }

  if (ready) {
    ready->launchAt(m_target);
  }
}

void Turret::onObjectRemoved(Object* object) {
  // If the object is one of our missiles, then we should create a missile in
  // it's place.
  for (usize i = 0; i < m_missiles.size(); ++i) {
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

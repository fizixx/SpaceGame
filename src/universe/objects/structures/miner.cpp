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

#include "universe/objects/structures/miner.h"

#include "canvas/math/transform.h"

#include "universe/universe.h"
#include "universe/objects/asteroid.h"

DEFINE_STRUCTURE(Miner, "Miner", -750, 1500);

Miner::Miner(Universe* universe, const ca::Vec2& pos)
  : Structure(universe, ObjectType::Miner, pos, 1500) {
  m_renderComponent = std::make_unique<SpriteRenderComponent>(
    universe->getResourceManager(), ResourceManager::Texture::Unknown);

  recreateLasers();

  m_removedObjectId = m_universe->getObjectRemovedSignal().connect(
      nu::slot(&Miner::onObjectRemoved, this));
}

Miner::~Miner() {
  m_universe->getObjectRemovedSignal().disconnect(m_removedObjectId);
}

void Miner::moveTo(const ca::Vec2& pos) {
  Structure::moveTo(pos);

  // We have move position, so recreate all our lasers.
  recreateLasers();
}

void Miner::tick(float adjustment) {
  Structure::tick(adjustment);

  if (m_lastMinedAsteroid > 100.f) {
    mineAsteroids();
    m_lastMinedAsteroid = 0.f;
  } else {
    m_lastMinedAsteroid += adjustment;
  }
}

void Miner::render(ca::Canvas* canvas, const ca::Mat4& transform) const {
  // Render the miner first.
  Structure::render(canvas, transform);

// Now render all the lasers.
#if 0
  for (const auto& laser : m_lasers) {
    target.draw(*laser, states);
  }
#endif  // 0
}

Miner::Laser::Laser(Universe* universe, Miner* miner, Asteroid* asteroid)
  : m_universe(universe), m_miner(miner), m_asteroid(asteroid) {
  // Set up the shape to point to the right thing.
  const float kLaserWidth{5.f};

  float xd = m_asteroid->getPos().x - m_miner->getPos().x;
  float yd = m_asteroid->getPos().y - m_miner->getPos().y;

  // Calculate the distance between the source and destination.
  float distance = std::sqrtf(xd * xd + yd * yd);

  // Calculate the angle between the source and destination.
  float angle = std::atan2(yd, xd) * 180.f / 3.1415f;

#if 0
  m_shape.setPosition(m_miner->getPos());
  m_shape.setSize(sf::Vector2f{kLaserWidth, distance});
  m_shape.setOrigin(sf::Vector2f{kLaserWidth / 2.f, 0.f});
  m_shape.setRotation(angle - 90.f);
#endif  // 0
}

Miner::Laser::~Laser() {
}

void Miner::Laser::render(ca::Canvas* canvas) const {
#if 0
  target.draw(m_shape, states);
#endif  // 0
}

void Miner::onObjectRemoved(Object* object) {
  // We just blindly recreate the lasers to handle the fact that one of our
  // asteroids might be gone.
  recreateLasers();
}

void Miner::recreateLasers() {
  // Clear out all the old lasers.
  m_lasers.clear();

  // Find a list of all the astroids in our range.
  std::vector<Object*> asteroids;
  m_universe->findObjectsInRadius(std::set<ObjectType>{ObjectType::Asteroid},
                                  m_pos, 500.f, &asteroids);
  if (asteroids.empty()) {
    return;
  }

  // Create lasers for each asteroid in range.
  for (auto& asteroid : asteroids) {
    m_lasers.push_back(std::make_unique<Laser>(
        m_universe, this, static_cast<Asteroid*>(asteroid)));
  }
}

void Miner::mineAsteroids() {
  for (auto& laser : m_lasers) {
    Asteroid* asteroid = laser->getAsteroid();
    // Mine the asteroid.
    int32_t amountMined = asteroid->mine(10);
    m_universe->adjustMinerals(amountMined);
  }
}

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

#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"
#include "universe/objects/asteroid.h"

DEFINE_STRUCTURE(Miner, "Miner", -750, 1500);

Miner::Miner(Universe* universe)
  : Structure(universe, ObjectType::Miner, 1500), m_shape(75.f) {
  m_shape.setFillColor(sf::Color{0, 255, 255, 255});
  m_shape.setOrigin(m_shape.getGlobalBounds().width / 2.f,
                    m_shape.getGlobalBounds().height / 2.f);

  recreateLasers();
}

Miner::~Miner() {
}

void Miner::moveTo(const sf::Vector2f& pos) {
  Structure::moveTo(pos);

  m_shape.setPosition(pos);

  // We have move position, so recreate all our lasers.
  recreateLasers();
}

sf::FloatRect Miner::getBounds() const {
  return m_shape.getGlobalBounds();
}

void Miner::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Draw all the lasers
  for (const auto& laser : m_lasers) {
    target.draw(*laser, states);
  }

  // Draw the miner itself.
  target.draw(m_shape, states);
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

  m_shape.setPosition(m_miner->getPos());
  m_shape.setSize(sf::Vector2f{kLaserWidth, distance});
  m_shape.setOrigin(sf::Vector2f{kLaserWidth / 2.f, 0.f});
  m_shape.setRotation(angle - 90.f);
}

Miner::Laser::~Laser() {
}

void Miner::Laser::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  target.draw(m_shape, states);
}

void Miner::recreateLasers() {
  // Clear out all the old lasers.
  m_lasers.clear();

  // Find a list of all the astroids in our range.
  std::vector<Object*> asteroids =
      m_universe->findObjectsInRadius(ObjectType::Asteroid, m_pos, 500.f);
  if (asteroids.empty()) {
    return;
  }

  // Create lasers for each asteroid in range.
  for (auto& asteroid : asteroids) {
    m_lasers.push_back(std::make_unique<Laser>(
        m_universe, this, static_cast<Asteroid*>(asteroid)));
  }
}

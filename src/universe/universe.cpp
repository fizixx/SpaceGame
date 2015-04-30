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

#include "universe/universe.h"

#include <cassert>

#include <cstdlib>
#include <algorithm>
#include <limits>
#include <memory>

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/link.h"
#include "universe/objects/asteroid.h"
#include "universe/objects/structures/command_center.h"
#include "universe/objects/structures/power_generator.h"

Universe::Universe(ResourceManager* resourceManager)
  : m_resourceManager(resourceManager) {
  // Create a dummy universe.

  auto commandCenter = createObject<CommandCenter>();
  commandCenter->moveTo(sf::Vector2f{0.f, 0.f});

  PowerGenerator* powerGenerator;

  powerGenerator = createObject<PowerGenerator>();
  powerGenerator->moveTo(sf::Vector2f{500.f, 250.f});

  powerGenerator = createObject<PowerGenerator>();
  powerGenerator->moveTo(sf::Vector2f{-450.f, 50.f});

  addLink(m_objects[0], m_objects[1]);
  addLink(m_objects[0], m_objects[2]);

#if 0
  createAsteroids(sf::Vector2f{0.f, 0.f}, 500.f, 5000.f, 100);
#endif  // 0
}

Universe::~Universe() {
  // Delete all the links we own.
  for (auto& link : m_links) {
    delete link;
  }

  // Delete all the objects we own.
  for (auto& object : m_objects) {
    delete object;
  }
}

void Universe::addObject(std::unique_ptr<Object> object) {
  if (m_useIncomingObjectList) {
    m_incomingObjects.push_back(object.release());
  } else {
    m_objects.push_back(object.release());
  }
}

void Universe::removeObject(Object* object) {
  if (m_useIncomingObjectList) {
    m_incomingRemoveObjects.push_back(object);
  } else {
    removeObjectInternal(object);
  }
}

void Universe::addLink(Object* source, Object* destination) {
  m_links.emplace_back(new Link{this, source, destination});
}

void Universe::removeLinksConnectedTo(Object* object) {
  // Find links that are either sources or destinations of the specified object.
  auto it = std::find_if(
      std::begin(m_links), std::end(m_links), [object](Link* link) {
        return link->getSource() == object || link->getDestination() == object;
      });

  if (it == std::end(m_links))
    return;

  m_links.erase(it);
}

Object* Universe::getClosestLinkObject(const sf::Vector2f& pos) const {
  // Go through each object and calculate the distance to the pos, overriding
  // the best one as we go.
  float bestDistance = std::numeric_limits<float>::max();
  Object* bestObject = nullptr;

  for (const auto& object : m_objects) {
    if (!object->canLink()) {
      continue;
    }

    float distance = object->calculateDistanceFrom(pos);
    if (distance < bestDistance) {
      bestDistance = distance;
      bestObject = object;
    }
  }

  return bestObject;
}

Object* Universe::findObjectAt(const sf::Vector2f& pos) const {
  for (size_t i = m_objects.size() - 1; i != -1; --i) {
    if (m_objects[i] && m_objects[i]->getBounds().contains(pos)) {
      return m_objects[i];
    }
  }

  // Nothing found.
  return nullptr;
}

std::vector<Object*> Universe::findObjectsInRadius(ObjectType objectType,
                                                   const sf::Vector2f& origin,
                                                   float radius) const {
  std::vector<Object*> results;

  for (const auto& object : m_objects) {
    if (objectType != object->getType()) {
      continue;
    }

    float distance = object->calculateDistanceFrom(origin);
    if (distance <= radius) {
      results.push_back(object);
    }
  }

  return std::move(results);
}

Object* Universe::findClosestObjectOfType(const sf::Vector2f& pos,
                                          ObjectType objectType) {
  float bestDistance{std::numeric_limits<float>::max()};
  Object* bestObject{nullptr};

  for (const auto& object : m_objects) {
    if (object->getType() != objectType) {
      continue;
    }

    float distance = object->calculateDistanceFrom(pos);
    if (distance < bestDistance) {
      bestDistance = distance;
      bestObject = object;
    }
  }

  return bestObject;
}

void Universe::adjustPower(int32_t amount) {
  m_totalPower += amount;
}

void Universe::adjustMinerals(int32_t amount) {
  m_totalMinerals += amount;
}

void Universe::tick(float adjustment) {
  // We start with 0 power so that we can calculate the total.
  m_totalPower = 0;

  m_useIncomingObjectList = true;

  // Update each object.
  for (auto& object : m_objects) {
    object->tick(adjustment);
  }

  m_useIncomingObjectList = false;

  // Check for collision between bullets and structures.
  for (auto& bullet : m_objects) {
    if (!bullet || bullet->getType() != ObjectType::Bullet) {
      continue;
    }

    assert(!!bullet);

    static const ObjectType kStructures[] = {ObjectType::CommandCenter,
                                             ObjectType::PowerGenerator,
                                             ObjectType::Miner};

    for (size_t i = 0; i < ARRAY_SIZE(kStructures); ++i) {
      std::vector<Object*> closeStructures =
          findObjectsInRadius(kStructures[i], bullet->getPos(), 10.f);
      // TODO: This will send a shot call to all objects even if the projectile
      // was deleted.
      for (auto& structure : closeStructures) {
        structure->shot(reinterpret_cast<Projectile*>(bullet));
      }
    }
  }

  // Remove items that is in the incoming remove list.
  if (!m_incomingRemoveObjects.empty()) {
    for (auto& incomingObject : m_incomingRemoveObjects) {
      removeObjectInternal(incomingObject);
    }
    m_incomingRemoveObjects.clear();
  }

  // Add any objects that might be in the incoming object list.
  if (!m_incomingObjects.empty()) {
    std::copy(std::begin(m_incomingObjects), std::end(m_incomingObjects),
              std::back_inserter(m_objects));
    m_incomingObjects.clear();
  }
}

void Universe::createAsteroids(const sf::Vector2f& origin, float minRadius,
                               float maxRadius, size_t count) {
  const float kPi = 3.1415f;

  for (size_t i = 0; i < count; ++i) {
    // Get a random direction between 0 and 360.
    float randDirection = static_cast<float>(std::rand() % 36000) / 100.f;

    // Get a random radius between 100 and the max radius.
    float randRadius =
        minRadius + static_cast<float>(
                        std::rand() % (static_cast<int32_t>(
                                          std::roundl(maxRadius) - minRadius)));

    // Get a random starting amount.
    int32_t mineralCount = (std::rand() % 100) + 500;

    sf::Vector2f pos{origin.x + randRadius * std::cosf(randDirection),
                     origin.y + randRadius * std::sinf(randDirection)};
    auto asteroid = createObject<Asteroid>(mineralCount);
    asteroid->moveTo(pos);
  }
}

void Universe::removeObjectInternal(Object* object) {
  // We don't erase the object from the list yet, we set it to null and then
  // delete it when it is convenient.
  auto it = std::find(std::begin(m_objects), std::end(m_objects), object);
  if (it == std::end(m_objects))
    return;

  std::unique_ptr<Object> obj(*it);

  // Remove all links that is connected to this.
  removeLinksConnectedTo(obj.get());
}

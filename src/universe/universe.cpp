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

#include <nucleus/logging.h>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/objects/asteroid.h"
#include "universe/objects/structures/command_center.h"
#include "universe/objects/structures/power_generator.h"
#include "utils/math.h"

Universe::Universe(ResourceManager* resourceManager)
  : m_resourceManager(resourceManager) {
  // Create a dummy universe.

  addObject(std::make_unique<CommandCenter>(this, sf::Vector2f{0.f, 0.f}));
  // addObject(std::make_unique<PowerGenerator>(this, sf::Vector2f{500.f, 250.f}));
  // addObject(std::make_unique<PowerGenerator>(this, sf::Vector2f{-450.f, 50.f}));

  createAsteroids(sf::Vector2f{0.f, 0.f}, 500.f, 5000.f, 100);
}

Universe::~Universe() {
  m_inDestructor = true;

  // Delete all the objects we own.
  for (auto& object : m_objects) {
    delete object;
  }
}

Object* Universe::addObject(std::unique_ptr<Object> object) {
  // If we are in the destructor, we don't do anything with the object, so it
  // will just be destroyed.
  if (m_inDestructor) {
    return nullptr;
  }

  Object* result = object.get();
  if (m_useIncomingObjectList) {
    m_incomingObjects.emplace_back(object.release());
  } else {
    addObjectInternal(object.release());
  }
  return result;
}

void Universe::removeObject(Object* object) {
  // We don't remove objects if we're in the destructor, because we're busy
  // deleting everything anyway.
  if (m_inDestructor) {
    return;
  }

  if (m_useIncomingObjectList) {
    m_incomingRemoveObjects.emplace_back(object);
  } else {
    removeObjectInternal(object);
  }
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

void Universe::findObjectsInRadius(ObjectType objectType,
                                   const sf::Vector2f& origin, float radius,
                                   std::vector<Object*>* objectsOut) const {
  // DCHECK(objectsOut);

  for (const auto& object : m_objects) {
    if (objectType != object->getType()) {
      continue;
    }

    float distance = object->calculateDistanceFrom(origin);
    if (distance <= radius) {
      objectsOut->emplace_back(object);
    }
  }
}

Object* Universe::findClosestObjectOfType(const sf::Vector2f& pos,
                                          ObjectType objectType,
                                          float maxRange) {
  float bestDistance{std::numeric_limits<float>::max()};
  Object* bestObject{nullptr};

  for (const auto& object : m_objects) {
    if (object->getType() != objectType) {
      continue;
    }

    float distance = object->calculateDistanceFrom(pos);

    if (distance > maxRange) {
      continue;
    }

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

void Universe::addRemoveObjectObserver(RemoveObjectObserver* observer) {
  m_removeObjectObservers.addObserver(observer);
}

void Universe::removeRemoveObjectObserver(RemoveObjectObserver* observer) {
  m_removeObjectObservers.removeObserver(observer);
}

void Universe::addObjectInternal(Object* object) {
  // Find the place where we have to insert the new object.
  auto it = std::lower_bound(std::begin(m_objects), std::end(m_objects), object,
                             [](Object* left, Object* right) {
                               return left->getType() < right->getType();
                             });

  // Insert the new object.
  m_objects.insert(it, object);
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
    int32_t mineralCount = (std::rand() % 1000) + 100;

    sf::Vector2f pos{origin.x + randRadius * std::cosf(randDirection),
                     origin.y + randRadius * std::sinf(randDirection)};
    addObject(std::make_unique<Asteroid>(this, pos, mineralCount));
  }
}

void Universe::removeObjectInternal(Object* object) {
  // We don't erase the object from the list yet, we set it to null and then
  // delete it when it is convenient.
  auto it = std::find(std::begin(m_objects), std::end(m_objects), object);
  if (it == std::end(m_objects)) {
    LOG(Error) << "Trying to delete object that doesn't exist!";
    return;
  }

  // Let all the observers know that this object is about to be removed.
  FOR_EACH_OBSERVER(RemoveObjectObserver, m_removeObjectObservers,
                    onRemovingObject(object));

  delete *it;
  m_objects.erase(it);

  // Let all the observers know that this object has been removed.
  FOR_EACH_OBSERVER(RemoveObjectObserver, m_removeObjectObservers,
                    onObjectRemoved(object));
}

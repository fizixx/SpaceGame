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

#include "nucleus/logging.h"

#include "universe/link.h"
#include "universe/objects/asteroid.h"
#include "universe/objects/structures/command_center.h"
#include "universe/objects/structures/power_relay.h"
#include "utils/math.h"

Universe::Universe(ResourceManager* resourceManager)
  : m_resourceManager(resourceManager) {
  // Create a dummy universe.

  addObject(std::make_unique<CommandCenter>(this, ca::Vec2{0.f, 0.f}));
  // addObject(std::make_unique<PowerGenerator>(this, sf::Vector2f{500.f,
  // 250.f}));
  // addObject(std::make_unique<PowerGenerator>(this, sf::Vector2f{-450.f,
  // 50.f}));

  createAsteroids(ca::Vec2{0.f, 0.f}, 500.f, 5000.f, 100);
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

Object* Universe::findObjectAt(const ca::Vec2& pos) const {
  for (usize i = m_objects.size() - 1; i != -1; --i) {
    if (m_objects[i] &&
        m_objects[i]->getBounds().contains(ca::Pos<f32>{pos.x, pos.y})) {
      return m_objects[i];
    }
  }

  // Nothing found.
  return nullptr;
}

void Universe::findObjectsInRadius(const std::set<ObjectType>& objectTypes,
                                   const ca::Vec2& origin, f32 radius,
                                   std::vector<Object*>* objectsOut) const {
  DCHECK(objectsOut);

  for (const auto& object : m_objects) {
    if (!objectTypes.count(object->getType())) {
      continue;
    }

    f32 distance = object->calculateDistanceFrom(origin);
    if (distance <= radius) {
      objectsOut->emplace_back(object);
    }
  }
}

Object* Universe::findClosestObjectOfType(const ca::Vec2& pos,
                                          ObjectType objectType, f32 maxRange) {
  f32 bestDistance{std::numeric_limits<f32>::max()};
  Object* bestObject{nullptr};

  for (const auto& object : m_objects) {
    if (object->getType() != objectType) {
      continue;
    }

    f32 distance = object->calculateDistanceFrom(pos);

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

void Universe::createLinksFor(Object* object) {
  if (!Object::isStructure(object)) {
    return;
  }

  std::vector<Object*> objectsToLinkTo;

  if (object->getType() == ObjectType::PowerRelay) {
    // If we are creating links from a power relay, we create a link between us
    // and every other power relay in range.
    findObjectsInRadius(Object::objectTypesForStructures(), object->getPos(),
                        1500.f, &objectsToLinkTo);
  } else {
    // If we are a normal structure, we create links between us and the closest
    // power relay.
    Object* linkTo = findClosestObjectOfType(object->getPos(),
                                             ObjectType::PowerRelay, 1500.f);
    if (linkTo) {
      objectsToLinkTo.emplace_back(linkTo);
    }
  }

  for (auto& linkTo : objectsToLinkTo) {
    // Can't link to ourselves.
    if (linkTo == object) {
      continue;
    }

    if (linkTo->getType() == ObjectType::PowerRelay) {
      // Make sure there isn't a link between object and linkTo already.
      auto it = std::find_if(std::begin(m_links), std::end(m_links),
                             [&object, &linkTo](Link* link) {
                               return link->getSource() == object &&
                                      link->getDestination() == linkTo;
                             });
      if (it != std::end(m_links)) {
        // We already have a link between these objects, so continue to the
        // next.
        continue;
      }

      // Add the new link.
      m_links.push_back(new Link{this, object, linkTo});
    }

    // We also create links the other way.
    createLinksFor(linkTo);
  }
}

void Universe::adjustPower(i32 amount) {
  m_totalPower += amount;
}

void Universe::adjustMinerals(i32 amount) {
  m_totalMinerals += amount;
}

void Universe::tick(f32 adjustment) {
  // We start with 0 power so that we can calculate the total.
  m_totalPower = 0;

  m_useIncomingObjectList = true;

  // Update each object.
  for (auto& object : m_objects) {
    object->tick(adjustment);
  }

  // Update all the links.
  for (auto& link : m_links) {
    link->tick(adjustment);
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

void Universe::addObjectInternal(Object* object) {
  // Find the place where we have to insert the new object.
  auto it = std::lower_bound(std::begin(m_objects), std::end(m_objects), object,
                             [](Object* left, Object* right) {
                               return left->getType() < right->getType();
                             });

  // Insert the new object.
  m_objects.insert(it, object);

  // Create links for the newly added object.
  createLinksFor(object);
}

void Universe::createAsteroids(const ca::Vec2& origin, f32 minRadius,
                               f32 maxRadius, usize count) {
  for (usize i = 0; i < count; ++i) {
    // Get a random direction between 0 and 360.
    f32 randDirection = static_cast<f32>(std::rand() % 36000) / 100.f;

    // Get a random radius between 100 and the max radius.
    f32 randRadius =
        minRadius + static_cast<f32>(std::rand() %
                                     (static_cast<i32>(
                                         std::roundl(maxRadius) - minRadius)));

    // Get a random starting amount.
    i32 mineralCount = (std::rand() % 1000) + 100;

    ca::Vec2 pos{origin.x + randRadius * std::cosf(randDirection),
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

  delete *it;
  m_objects.erase(it);

  // Emit the signal that the specified object has been removed.
  m_objectRemovedSignal.emit(object);
}

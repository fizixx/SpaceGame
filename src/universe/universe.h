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

#ifndef UNIVERSE_UNIVERSE_H_
#define UNIVERSE_UNIVERSE_H_

#include <memory>
#include <set>
#include <vector>

#include <nucleus/macros.h>
#include <nucleus/utils/signals.h>

#include "game/resource_manager.h"
#include "universe/camera.h"
#include "universe/objects/object.h"

class Link;
class Object;

class Universe {
public:
  using ObjectRemovedSignal = nu::Signal<void(Object*)>;

  // Construct the universe with the specified viewport size.
  explicit Universe(ResourceManager* resourceManager);
  ~Universe();

  // Return the resource manager attached to this universe.
  ResourceManager* getResourceManager() const { return m_resourceManager; }

  // Add or remove objects from the universe.
  Object* addObject(std::unique_ptr<Object> object);
  void removeObject(Object* object);

  // Find the object that is at the specified location.  This function takes
  // z-order into account for objects that might be overlapping.
  Object* findObjectAt(const ca::Vec2& pos) const;

  // Find a list of objects with in a radius to the origin with the specified
  // type.
  void findObjectsInRadius(const std::set<ObjectType>& objectTypes,
                           const ca::Vec2& origin, float radius,
                           std::vector<Object*>* objectsOut) const;

  // Find the closest object to the given position of the specified type.
  Object* findClosestObjectOfType(
      const ca::Vec2& pos, ObjectType objectType,
      float maxRange = std::numeric_limits<float>::max());

  // Create links for the specified object.  This will only create links in one
  // direction.
  void createLinksFor(Object* object);

  // Power
  int32_t getPower() const { return m_totalPower; }
  void adjustPower(int32_t amount);

  // Minerals
  int32_t getMinerals() const { return m_totalMinerals; }
  void adjustMinerals(int32_t amount);

  // Update the entire universe.  This should run at 60fps.
  void tick(f32 adjustment);

  // Signal that will let slots know that we removed an object.
  ObjectRemovedSignal& getObjectRemovedSignal() {
    return m_objectRemovedSignal;
  }

private:
  friend class UniverseView;

  // Add an object internally.  This keeps the list of objects sorted in the
  // correct order.
  void addObjectInternal(Object* object);

  // Create count number of asteroids within the given radius around the given
  // origin.
  void createAsteroids(const ca::Vec2& origin, float minRadius, float maxRadius,
                       size_t count);

  // Do the actual work of deleting an object.
  void removeObjectInternal(Object* object);

  // The resource manager we load everything from.
  ResourceManager* m_resourceManager{nullptr};

  // The entire list of objects that exist in the universe.
  std::vector<Object*> m_objects;

  // If this is true, we add objects to the incoming list and not directly to
  // the object list.
  bool m_useIncomingObjectList{false};

  // A list of objects that was added while we were updateing other objects.
  std::vector<Object*> m_incomingObjects;

  // A list used for all objects that need to be deleted.
  std::vector<Object*> m_incomingRemoveObjects;

  // All the links that exist in the universe.
  std::vector<Link*> m_links;

  // Whether we are in the destructor or not.  If we are in the destructor, we
  // don't add or remove any more objects.
  bool m_inDestructor{false};

  // The total amount of power in the universe.
  int32_t m_totalPower{0};

  // The total amount of minerals in the universe.
  int32_t m_totalMinerals{5000};

  // Signal that is emitted when an object is removed from the universe.
  ObjectRemovedSignal m_objectRemovedSignal;

  DISALLOW_COPY_AND_ASSIGN(Universe);
};

#endif  // UNIVERSE_UNIVERSE_H_

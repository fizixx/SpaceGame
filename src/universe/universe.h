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
#include <vector>

#include <nucleus/macros.h>

#include "game/resource_manager.h"
#include "universe/camera.h"

class Link;
class Object;

class Universe {
public:
  // Construct the universe with the specified viewport size.
  explicit Universe(ResourceManager* resourceManager);
  ~Universe();

  // Return the resource manager attached to this universe.
  ResourceManager* getResourceManager() const { return m_resourceManager; }

  // Add or remove objects from the universe.
  void addObject(std::unique_ptr<Object> object);
  void removeObject(Object* object);

  // Create an object in the universe.
  template <typename ObjectType>
  ObjectType* createObject() {
    // Insert the new object into the universe.
    ObjectType* obj = new ObjectType{this};
    m_objects.emplace_back(obj);
    return obj;
  }

  // Add or remove links.
  void addLink(Object* source, Object* destination);
  void removeLinksConnectedTo(Object* object);

  // Given a position, find the closest object that can be linked to.  Returns
  // null if no point could be found.
  Object* getClosestLinkObject(const sf::Vector2f& pos) const;

  // Find the object that is at the specified location.  This function takes
  // z-order into account for objects that might be overlapping.
  Object* findObjectAt(const sf::Vector2f& pos);

  // Power
  int32_t getPower() const { return m_totalPower; }
  void adjustPower(int32_t amount);

  // Minerals
  int32_t getMinerals() const { return m_totalMinerals; }
  void adjustMinerals(int32_t amount);

  // Update the entire universe.  This should run at 60fps.
  void tick(float adjustment);

private:
  friend class UniverseView;

  // The resource manager we load everything from.
  ResourceManager* m_resourceManager{nullptr};

  // The entire list of links that exist in the universe.
  std::vector<Link*> m_links;

  // The entire list of objects that exist in the universe.
  std::vector<Object*> m_objects;

  // The total amount of power in the universe.
  int32_t m_totalPower{0};

  // The total amount of minerals in the universe.
  int32_t m_totalMinerals{0};

  DISALLOW_COPY_AND_ASSIGN(Universe);
};

#endif  // UNIVERSE_UNIVERSE_H_

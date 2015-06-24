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

#ifndef UNIVERSE_OBJECTS_OBJECT_H_
#define UNIVERSE_OBJECTS_OBJECT_H_

#include <cstdint>
#include <set>

#include "canvas/math/vec2.h"
#include "canvas/math/mat4.h"
#include "canvas/rendering/canvas.h"
#include "canvas/utils/rect.h"
#include "nucleus/macros.h"

class Projectile;
class Universe;

#define DECLARE_OBJECT(ClassName)                                              \
  \
public:                                                                        \
  static const char* typeName;                                                 \
  virtual const char* getTypeName() const { return ClassName::typeName; }

#define DEFINE_OBJECT(ClassName, Label) const char* ClassName::typeName = Label

// Objects are also rendered in this order, so take care when adding new types.
enum class ObjectType {
  // Asteroidst
  Asteroid,

  // Structures
  CommandCenter,
  PowerRelay,
  Miner,
  Turret,

  // Units
  EnemyShip,

  // Projectiles
  Bullet,
  Missile,
};

class Object {
  DECLARE_OBJECT(Object);

public:
  static std::set<ObjectType> objectTypesForStructures();

  static bool isAsteroid(Object* object);
  static bool isStructure(Object* object);
  static bool isProjectile(Object* object);
  static bool isUnit(Object* object);

  Object(Universe* universe, ObjectType type, const ca::Vec2& pos);
  virtual ~Object();

  // objectType
  ObjectType getType() const { return m_objectType; }

  // pos
  const ca::Vec2& getPos() const { return m_pos; }

  // Calculate the distance from pos to this object.
  float calculateDistanceFrom(const ca::Vec2& pos) const;

  // This is called when we are shot by the specified projectile.
  virtual void shot(Projectile* projectile);

  // Move the object to the specified coordinates.
  virtual void moveTo(const ca::Vec2& pos);

  // Return the bounds of the object.
  virtual ca::Rect<f32> getBounds() const = 0;

  // Tick the object.
  virtual void tick(float adjustment) = 0;

  // Render the object.
  virtual void render(ca::Canvas* canvas, const ca::Mat4& transform) const = 0;

protected:
  // The universe we belong to.
  Universe* m_universe;

  // The type of object.
  ObjectType m_objectType;

  // The position of the object in universe coordinates.
  ca::Vec2 m_pos;

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(Object);
};

#endif  // UNIVERSE_OBJECTS_OBJECT_H_

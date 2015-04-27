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

#include <nucleus/macros.h>
#include <SFML/Graphics/Drawable.hpp>

class Universe;

#define DECLARE_OBJECT(ClassName)                                              \
  \
public:                                                                        \
  static const char* typeName;                                                 \
  virtual const char* getTypeName() const { return ClassName::typeName; }

#define DEFINE_OBJECT(ClassName, Label) const char* ClassName::typeName = Label

enum class ObjectType {
  None,
  CommandCenter,
  PowerGenerator,
  Miner,
  Asteroid,
};

class Object : public sf::Drawable {
  DECLARE_OBJECT(Object);

public:
  explicit Object(Universe* universe, ObjectType type);
  virtual ~Object();

  // objectType
  ObjectType getType() const { return m_objectType; }

  // pos
  const sf::Vector2f& getPos() const { return m_pos; }

  // Calculate the distance from pos to this object.
  float calculateDistanceFrom(const sf::Vector2f& pos) const;

  // Return true if we can link to this object.  Default is false.
  virtual bool canLink() const;

  // Move the object to the specified coordinates.
  virtual void moveTo(const sf::Vector2f& pos);

  // Return the bounds of the object.
  virtual sf::FloatRect getBounds() const = 0;

  // Tick the object.
  virtual void tick(float adjustment) = 0;

protected:
  // The universe we belong to.
  Universe* m_universe;

  // The type of object.
  ObjectType m_objectType;

  // The position of the object in universe coordinates.
  sf::Vector2f m_pos;

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(Object);
};

#endif  // UNIVERSE_OBJECTS_OBJECT_H_

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

#ifndef UNIVERSE_OBJECTS_STRUCTURES_STRUCTURE_H_
#define UNIVERSE_OBJECTS_STRUCTURES_STRUCTURE_H_

#include "universe/objects/destructible_object.h"

#define DECLARE_STRUCTURE(ClassName)                                           \
  \
public:                                                                        \
  static const int32_t powerCost;                                              \
  static const int32_t mineralCost;                                            \
  virtual int32_t getPowerCost() { return ClassName::powerCost; }              \
  virtual int32_t getMineralCost() { return ClassName::mineralCost; }          \
  DECLARE_OBJECT(ClassName);

#define DEFINE_STRUCTURE(ClassName, Label, PowerCost, MineralCost)             \
  const int32_t ClassName::powerCost = PowerCost;                              \
  const int32_t ClassName::mineralCost = MineralCost;                          \
  DEFINE_OBJECT(ClassName, Label)

class Structure : public DestructibleObject {
  DECLARE_STRUCTURE(Structure);

public:
  Structure(Universe* universe, ObjectType objectType, const sf::Vector2f& pos,
            int32_t hitPoints);
  ~Structure() override;

  // Override: Object
  bool canLink() const { return true; }
  void tick(float adjustment) override;

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(Structure);
};

#endif  // UNIVERSE_OBJECTS_STRUCTURES_STRUCTURE_H_

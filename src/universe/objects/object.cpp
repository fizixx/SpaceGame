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

#include "universe/objects/object.h"

#include "universe/universe.h"
#include "utils/math.h"
#include "utils/stream_operators.h"

DEFINE_OBJECT(Object, "Object");

Object::Object(Universe* universe, ObjectType objectType)
  : m_universe(universe), m_objectType(objectType) {
}

Object::~Object() {
}

bool Object::canLink() const {
  return false;
}

void Object::moveTo(const sf::Vector2f& pos) {
  m_pos = pos;
}

float Object::calculateDistanceFrom(const sf::Vector2f& pos) const {
  return distanceBetween(m_pos, pos);
}

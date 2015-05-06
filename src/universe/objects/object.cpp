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

// static
bool Object::isAsteroid(Object* object) {
  return object->getType() == ObjectType::Asteroid;
}

// static
bool Object::isStructure(Object* object) {
  switch (object->getType()) {
    case ObjectType::CommandCenter:
    case ObjectType::PowerGenerator:
    case ObjectType::Miner:
    case ObjectType::Turret:
      return true;

    default:
      return false;
  }
}

// static
bool Object::isProjectile(Object* object) {
  switch (object->getType()) {
    case ObjectType::Bullet:
    case ObjectType::Missile:
      return true;

    default:
      return false;
  }
}

// static
bool Object::isUnit(Object* object) {
  switch (object->getType()) {
    case ObjectType::EnemyShip:
      return true;

    default:
      return false;
  }
}

Object::Object(Universe* universe, ObjectType objectType,
               const sf::Vector2f& pos)
  : m_universe(universe), m_objectType(objectType), m_pos(pos) {
}

Object::~Object() {
}

void Object::shot(Projectile* projectile) {
  // By default we do nothing when we are shot.
}

void Object::moveTo(const sf::Vector2f& pos) {
  m_pos = pos;
}

float Object::calculateDistanceFrom(const sf::Vector2f& pos) const {
  return distanceBetween(m_pos, pos);
}

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

#include "universe/link.h"

#include "universe/objects/object.h"
#include "utils/math.h"

Link::Link(Universe* universe, Object* source, Object* destination)
  : m_universe(universe), m_source(source), m_destination(destination) {
}

Link::~Link() {
}

void Link::tick(f32 adjustment) {
  // Calculate the geometry of the shape.
  // Get the positions of the objects.
  ca::Vec2 sourcePos{m_source->getPos()};
  ca::Vec2 destinationPos{m_destination->getPos()};

  // Get the direction and distance.
  float directionToDestination = directionBetween(sourcePos, destinationPos);
  float distanceToDestination = distanceBetween(sourcePos, destinationPos);

#if 0
  m_shape.setSize(sf::Vector2f{distanceToDestination, 5.f});
  m_shape.setOrigin(sf::Vector2f{0.f, -10.f});
  m_shape.setRotation(directionToDestination);
#endif  // 0
}

void Link::render(ca::Canvas* canvas) const {
#if 0
  states.transform.translate(m_source->getPos());
  target.draw(m_shape, states);
#endif  // 0
}

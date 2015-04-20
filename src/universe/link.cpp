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

#include <cmath>

#include <nucleus/logging.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/objects/object.h"

Link::Link(Universe* universe, Object* source, Object* destination)
  : m_universe(universe), m_source(source), m_destination(destination) {
  m_shape.setFillColor(sf::Color{255, 0, 255, 255});
  calculateShape();
}

Link::~Link() {
}

void Link::setSource(Object* source) {
  m_source = source;
  calculateShape();
}

void Link::setDestination(Object* destination) {
  m_destination = destination;
  calculateShape();
}

void Link::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  if (m_source && m_destination) {
    target.draw(m_shape);
  }
}

void Link::calculateShape() {
  if (!m_source || !m_destination) {
    return;
  }

  const float kLinkWidth{15.f};

  float xd = m_destination->getPos().x - m_source->getPos().x;
  float yd = m_destination->getPos().y - m_source->getPos().y;

  // Calculate the distance between the source and destination.
  float distance = std::sqrtf(xd * xd + yd * yd);

  // Calculate the angle between the source and destination.
  float angle = std::atan2(yd, xd) * 180.f / 3.1415f;

  m_shape.setPosition(m_source->getPos());
  m_shape.setSize(sf::Vector2f{kLinkWidth, distance});
  m_shape.setOrigin(sf::Vector2f{kLinkWidth / 2.f, 0.f});
  m_shape.setRotation(angle - 90.f);
}

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

#include "universe/objects/structures/command_center.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"

DEFINE_STRUCTURE(CommandCenter, "Command Center", 2000, 0);

CommandCenter::CommandCenter(Universe* universe)
  : Structure(universe, ObjectType::CommandCenter), m_shape(100.f) {
  m_shape.setOrigin(m_shape.getGlobalBounds().width / 2.f,
                    m_shape.getGlobalBounds().height / 2.f);
}

CommandCenter::~CommandCenter() {
}

void CommandCenter::moveTo(const sf::Vector2f& pos) {
  Structure::moveTo(pos);
  m_shape.move(pos);
}

sf::FloatRect CommandCenter::getBounds() const {
  return m_shape.getGlobalBounds();
}

void CommandCenter::tick(float adjustment) {
  m_universe->adjustPower(1000);
}

void CommandCenter::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  target.draw(m_shape);
}

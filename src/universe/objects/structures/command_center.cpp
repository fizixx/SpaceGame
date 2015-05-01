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

CommandCenter::CommandCenter(Universe* universe, const sf::Vector2f& pos)
  : Structure(universe, ObjectType::CommandCenter, pos, 5000) {
  m_texture = m_universe->getResourceManager()->getTexture(
      ResourceManager::Texture::CommandCenter);

  if (m_texture) {
    m_texture->setSmooth(false);
    m_shape.setTexture(*m_texture);
    //m_shape.setScale(2.f, 2.f);
    sf::FloatRect bounds = m_shape.getLocalBounds();
    m_shape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  }
}

CommandCenter::~CommandCenter() {
}

sf::FloatRect CommandCenter::getBounds() const {
  sf::FloatRect bounds = m_shape.getGlobalBounds();
  bounds.left += m_pos.x;
  bounds.top += m_pos.y;
  return bounds;
}

void CommandCenter::tick(float adjustment) {
  m_universe->adjustPower(1000);
}

void CommandCenter::draw(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  states.transform.translate(m_pos);
  target.draw(m_shape, states);
}

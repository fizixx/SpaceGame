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

#include "universe/hud.h"

#include "universe/objects/object.h"
#include "universe/universe_view.h"

Hud::Hud(UniverseView* universeView) : m_universeView(universeView) {
}

Hud::~Hud() {
}

void Hud::tick(float adjustment) {
}

void Hud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Get the selected object from the view.
  Object* selectedObject = m_universeView->getSelectedObject();
  if (selectedObject) {
    drawSelectedObject(target, states, selectedObject);
  }
}

void Hud::drawSelectedObject(sf::RenderTarget& target, sf::RenderStates states,
                             Object* object) const {
  const Camera& camera = m_universeView->getCamera();

  // Get the bounds of the object.
  sf::FloatRect bounds = object->getBounds();

  const sf::Vector2i topLeft =
      camera.universePosToMousePos(sf::Vector2f{bounds.left, bounds.top});
  const sf::Vector2i bottomRight = camera.universePosToMousePos(
      sf::Vector2f{bounds.left + bounds.width, bounds.top + bounds.height});

  sf::RectangleShape shape{
      sf::Vector2f{static_cast<float>(bottomRight.x - topLeft.x + 8),
                   static_cast<float>(bottomRight.y - topLeft.y + 8)}};
  shape.setPosition(sf::Vector2f{static_cast<float>(topLeft.x) - 4,
                                 static_cast<float>(topLeft.y) - 4});
  shape.setOutlineThickness(2);
  shape.setFillColor(sf::Color{0, 0, 0, 0});

  target.draw(shape, states);
}

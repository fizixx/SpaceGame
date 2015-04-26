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
#include "universe/universe.h"
#include "universe/universe_view.h"

Hud::Hud(UniverseView* universeView)
  : m_universeView(universeView), m_universe(universeView->getUniverse()) {
  // Set up the hover shape.
  m_hoverShape.setFillColor(sf::Color{0, 0, 0, 0});
  m_hoverShape.setOutlineThickness(2);
  m_hoverShape.setOutlineColor(sf::Color{0, 255, 0, 255});
}

Hud::~Hud() {
}

void Hud::updateUniverseMousePos(const sf::Vector2f& universeMousePos) {
  const Camera& camera = m_universeView->getCamera();

  // Find the hover object.
  m_hoverObject = m_universe->findObjectAt(universeMousePos);
  if (m_hoverObject) {
    adjustShapeOverObject(m_hoverObject, &m_hoverShape,
                          m_universeView->getCamera(), 4);
  }
}

void Hud::tick(float adjustment) {
}

void Hud::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Draw the shape over the hover object.
  if (m_hoverObject) {
    target.draw(m_hoverShape, states);
  }
#if 0
  // Get the selected object from the view.
  Object* selectedObject = m_universeView->getSelectedObject();
  if (selectedObject) {
    drawSelectedObject(target, states, selectedObject);
  }
#endif  // 0
}

void Hud::adjustShapeOverObject(Object* object, sf::RectangleShape* shape,
                                const Camera& camera, int borderSize) {
  // Get the bounds of the object.
  sf::FloatRect bounds{object->getBounds()};

  // Convert the bounds to hud coordinates.
  sf::Vector2f topLeft{
      camera.universePosToMousePos(sf::Vector2f{bounds.left, bounds.top})};
  sf::Vector2f bottomRight{camera.universePosToMousePos(
      sf::Vector2f{bounds.left + bounds.width, bounds.top + bounds.height})};

  shape->setSize(sf::Vector2f{
      static_cast<float>(bottomRight.x - topLeft.x + borderSize * 2),
      static_cast<float>(bottomRight.y - topLeft.y + borderSize * 2)});
  shape->setPosition(sf::Vector2f{static_cast<float>(topLeft.x) - borderSize,
                                  static_cast<float>(topLeft.y) - borderSize});
}

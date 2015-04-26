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

#ifndef UNIVERSE_HUD_H_
#define UNIVERSE_HUD_H_

#include <nucleus/macros.h>

#include <SFML/Graphics/RectangleShape.hpp>

#include "utils/component.h"

class Object;
class Universe;
class UniverseView;

class Hud : public Component {
public:
  explicit Hud(UniverseView* universeView);
  ~Hud() override;

  // Called from the UniverseView whenever the mouse position has changed.
  void updateUniverseMousePos(const sf::Vector2f& universeMousePos);

  // Handle mouse presses.  If we return true, it means we clicked on something
  // special and would like to receive an onMouseReleased.
  bool onMousePressed(sf::Event& event);

  // Handle a mouse release.  If we did not return true from the onMousePressed,
  // then we won't receive this.
  void onMouseReleased(sf::Event& event);

  // Override: Component
  void tick(float adjustment) override;
  void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
  // Shapes the given rectangle around the give object.
  void adjustShapeOverObject(Object* object, sf::RectangleShape* shape,
                             int borderSize = 4);

  // The universe we're operating on.
  UniverseView* m_universeView;

  // A shortcut to the universe that the m_universeView is looking at.
  Universe* m_universe;

  // The current position of the mouse inside the universe.
  sf::Vector2f m_universeMousePos;

  // The object currently under the mouse pointer at any given time.  null if
  // there are no objects under the mouse.
  Object* m_hoverObject{nullptr};

  // The shape used to render over the hover object.
  sf::RectangleShape m_hoverShape;

  // The object that is currently selected.  null if no object is currently
  // selected.
  Object* m_selectedObject{nullptr};

  // The shape used to render over the selected object.
  sf::RectangleShape m_selectedShape;

  // The object that we mouse down'd on.
  Object* m_mouseDownObject{nullptr};

  DISALLOW_COPY_AND_ASSIGN(Hud);
};

#endif  // UNIVERSE_HUD_H_

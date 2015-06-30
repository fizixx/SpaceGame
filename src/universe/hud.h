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

#include "canvas/math/vec2.h"
#include "nucleus/macros.h"

#include "utils/component.h"

class Object;
class Universe;
class UniverseView;

class Hud : public Component {
public:
  explicit Hud(UniverseView* universeView);
  ~Hud() override;

  // Called from the UniverseView whenever the mouse position has changed.
  void updateUniverseMousePos(const ca::Vec2& universeMousePos);

  // Set the object that the mouse is currently hovering over.
  void setHoverObject(Object* object);

  // Set the currently selected object.
  void setSelectedObject(Object* object);

  // Override: Component
  void tick(f32 adjustment) override;
  void render(ca::Canvas* canvas) const override;

private:
  // The universe we're operating on.
  UniverseView* m_universeView;

  // A shortcut to the universe that the m_universeView is looking at.
  Universe* m_universe;

  // The current position of the mouse inside the universe.
  ca::Vec2 m_universeMousePos;

  // The object currently under the mouse pointer at any given time.  null if
  // there are no objects under the mouse.
  Object* m_hoverObject{nullptr};

  // The shape used to render over the hover object.
#if 0
  sf::RectangleShape m_hoverShape;
#endif  // 0

  // The object that is currently selected.  null if no object is currently
  // selected.
  Object* m_selectedObject{nullptr};

  // The shape used to render over the selected object.
#if 0
  sf::RectangleShape m_selectedShape;
#endif  // 0

  // The object that we mouse down'd on.
  Object* m_mouseDownObject{nullptr};

  DISALLOW_COPY_AND_ASSIGN(Hud);
};

#endif  // UNIVERSE_HUD_H_

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

#ifndef UNIVERSE_UNIVERSE_VIEW_H_
#define UNIVERSE_UNIVERSE_VIEW_H_

#include <memory>

#include <nucleus/config.h>

#include "ui/views/color_view.h"
#include "universe/camera.h"

#if BUILD(DEBUG)
#define SHOW_UNIVERSE_MOUSE_POS 0
#endif

class Universe;
class Object;
class Link;

class UniverseView : public ui::View {
public:
  explicit UniverseView(ui::Context* context, Universe* universe = nullptr);
  virtual ~UniverseView() override;

  // Start placing the given object.
  void startPlacingObject(std::unique_ptr<Object> object);

  // Override: ui::View
  virtual bool onMousePressed(sf::Event& event) override;
  virtual bool onMouseDragged(sf::Event& event) override;
  virtual void onMouseMoved(sf::Event& event) override;
  virtual void onMouseReleased(sf::Event& event) override;
  virtual void onMouseWheel(sf::Event& event) override;
  virtual void tick(float adjustment) override;
  virtual void layout(const sf::IntRect& rect) override;
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

private:
  // Make the needed changes to the view if the perceived mouse position
  // changed.
  void onMouseMovedInternal(const sf::Vector2f& mousePos);

  // Find the object that is currently under the mouse.
  void updateHoverObject();

  // The universe we are looking at.
  Universe* m_universe;

  // The camera we use to look into the universe.
  Camera m_camera;

  // The last postition where the mouse cursor was in view coordinates.
  sf::Vector2i m_viewMousePos;

  // The current mouse position in the universe.
  sf::Vector2f m_universeMousePos;

  // The current object that is being hovered over.
  Object* m_hoverObject{nullptr};

  // The current object that is selected.
  Object* m_selectedObject{nullptr};

  // The rectangle we use to draw the hover outline.
  sf::RectangleShape m_hoverShape;

  // The rectangle we use to draw the selected outline.
  sf::RectangleShape m_selectedShape;

  // The current ghost object.  We own this pointer as it's not part of the
  // universe yet.
  std::unique_ptr<Object> m_ghostObject;

  // The current ghost link.  We draw this link between the closest link object
  // and the ghost object.
  std::unique_ptr<Link> m_ghostLink;

#if SHOW_UNIVERSE_MOUSE_POS
  // A shape to show where the current mouse position is in the universe.
  sf::CircleShape m_mousePosShape;
#endif

  DISALLOW_IMPLICIT_CONSTRUCTORS(UniverseView);
};

#endif  // UNIVERSE_UNIVERSE_VIEW_H_

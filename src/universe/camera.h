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

#ifndef UNIVERSE_CAMERA_H_
#define UNIVERSE_CAMERA_H_

#include <nucleus/macros.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

class Camera : public sf::Drawable {
public:
  Camera();
  ~Camera();

  // Return our current view.
  const sf::View& getView() const { return m_view; }

  // Given a mouse position in the viewport, return the universe position.
  sf::Vector2f mousePosToUniversePos(const sf::Vector2f& mousePos) const;

  // Handle input.
  void onMousePressed(sf::Event& event);
  void onMouseDragged(sf::Event& event);
  void onMouseReleased(sf::Event& event);
  void onMouseWheel(sf::Event& event);

  // Tick the universe.
  void tick(float adjustment);

  // Set the camera dimensions.
  void layout(const sf::IntRect& rect);

  // Override: sf::Drawable
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

private:
  // Calculate a view taking the position and zoom level into account.
  void updateView();

  // The size of the viewport we're looking into.
  sf::Vector2f m_viewportSize;

  // This is set to true if we are currently dragging the viewport around.
  bool m_isDraggingView{false};

  // This is the position where we started to drag the view from.
  sf::Vector2f m_startDragViewPos;

  // The current position of the camera.
  sf::Vector2f m_cameraPos;

  // The location where we want the camera to animate to.
  sf::Vector2f m_cameraTarget;

  // The current zoom level of the camera.
  float m_zoomLevel{1.f};

  // The target zoom level for the camera.
  float m_targetZoomLevel{1.f};

  // The final calculated view we use to translate everything.
  sf::View m_view;

  // A circle we use to render the camera target.
  sf::CircleShape m_cameraTargetShape{100.f};

  DISALLOW_COPY_AND_ASSIGN(Camera);
};

#endif  // UNIVERSE_CAMERA_H_

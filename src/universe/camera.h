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

#include <cstdint>

#include "canvas/math/mat4.h"
#include "canvas/rendering/sprite.h"
#include "canvas/utils/rect.h"
#include "elastic/context.h"
#include "nucleus/config.h"
#include "nucleus/macros.h"

#include "utils/component.h"

#if BUILD(DEBUG)
#define SHOW_CAMERA_TARGET 1
#endif

class Camera : public Component {
public:
  explicit Camera(el::Context* context);
  ~Camera() = default;

  // Set the camera dimensions.
  void setViewportDimensions(const ca::Rect<i32>& rect);

  // Return our current view.
  const ca::Mat4& getView() const { return m_view; }

  // Given a mouse position in the viewport, return the universe position.
  ca::Vec2 mousePosToUniversePos(const ca::Pos<i32>& mousePos) const;

  // Given a universe position, convert it to a position in the camera (view).
  ca::Pos<i32> universePosToMousePos(const ca::Vec2& universePos) const;

  // Adjust the camera position by the delta given.
  void adjustPosition(const ca::Pos<i32>& delta);

  // Adjust the camera zoom level.
  void adjustZoom(i32 delta);

  // Override: sf::Drawable
  void tick(float adjustment) override;
  void render(ca::Canvas* canvas) const override;

private:
  // Calculate a view taking the position and zoom level into account.
  void updateView();

  // The size of the viewport we're looking into.
  ca::Size<i32> m_viewportSize;

  // The current position of the camera.
  ca::Vec2 m_cameraPos;

  // The current zoom level of the camera.
  float m_zoomLevel{1.f};

  // The location where we want the camera to animate to.
  ca::Vec2 m_cameraTarget;

  // The target zoom level for the camera.
  float m_targetZoomLevel{1.f};

  // The final calculated transform we use to translate everything.
  ca::Mat4 m_view;

  // --------------

  // This is set to true if we are currently dragging the viewport around.
  bool m_isDraggingView{false};

  // This is the position where we started to drag the view from.
  ca::Vec2 m_startDragViewPos;

#if SHOW_CAMERA_TARGET
  // A circle we use to render the camera target.
  ca::Sprite m_cameraTargetSprite;
#endif

  DISALLOW_COPY_AND_ASSIGN(Camera);
};

#endif  // UNIVERSE_CAMERA_H_

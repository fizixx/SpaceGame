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

#include "universe/camera.h"

#include "canvas/math/transform.h"

Camera::Camera(el::Context* context) {
#if SHOW_CAMERA_TARGET
  m_cameraTargetSprite.setTexture(context->getTexture("camera_target"));
#endif
}

void Camera::setViewportDimensions(const ca::Rect<i32>& rect) {
  m_viewportSize = rect.size;
  updateView();
}

ca::Vec2 Camera::mousePosToUniversePos(const ca::Pos<i32>& mousePos) const {
  const float width = static_cast<float>(m_viewportSize.width);
  const float height = static_cast<float>(m_viewportSize.height);
  const ca::Rect<f32> viewport{0.f, 0.f, 1.f, 1.f};

  const ca::Rect<i32> adjViewport{
      static_cast<int>(0.5f + width * viewport.pos.x),
      static_cast<int>(0.5f + height * viewport.pos.y),
      static_cast<int>(0.5f + width * viewport.size.width),
      static_cast<int>(0.5f + height * viewport.size.height)};

  const ca::Vec4 normalized{
      -1.f + 2.f * (mousePos.x - adjViewport.pos.x) / adjViewport.size.width,
      1.f - 2.f * (mousePos.y - adjViewport.pos.y) / adjViewport.size.height,
      0.f, 1.f};

  // Then transform by the inverse of the view matrix
  ca::Mat4 inv = ca::inverse(m_view);

  ca::Vec4 result = ca::inverse(m_view) * normalized;

  result.w = 1.0f / result.w;
  result.x /= result.w;
  result.y /= result.w;
  result.w /= result.w;

  return ca::Vec2{result.x, result.y};
}

ca::Pos<i32> Camera::universePosToMousePos(const ca::Vec2& universePos) const {
  const float width = static_cast<float>(m_viewportSize.width);
  const float height = static_cast<float>(m_viewportSize.height);

  // Transform the point by the view matrix.
  const ca::Vec4 normalized =
      m_view * ca::Vec4{universePos.x, universePos.y, 0.f, 0.f};

  // Convert the point to viewport coordinates.
  const ca::Rect<f32> viewport{0.f, 0.f, 1.f, 1.f};
  const ca::Rect<i32> adjViewport{
      static_cast<i32>(width * viewport.pos.x),
      static_cast<i32>(height * viewport.pos.y),
      static_cast<i32>(width * viewport.size.width),
      static_cast<i32>(height * viewport.size.height)};

  const ca::Pos<i32> result{
      static_cast<i32>((normalized.x + 1.f) / 2.f * adjViewport.size.width +
                       adjViewport.pos.x),
      static_cast<i32>((-normalized.y + 1.f) / 2.f * adjViewport.size.height +
                       adjViewport.pos.y)};

  return result;
}

void Camera::adjustPosition(const ca::Pos<i32>& delta) {
  m_cameraTarget -=
      ca::Vec2{static_cast<float>(delta.x), static_cast<float>(delta.y)} *
      m_zoomLevel;
}

void Camera::adjustZoom(i32 delta) {
  m_targetZoomLevel -= static_cast<float>(delta) / 2.5f;

  if (m_targetZoomLevel < 1.f) {
    m_targetZoomLevel = 1.f;
  }

  if (m_targetZoomLevel > 10.f) {
    m_targetZoomLevel = 10.f;
  }
}

void Camera::tick(f32 adjustment) {
  // Adjust the current camera position towards the camera target position.
  m_cameraPos.x =
      m_cameraPos.x + (m_cameraTarget.x - m_cameraPos.x) / 10.f * adjustment;
  m_cameraPos.y =
      m_cameraPos.y + (m_cameraTarget.y - m_cameraPos.y) / 10.f * adjustment;

  // Adjust the zoom level towards the target zoom level.
  m_zoomLevel =
      m_zoomLevel + (m_targetZoomLevel - m_zoomLevel) / 7.5f * adjustment;

  // If we changed the zoom level of the camera position, we have to update the
  // view.
  updateView();
}

void Camera::render(ca::Canvas* canvas) const {
#if SHOW_CAMERA_TARGET
  m_cameraTargetSprite.render(
      canvas, m_view * ca::translate(m_cameraTarget.x, m_cameraTarget.y, 0.f));
#endif
}

void Camera::updateView() {
  const f32 ratio = 1080.f / m_viewportSize.height;

  const f32 halfWidth = static_cast<f32>(m_viewportSize.width) * ratio / 2.0f;
  const f32 halfHeight = static_cast<f32>(m_viewportSize.height) * ratio / 2.0f;

  // We use an orthographic projection to render the universe.
  m_view = ca::ortho(-halfWidth, halfWidth, halfHeight, -halfHeight);

  // Scale by the zoom factor.
  m_view *= ca::scale(1.f / m_zoomLevel, 1.f / m_zoomLevel, 1.f);

  // Move the camera to it's position.
  m_view *= ca::translate(-m_cameraPos.x, -m_cameraPos.y, 0.f);
}

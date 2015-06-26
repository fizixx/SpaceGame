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

#include <SFML/Graphics/RenderTarget.hpp>

Camera::Camera() {
#if SHOW_CAMERA_TARGET
  // Adjust some values on the camera target shape.
  m_cameraTargetShape.setRadius(10.f);
  sf::FloatRect bounds{m_cameraTargetShape.getGlobalBounds()};
  m_cameraTargetShape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  m_cameraTargetShape.setFillColor(sf::Color(255, 0, 0));
#endif  // SHOW_CAMERA_TARGET
}

Camera::~Camera() {
}

sf::Vector2f Camera::mousePosToUniversePos(const sf::Vector2i& mousePos) const {

  const float width = static_cast<float>(m_viewportSize.x);
  const float height = static_cast<float>(m_viewportSize.y);
  const sf::FloatRect& viewport = m_view.getViewport();

  const sf::IntRect adjViewport{
      static_cast<int>(0.5f + width * viewport.left),
      static_cast<int>(0.5f + height * viewport.top),
      static_cast<int>(0.5f + width * viewport.width),
      static_cast<int>(0.5f + height * viewport.height)};

  const sf::Vector2f normalized{
      -1.f + 2.f * (mousePos.x - adjViewport.left) / adjViewport.width,
      1.f - 2.f * (mousePos.y - adjViewport.top) / adjViewport.height};

  // Then transform by the inverse of the view matrix
  return m_view.getInverseTransform().transformPoint(normalized);
}

sf::Vector2i Camera::universePosToMousePos(
    const sf::Vector2f& universePos) const {
  const float width = static_cast<float>(m_viewportSize.x);
  const float height = static_cast<float>(m_viewportSize.y);

  // Transform the point by the vie matrix.
  const sf::Vector2f normalized =
      m_view.getTransform().transformPoint(universePos);

  // Convert the point to viewport coordinates.
  const sf::FloatRect& viewport = m_view.getViewport();
  const sf::IntRect adjViewport{static_cast<int>(width * viewport.left),
                                static_cast<int>(height * viewport.top),
                                static_cast<int>(width * viewport.width),
                                static_cast<int>(height * viewport.height)};

  const sf::Vector2i result{
      static_cast<int>((normalized.x + 1.f) / 2.f * adjViewport.width +
                       adjViewport.left),
      static_cast<int>((-normalized.y + 1.f) / 2.f * adjViewport.height +
                       adjViewport.top)};

  return result;
}

void Camera::adjustPosition(const sf::Vector2i& delta) {
  m_cameraTarget -=
      sf::Vector2f{static_cast<float>(delta.x), static_cast<float>(delta.y)} *
      m_zoomLevel;
}

void Camera::adjustZoom(int32_t delta) {
  m_targetZoomLevel -= static_cast<float>(delta) / 2.5f;

  if (m_targetZoomLevel < 1.f) {
    m_targetZoomLevel = 1.f;
  }

  if (m_targetZoomLevel > 10.f) {
    m_targetZoomLevel = 10.f;
  }
}

void Camera::layout(const sf::IntRect& rect) {
  m_viewportSize.x = static_cast<float>(rect.width);
  m_viewportSize.y = static_cast<float>(rect.height);

  updateView();
}

void Camera::tick(float adjustment) {
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

void Camera::draw(sf::RenderTarget& target, sf::RenderStates states) const {
#if SHOW_CAMERA_TARGET
  target.draw(m_cameraTargetShape);
#endif
}

void Camera::updateView() {
  // Adjust the viewport size.
  float ratio = 1080.f / m_viewportSize.y;

  sf::Vector2f size{m_viewportSize.x * ratio, m_viewportSize.y * ratio};

  sf::View result{m_cameraPos, size};
  result.zoom(m_zoomLevel);

  m_view = result;
}

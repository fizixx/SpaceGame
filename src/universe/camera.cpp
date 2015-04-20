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

#if BUILD(DEBUG)
#define SHOW_CAMERA_TARGET 0
#endif

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
  float width = static_cast<float>(m_viewportSize.x);
  float height = static_cast<float>(m_viewportSize.y);
  const sf::FloatRect& viewport = m_view.getViewport();

  sf::IntRect adjViewport{static_cast<int>(0.5f + width * viewport.left),
                          static_cast<int>(0.5f + height * viewport.top),
                          static_cast<int>(0.5f + width * viewport.width),
                          static_cast<int>(0.5f + height * viewport.height)};

  sf::Vector2f normalized;
  normalized.x =
      -1.f + 2.f * (mousePos.x - adjViewport.left) / adjViewport.width;
  normalized.y =
      1.f - 2.f * (mousePos.y - adjViewport.top) / adjViewport.height;

  // Then transform by the inverse of the view matrix
  return m_view.getInverseTransform().transformPoint(normalized);
}

void Camera::onMousePressed(sf::Event& event) {
  switch (event.mouseButton.button) {
    case sf::Mouse::Right:
      m_isDraggingView = true;
      m_startDragViewPos =
          sf::Vector2f{static_cast<float>(event.mouseButton.x),
                       static_cast<float>(event.mouseButton.y)};
      break;
  }
}

void Camera::onMouseDragged(sf::Event& event) {
  // Adjust the camera if we are dragging the view around.
  if (m_isDraggingView) {
    // Get the amount that the mouse moved from the last position.
    sf::Vector2f delta = sf::Vector2f(static_cast<float>(event.mouseMove.x),
                                      static_cast<float>(event.mouseMove.y)) -
                         m_startDragViewPos;

    // Update the position of the camera.
    m_cameraTarget -= delta * m_zoomLevel;
#if SHOW_CAMERA_TARGET
    m_cameraTargetShape.setPosition(m_cameraTarget);
#endif

    // Set the last view position to the current position for the next drag
    // event.
    m_startDragViewPos = sf::Vector2f(static_cast<float>(event.mouseMove.x),
                                      static_cast<float>(event.mouseMove.y));
  }
}

void Camera::onMouseReleased(sf::Event& event) {
  switch (event.mouseButton.button) {
    case sf::Mouse::Right:
      m_isDraggingView = false;
      break;
  }
}

void Camera::onMouseWheel(sf::Event& event) {
  m_targetZoomLevel -= static_cast<float>(event.mouseWheel.delta);
  if (m_targetZoomLevel < 1.f)
    m_targetZoomLevel = 1.f;
  if (m_targetZoomLevel > 5.f)
    m_targetZoomLevel = 5.f;

  // We also move the camera target to where we scrolled the mouse wheel.
  // NOTE: We only do it half way between the current camera target and the
  // mouse position so that movements aren't so sudden.
  sf::Vector2f uniPos{mousePosToUniversePos(
      sf::Vector2i{event.mouseWheel.x, event.mouseWheel.y})};

  m_cameraTarget = uniPos;

#if SHOW_CAMERA_TARGET
  m_cameraTargetShape.setPosition(m_cameraTarget);
#endif
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

void Camera::layout(const sf::IntRect& rect) {
  m_viewportSize.x = static_cast<float>(rect.width);
  m_viewportSize.y = static_cast<float>(rect.height);

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

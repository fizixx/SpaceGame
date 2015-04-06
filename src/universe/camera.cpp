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

#include "camera.h"

#include <base/logging.h>
#include <SFML/Graphics/RenderTarget.hpp>

Camera::Camera(const sf::Vector2f& viewportSize) : m_viewportSize(viewportSize) {
  // Adjust some values on the camera target shape.
  sf::FloatRect bounds{m_cameraTargetShape.getGlobalBounds()};
  m_cameraTargetShape.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
  m_cameraTargetShape.setFillColor(sf::Color(255, 0, 0));
}

Camera::~Camera() {
}

sf::View Camera::calculateCameraView() const {
  // Adjust the viewport size.
  float ratio = 1080.f / m_viewportSize.y;

  sf::Vector2f size{m_viewportSize.x * ratio, m_viewportSize.y * ratio};

  sf::View result{m_cameraPos, size};
  result.zoom(m_zoomLevel);

  return result;
}

void Camera::handleInput(sf::Event& event) {
  switch (event.type) {
    case sf::Event::MouseButtonPressed:
      switch (event.mouseButton.button) {
        case sf::Mouse::Right:
          m_isDraggingView = true;
          m_startDragViewPos =
              sf::Vector2f{static_cast<float>(event.mouseButton.x),
                           static_cast<float>(event.mouseButton.y)};
          break;
      }
      break;

    case sf::Event::MouseButtonReleased:
      switch (event.mouseButton.button) {
        case sf::Mouse::Right:
          m_isDraggingView = false;
          break;
      }
      break;

    case sf::Event::MouseMoved:
      // Adjust the camera if we are dragging the view around.
      if (m_isDraggingView) {
        // Get the amount that the mouse moved from the last position.
        sf::Vector2f delta =
            sf::Vector2f(static_cast<float>(event.mouseMove.x),
                         static_cast<float>(event.mouseMove.y)) -
            m_startDragViewPos;

        // Update the position of the camera.
        m_cameraTarget += delta * m_zoomLevel;
        m_cameraTargetShape.setPosition(m_cameraTarget);

        // Set the last view position to the current position for the next drag
        // event.
        m_startDragViewPos =
            sf::Vector2f(static_cast<float>(event.mouseMove.x),
                         static_cast<float>(event.mouseMove.y));
      }
      break;

    case sf::Event::MouseWheelMoved:
      m_targetZoomLevel -= static_cast<float>(event.mouseWheel.delta);
      if (m_targetZoomLevel < 1.f)
        m_targetZoomLevel = 1.f;
      if (m_targetZoomLevel > 10.f)
        m_targetZoomLevel = 10.f;

      // TODO(tiaan.louw): Adjuat the mouse position to the view's position.
#if 0
      // We also move the camera target to where we scrolled the mouse wheel.
      m_cameraTarget = sf::Vector2f{static_cast<float>(event.mouseWheel.x),
                                    static_cast<float>(event.mouseWheel.y)};
      m_cameraTargetShape.setPosition(m_cameraTarget);
#endif  // 0

      break;
  }
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
}

void Camera::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(m_cameraTargetShape);
}

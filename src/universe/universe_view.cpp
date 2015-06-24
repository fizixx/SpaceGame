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

#include "universe/universe_view.h"

#include "canvas/math/transform.h"
#include "elastic/context.h"

#include "universe/link.h"
#include "universe/objects/object.h"
#include "universe/objects/structures/miner.h"
#include "universe/objects/structures/turret.h"
#include "universe/objects/units/enemy_ship.h"
#include "universe/universe.h"

UniverseView::UniverseView(el::Context* context, Universe* universe)
  : el::View(context), m_universe(universe), m_hud{this} {
// Set up the mouse position shape.

#if SHOW_UNIVERSE_MOUSE_POS
  m_mousePosSprite.setTexture(context->getTexture("mouse_cursor"));
#endif
}

UniverseView::~UniverseView() {
}

void UniverseView::startPlacingObject(std::unique_ptr<Object> object) {
  m_ghostObject = std::move(object);
}

void UniverseView::stopPlacingObject(bool place) {
  if (place) {
    m_universe->addObject(std::move(m_ghostObject));
  } else {
    m_ghostObject.reset();
  }
}

bool UniverseView::onMousePressed(const ca::MouseEvent& event) {
  el::View::onMousePressed(event);

#if 0
  // We only handle left mouse presses for now.
  if (event.mouseButton.button != sf::Mouse::Left) {
    return false;
  }
#endif  // 0

  // Store the position where we pressed the mouse the first time.
  m_mouseStartDragPos = event.pos;

  // Convert the mouse position to a universe position.
  ca::Vec2 universePos{m_camera.mousePosToUniversePos(m_mouseStartDragPos)};

  // See if the mouse is currently over an object.
  m_mousePressedObject = m_universe->findObjectAt(universePos);
  if (m_mousePressedObject) {
    m_mouseHandler = MouseHandler::Object;
    return true;
  }

  // If we didn't press on an object, then we pressed on the background which
  // means we're not controlling the camera.
  m_mouseHandler = MouseHandler::Camera;

  return true;
}

bool UniverseView::onMouseDragged(const ca::MouseEvent& event) {
  el::View::onMouseDragged(event);

  // Update the last mouse position.
  m_viewMousePos = event.pos;

  // Get the universe position.
  // m_universeMousePos = m_camera.mousePosToUniversePos(m_viewMousePos);

  // If we are controlling the camera, then adjust the camera when we dragged
  // somewhere.
  if (m_mouseHandler == MouseHandler::Camera) {
    // Get the delta position.
    ca::Pos<i32> currentMousePos = event.pos;
    ca::Pos<i32> delta = currentMousePos - m_mouseStartDragPos;

    // If we haven't moved past the threshold yet, check to see if we haven't
    // this time.
    if (!m_cameraMovedPastThreshold) {
      if (std::abs(delta.x) > 5 || std::abs(delta.y) > 5) {
        m_cameraMovedPastThreshold = true;
      }
    } else {
      // We have moved past the threshold, so we can update the camera position.

      // Adjust the camera.
      m_camera.adjustPosition(delta);

      // Reset the starting mouse drag position.
      m_mouseStartDragPos = currentMousePos;
    }
  }

#if 0
  // When the mouse is dragged, we update any possible ghost objects.
  updateGhostPosition(universeMousePos);
#endif  // 0

  return true;
}

void UniverseView::onMouseMoved(const ca::MouseEvent& event) {
  el::View::onMouseMoved(event);

  // Update the last mouse position.
  m_viewMousePos = event.pos;

  // Get the universe position.
  m_universeMousePos = m_camera.mousePosToUniversePos(m_viewMousePos);

  ca::Vec2 pos = m_camera.mousePosToUniversePos(ca::Pos<i32>{700, 450});
  LOG(Info) << pos.x << " : " << pos.y;

  // The hud wants to know if we moved the mouse.
  m_hud.updateUniverseMousePos(m_universeMousePos);

  // Update any ghost objects that we might have.
  updateGhostPosition(m_universeMousePos);
}

void UniverseView::onMouseReleased(const ca::MouseEvent& event) {
  el::View::onMouseReleased(event);

#if 0
  // If we pressed the mouse on an object, then we check to see if we are still
  // over the same object, and if it is, select that object.
  if (m_mouseHandler == MouseHandler::Object) {
    // Get the universe position.
    sf::Vector2f universePos{m_camera.mousePosToUniversePos(
        sf::Vector2i{event.mouseButton.x, event.mouseButton.y})};

    if (m_mousePressedObject == m_universe->findObjectAt(universePos)) {
      m_hud.setSelectedObject(m_mousePressedObject);
      m_mousePressedObject = nullptr;
    }
  } else if (m_mouseHandler == MouseHandler::Camera) {
    // If we released the mouse, but we didn't drag past the camera move
    // threshold, then it counts as a cancel selection or placing the ghost
    // object if we have one.
    if (!m_cameraMovedPastThreshold) {
      if (m_ghostObject) {
        // If we have a ghost object at this point, then rather than cancelling
        // the selection, we place the ghost object.
        stopPlacingObject(true);
      } else {
        m_hud.setSelectedObject(nullptr);
      }
    }
    m_cameraMovedPastThreshold = false;
  }

  m_mouseHandler = MouseHandler::None;
#endif  // 0
}

void UniverseView::onMouseWheel(const ca::MouseEvent& event) {
  el::View::onMouseWheel(event);

#if 0
  // Adjust the camera zoom level.
  m_camera.adjustZoom(event.mouseWheel.delta);
#endif  // 0
}

#if 0
void UniverseView::onKeyPressed(sf::Event& event) {
}

void UniverseView::onKeyReleased(sf::Event& event) {
  if (event.key.code == sf::Keyboard::E) {
    placeEnemyShip(m_camera.mousePosToUniversePos(m_viewMousePos));
  } else if (event.key.code == sf::Keyboard::T) {
    startPlacingObject(std::make_unique<Turret>(
        m_universe, m_camera.mousePosToUniversePos(m_viewMousePos)));
  } else if (event.key.code == sf::Keyboard::M) {
    startPlacingObject(std::make_unique<Miner>(
      m_universe, m_camera.mousePosToUniversePos(m_viewMousePos)));
  }
}
#endif  // 0

void UniverseView::tick(float adjustment) {
  m_camera.tick(adjustment);
  m_hud.tick(adjustment);

// Update the location of the mouse within the universe.
#if SHOW_UNIVERSE_MOUSE_POS
  // m_universeMousePos = m_camera.mousePosToUniversePos(m_viewMousePos);
#endif
}

void UniverseView::layout(const ca::Rect<i32>& rect) {
  View::layout(rect);

  m_camera.setViewportDimensions(rect);
}

void UniverseView::render(ca::Canvas* canvas, const ca::Mat4& transform) const {
#if 0
  // Store the original view state.
  sf::View origView = target.getView();

  // Set the new view to our camera view.
  target.setView(m_camera.getView());

  // Render all the links in the universe.
  for (const auto& link : m_universe->m_links) {
    target.draw(*link, states);
  }
#endif  // 0

  ca::Mat4 cameraView = m_camera.getView();

  // Render the objects.
  for (const auto& object : m_universe->m_objects) {
    object->render(canvas, cameraView);
  }

#if 0
  // Render the ghost object and link over the existing objects.
  if (m_ghostObject) {
    target.draw(*m_ghostObject);
  }

  // Render all the debugging stuff.

  // Render the camera target.
  target.draw(m_camera);
#endif  // 0

#if SHOW_UNIVERSE_MOUSE_POS
  // Draw the mouse position.
  m_mousePosSprite.render(
      canvas, cameraView * ca::translate(m_universeMousePos.x,
                                         m_universeMousePos.y, 0.f));
#endif

#if 0
  // Reset the target view.
  target.setView(origView);

  // Render the hud after we reset the view.
  target.draw(m_hud, states);
#endif  // 0
}

void UniverseView::updateGhostPosition(const ca::Vec2& universeMousePos) {
  // Move the ghost object to the new mouse position.
  if (m_ghostObject) {
    m_ghostObject->moveTo(universeMousePos);
  }
}

void UniverseView::placeEnemyShip(const ca::Vec2& pos) {
  m_universe->addObject(std::make_unique<EnemyShip>(m_universe, pos));
}

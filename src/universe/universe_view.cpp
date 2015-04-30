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

#include "universe/link.h"
#include "universe/objects/object.h"
#include "universe/objects/projectiles/bullet.h"
#include "universe/objects/units/enemy_ship.h"
#include "universe/universe.h"

UniverseView::UniverseView(el::Context* context, Universe* universe)
  : el::View(context), m_universe(universe), m_hud{this} {
// Set up the mouse position shape.

#if SHOW_UNIVERSE_MOUSE_POS
  m_mousePosShape.setRadius(25.f);
  m_mousePosShape.setOrigin(m_mousePosShape.getGlobalBounds().width / 2.f,
                            m_mousePosShape.getGlobalBounds().height / 2.f);
  m_mousePosShape.setFillColor(sf::Color{0, 0, 255, 255});
#endif  // SHOW_UNIVERSE_MOUSE_POS
}

UniverseView::~UniverseView() {
}

void UniverseView::startPlacingObject(std::unique_ptr<Object> object) {
  m_ghostObject = std::move(object);

  // Create the ghost link between the closest link object and the ghost object.
  Object* closestLinkObject =
      m_universe->getClosestLinkObject(m_ghostObject->getPos());
  m_ghostLink = std::make_unique<Link>(m_universe, closestLinkObject,
                                       m_ghostObject.get());
}

void UniverseView::stopPlacingObject(bool place) {
  if (place) {
    m_universe->addObject(std::move(m_ghostObject));
    m_universe->addLink(m_ghostLink->getSource(),
                        m_ghostLink->getDestination());
  } else {
    m_ghostObject.reset();
  }
  m_ghostLink.reset();
}

bool UniverseView::onMousePressed(sf::Event& event) {
  el::View::onMousePressed(event);

  // We only handle left mouse presses for now.
  if (event.mouseButton.button != sf::Mouse::Left) {
    return false;
  }

  // Store the position where we pressed the mouse the first time.
  m_mouseStartDragPos.x = event.mouseButton.x;
  m_mouseStartDragPos.y = event.mouseButton.y;

  // Convert the mouse position to a universe position.
  sf::Vector2f universePos{m_camera.mousePosToUniversePos(m_mouseStartDragPos)};

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

bool UniverseView::onMouseDragged(sf::Event& event) {
  el::View::onMouseDragged(event);

  // Update the last mouse position.
  m_viewMousePos = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};

  // Get the universe position.
  sf::Vector2f universeMousePos{m_camera.mousePosToUniversePos(m_viewMousePos)};

#if SHOW_UNIVERSE_MOUSE_POS
  m_mousePosShape.setPosition(universeMousePos);
#endif

  // If we are controlling the camera, then adjust the camera when we dragged
  // somewhere.
  if (m_mouseHandler == MouseHandler::Camera) {
    // Get the delta position.
    sf::Vector2i currentMousePos{event.mouseMove.x, event.mouseMove.y};
    sf::Vector2i delta = currentMousePos - m_mouseStartDragPos;

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

  // When the mouse is dragged, we update any possible ghost objects.
  updateGhostPosition(universeMousePos);

  return true;
}

void UniverseView::onMouseMoved(sf::Event& event) {
  el::View::onMouseMoved(event);

  // Update the last mouse position.
  m_viewMousePos = sf::Vector2i{event.mouseMove.x, event.mouseMove.y};

  // Get the universe position.
  sf::Vector2f universeMousePos{m_camera.mousePosToUniversePos(m_viewMousePos)};

  // The hud wants to know if we moved the mouse.
  m_hud.updateUniverseMousePos(universeMousePos);

#if SHOW_UNIVERSE_MOUSE_POS
  m_mousePosShape.setPosition(universeMousePos);
#endif

  // Update any ghost objects that we might have.
  updateGhostPosition(universeMousePos);
}

void UniverseView::onMouseReleased(sf::Event& event) {
  el::View::onMouseReleased(event);

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
}

void UniverseView::onMouseWheel(sf::Event& event) {
  el::View::onMouseWheel(event);

  // Adjust the camera zoom level.
  m_camera.adjustZoom(event.mouseWheel.delta);
}

void UniverseView::onKeyPressed(sf::Event& event) {
}

void UniverseView::onKeyReleased(sf::Event& event) {
  if (event.key.code == sf::Keyboard::E) {
    placeEnemyShip(m_camera.mousePosToUniversePos(m_viewMousePos));
  } else if (event.key.code == sf::Keyboard::B) {
    placeBullet(m_camera.mousePosToUniversePos(m_viewMousePos), 0.f, 1.f);
  }
}

void UniverseView::tick(float adjustment) {
  m_camera.tick(adjustment);
  m_hud.tick(adjustment);

// Update the location of the mouse within the universe.
#if SHOW_UNIVERSE_MOUSE_POS
  sf::Vector2f universeMousePos{m_camera.mousePosToUniversePos(m_viewMousePos)};
  m_mousePosShape.setPosition(universeMousePos);
#endif
}

void UniverseView::layout(const sf::IntRect& rect) {
  View::layout(rect);

  m_camera.layout(rect);
}

void UniverseView::draw(sf::RenderTarget& target,
                        sf::RenderStates states) const {
  // Store the original view state.
  sf::View origView = target.getView();

  // Set the new view to our camera view.
  target.setView(m_camera.getView());

  // Render the links first.
  for (const auto& link : m_universe->m_links) {
    target.draw(*link);
  }

  // Render the ghost link with the rest of the links.
  if (m_ghostLink) {
    target.draw(*m_ghostLink);
  }

  // Render the objects.
  for (const auto& object : m_universe->m_objects) {
    if (!object) {
      continue;
    }
    target.draw(*object);
  }

  // Render the ghost object and link over the existing objects.
  if (m_ghostObject) {
    target.draw(*m_ghostObject);
  }

  // Render all the debugging stuff.

  // Render the camera target.
  target.draw(m_camera);

#if SHOW_UNIVERSE_MOUSE_POS
  // Draw the mouse position.
  target.draw(m_mousePosShape);
#endif

  // Reset the target view.
  target.setView(origView);

  // Render the hud after we reset the view.
  target.draw(m_hud, states);
}

void UniverseView::updateGhostPosition(const sf::Vector2f& universeMousePos) {
  // Move the ghost object to the new mouse position.
  if (m_ghostObject) {
    m_ghostObject->moveTo(universeMousePos);
  }

  // Find a new link source and update the link.
  if (m_ghostLink) {
    Object* closestObject = m_universe->getClosestLinkObject(universeMousePos);
    m_ghostLink->setSource(closestObject);
  }
}

void UniverseView::placeEnemyShip(const sf::Vector2f& pos) {
  m_universe->createObject<EnemyShip>()->moveTo(pos);
}

void UniverseView::placeBullet(const sf::Vector2f& pos, float direction, float speed) {
  m_universe->addObject(
      std::move(std::make_unique<Bullet>(m_universe, pos, direction, speed)));
}

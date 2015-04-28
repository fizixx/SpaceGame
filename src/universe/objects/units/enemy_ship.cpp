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

#include "universe/objects/units/enemy_ship.h"

#include <nucleus/logging.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"
#include "utils/math.h"
#include "utils/stream_operators.h"

EnemyShip::EnemyShip(Universe* universe)
  : Unit(universe, ObjectType::EnemyShip) {
  m_shape.setPrimitiveType(sf::Triangles);
  m_shape.append(sf::Vertex{sf::Vector2f{0.f, -25.f}});
  m_shape.append(sf::Vertex{sf::Vector2f{75.f, 0.f}});
  m_shape.append(sf::Vertex{sf::Vector2f{0.f, 25.f}});
}

EnemyShip::~EnemyShip() {
}

sf::FloatRect EnemyShip::getBounds() const {
  return m_shape.getBounds();
}

void EnemyShip::tick(float adjustment) {
  // If we're doing nothing, then decide where we want to go and then go there.
  if (m_task == Task::Nothing) {
    m_task = Task::Travel;

    // Find the nearest command center and travel there.
    Object* closestObject =
        m_universe->findClosestObjectOfType(m_pos, ObjectType::CommandCenter);
    if (closestObject) {
      m_travelTargetPos = closestObject->getPos();
    }
  }

  // If we are traveling, update the direction and speed we are traveling in.
  if (m_task == Task::Travel) {
    // Calculate the direction to the target.
    float directionToTarget = directionBetween(m_pos, m_travelTargetPos);

    // LOG(Info) << "directionToTarget: " << directionToTarget;
    // LOG(Info) << "m_direction: " << m_direction;

    // If we are not pointing directly towards the target, then we must turn.
    if (m_direction != directionToTarget) {
      // Calculate the direction we have to turn.
      float turnSide = directionToTarget - m_direction;

      if (turnSide < 0.f) {
        turnSide = std::max(-2.5f, turnSide);
      } else {
        turnSide = std::min(2.5f, turnSide);
      }

      m_direction = wrap(m_direction + turnSide, 0.f, 360.f);
    }

    m_speed = 5.f;

    // If we have speed, update out position.
    if (m_speed > 0.f) {
      m_pos = sf::Vector2f{
          m_pos.x + std::cos(degToRad(m_direction)) * m_speed,
          m_pos.y + std::sin(degToRad(m_direction)) * m_speed};
    }
  }
}

void EnemyShip::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  states.transform.translate(m_pos);
  states.transform.rotate(m_direction);
  target.draw(m_shape, states);
}

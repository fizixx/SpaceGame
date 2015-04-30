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

#include <sstream>

#include <nucleus/logging.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/universe.h"
#include "utils/math.h"
#include "utils/stream_operators.h"

namespace {

const float kMaxTurnRadius = 1.f;
const float kMaxSpeed = 1.f;
const float kMaxEngagementRange = 750.f;

}  // namespace

EnemyShip::EnemyShip(Universe* universe)
  : Unit(universe, ObjectType::EnemyShip) {
  // Set up the shape of the ship.
  m_shape.setPrimitiveType(sf::Triangles);
  m_shape.append(sf::Vertex{sf::Vector2f{0.f, -25.f}});
  m_shape.append(sf::Vertex{sf::Vector2f{75.f, 0.f}});
  m_shape.append(sf::Vertex{sf::Vector2f{0.f, 25.f}});

#if BUILD(DEBUG)
  // Set up the info text.
  sf::Font* font =
      universe->getResourceManager()->getFont(ResourceManager::Font::Default);
  if (font) {
    m_infoText.setFont(*font);
    m_infoText.setColor(sf::Color{0, 255, 0, 255});
    m_infoText.setCharacterSize(20);
    m_infoText.setString("Nothing to say");
  }
#endif  // BUILD(DEBUG)

  createEngagementRangeShape();
}

EnemyShip::~EnemyShip() {
}

sf::FloatRect EnemyShip::getBounds() const {
  return m_shape.getBounds();
}

void EnemyShip::tick(float adjustment) {
  stepper++;
  if (stepper % 10 != 0) {
    return;
  }

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

    // If we are not pointing directly towards the target, then we must turn.
    if (m_direction != directionToTarget) {
      // Calculate the direction we have to turn.
      float turnSide = directionToTarget - m_direction;

      if (std::abs(turnSide) < kMaxTurnRadius) {
        m_direction = directionToTarget;
      } else {
        turnSide = (turnSide < 0.f) ? -kMaxTurnRadius : kMaxTurnRadius;
        m_direction = wrap(m_direction + turnSide, 0.f, 360.f);
      }
    }

    m_speed = kMaxSpeed;

    // If we have speed, update our position.
    m_pos = sf::Vector2f{m_pos.x + std::cos(degToRad(m_direction)) * m_speed,
                         m_pos.y + std::sin(degToRad(m_direction)) * m_speed};

    // If we are heading directly towards the target and the target comes into
    // range, then we start our attack run.  We check if the difference is
    // within 1.f, to eliminate rounding errors.
    if (std::abs(m_direction - directionToTarget) < 1.f) {
      float distanceToTarget = distanceBetween(m_pos, m_travelTargetPos);
      if (distanceToTarget < kMaxEngagementRange) {
        m_task = Task::Attacking;
      }
    }
  }

  if (m_task == Task::Attacking) {
    // We don't move any more, but we only travel forward.

    // If we have speed, update out position.
    if (m_speed > 0.f) {
      m_pos = sf::Vector2f{m_pos.x + std::cos(degToRad(m_direction)) * m_speed,
                           m_pos.y + std::sin(degToRad(m_direction)) * m_speed};
    }
  }

#if BUILD(DEBUG)
  {
    std::ostringstream ss;

    switch (m_task) {
    case Task::Nothing:
      ss << "Nothing";
      break;

    case Task::Travel:
      ss << "Travel";
      break;

    case Task::Attacking:
      ss << "Attacking";
      break;

    default:
      ss << "Unknown";
      break;
    }

    ss << '\n' << m_direction << " ("
       << directionBetween(m_pos, m_travelTargetPos) << ")\n"
       << distanceBetween(m_pos, m_travelTargetPos);

    m_infoText.setString(ss.str());
    m_infoText.setPosition(m_pos);
    sf::FloatRect bounds{m_infoText.getLocalBounds()};
    m_infoText.setOrigin(sf::Vector2f{bounds.width / 2.f, bounds.height / 2.f});
  }
#endif
}

void EnemyShip::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  sf::RenderStates originalStates{states};

  states.transform.translate(m_pos);
  states.transform.rotate(m_direction);
  target.draw(m_engagementRangeShape, states);
  target.draw(m_shape, states);

#if BUILD(DEBUG)
  target.draw(m_infoText, originalStates);
#endif
}

void EnemyShip::createEngagementRangeShape() {
  const float kSpread = 45.f;
  const int kSteps = 9;
  static const sf::Color color{255, 0, 0, 127};

  m_engagementRangeShape.setPrimitiveType(sf::TrianglesFan);
  m_engagementRangeShape.resize(2 + kSteps);

  m_engagementRangeShape[0].position = sf::Vector2f(75.f, 0.f);
  m_engagementRangeShape[0].color = color;

  size_t i = 1;
  const float spreadStep = kSpread / static_cast<float>(kSteps);
  const float half = kSpread / -2.f;
  for (float degrees = 0.f; degrees <= kSpread; degrees += spreadStep, ++i) {
    m_engagementRangeShape[i].position.x =
        std::cos(degToRad(half + degrees)) * kMaxEngagementRange;
    m_engagementRangeShape[i].position.y =
        std::sin(degToRad(half + degrees)) * kMaxEngagementRange;
    m_engagementRangeShape[i].color = color;
  }
}

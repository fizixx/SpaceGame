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

#include "particles/particle.h"

#include <cmath>

Particle::Particle(ParticleEmitter* emitter, const ca::Vec2& pos)
  : m_emitter(emitter), m_pos(pos) {
  // Set up the particle shape.
#if 0
  m_shape.setRadius(15.f);
  m_shape.setFillColor(sf::Color{255, 255, 255});
  sf::FloatRect bounds = m_shape.getLocalBounds();
  m_shape.setOrigin(sf::Vector2f{bounds.width / 2.f, bounds.height / 2.f});
  m_shape.setPosition(m_pos);
#endif  // 0
}

Particle::~Particle() {
}

void Particle::setPos(const ca::Vec2& pos) {
  m_pos = pos;
#if 0
  m_shape.setPosition(pos);
#endif  // 0
}

void Particle::tick(f32 adjustment) {
#if 0
  // Adjust the color of the particle.
  sf::Uint8 alpha =
      static_cast<sf::Uint8>(std::round(255.f * m_life / m_lifeTime));
  m_shape.setFillColor(sf::Color{255, 255, 255, alpha});

  m_shape.setRadius(15.f * m_life / m_lifeTime);
  sf::FloatRect bounds = m_shape.getLocalBounds();
  m_shape.setOrigin(sf::Vector2f{bounds.width / 2.f, bounds.height / 2.f});

  // Adjust the life of the particle.
  m_life -= m_lifeTime / 20.f;
#endif  // 0
}

void Particle::render(ca::Canvas* canvas) const {
#if 0
  target.draw(m_shape);
#endif  // 0
}

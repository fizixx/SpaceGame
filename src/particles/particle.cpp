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

#include <SFML/Graphics/RenderTarget.hpp>

Particle::Particle(ParticleEmitter* emitter, const sf::Vector2f& pos)
  : m_emitter(emitter), m_pos(pos) {
  // Set up the particle shape.
  m_shape.setRadius(15.f);
  m_shape.setFillColor(sf::Color{255, 255, 255});
  sf::FloatRect bounds = m_shape.getLocalBounds();
  m_shape.setOrigin(sf::Vector2f{bounds.width / 2.f, bounds.height / 2.f});
  m_shape.setPosition(m_pos);
}

Particle::~Particle() {
}

void Particle::setPos(const sf::Vector2f& pos) {
  m_pos = pos;
  m_shape.setPosition(pos);
}

void Particle::tick(float adjustment) {
  // Adjust the color of the particle.
  sf::Uint8 alpha =
      static_cast<sf::Uint8>(std::round(255.f * m_life / m_lifeTime));
  m_shape.setFillColor(sf::Color{255, 255, 255, alpha});

  m_shape.setRadius(15.f * m_life / m_lifeTime);
  sf::FloatRect bounds = m_shape.getLocalBounds();
  m_shape.setOrigin(sf::Vector2f{bounds.width / 2.f, bounds.height / 2.f});

  // Adjust the life of the particle.
  m_life -= m_lifeTime / 20.f;
}

void Particle::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  target.draw(m_shape);
}

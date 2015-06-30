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

#ifndef PARTICLES_PARTICLE_EMITTER_H_
#define PARTICLES_PARTICLE_EMITTER_H_

#include <functional>
#include <vector>

#include "canvas/math/vec2.h"
#include "canvas/rendering/canvas.h"
#include "nucleus/macros.h"

class Particle;

class ParticleEmitter {
public:
  using ParticleFactory =
      std::function<Particle*(ParticleEmitter*, const ca::Vec2&)>;

  explicit ParticleEmitter(const ParticleFactory& factory);
  ~ParticleEmitter();

  // Get/set our position.
  const ca::Vec2& getPos() const { return m_pos; }
  void setPos(const ca::Vec2& pos);

  // Tick the emitter.
  void tick(f32 adjustment);

  // Override: sf::Drawable
  void render(ca::Canvas* canvas) const;

private:
  // Creates a new particle and add it to our list of particles.
  void createParticle(const ca::Vec2& pos);
  
  // The factory function we use to create particles.
  ParticleFactory m_factory;

  // Our position.
  ca::Vec2 m_pos;

  // The time since the last particle was emitted.
  f32 m_timeSinceLastParticle{0.f};

  // All the particles we are rendering.
  std::vector<Particle*> m_particles;

  DISALLOW_COPY_AND_ASSIGN(ParticleEmitter);
};

#endif  // PARTICLES_PARTICLE_EMITTER_H_

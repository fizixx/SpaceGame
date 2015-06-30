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

#include "universe/components/render_component.h"

#include "canvas/math/transform.h"

SpriteRenderComponent::SpriteRenderComponent(ResourceManager* resourceManager,
                                             ResourceManager::Texture texture) {
  m_texture = resourceManager->getTexture(texture);
  m_sprite.setTexture(m_texture);
}

ca::Rect<f32> SpriteRenderComponent::getBounds() const {
  return m_sprite.getBounds();
}

void SpriteRenderComponent::render(ca::Canvas* canvas,
                                   const ca::Mat4& transform,
                                   const ca::Vec2& pos) const {
  const ca::Mat4 local = transform * ca::translate(pos.x, pos.y, 0.f);
  m_sprite.render(canvas, local);
}

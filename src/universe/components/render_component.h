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

#ifndef UNIVERSE_COMPONENTS_RENDER_COMPONENT_H_
#define UNIVERSE_COMPONENTS_RENDER_COMPONENT_H_

#include "canvas/math/mat4.h"
#include "canvas/rendering/canvas.h"
#include "canvas/rendering/sprite.h"

#include "game/resource_manager.h"

class RenderComponent {
public:
  // Return the rendering bounds of this component.
  virtual ca::Rect<f32> getBounds() const = 0;

  // Render this component.
  virtual void render(ca::Canvas* canvas, const ca::Mat4& transform,
                      const ca::Vec2& pos) const = 0;
};

class SpriteRenderComponent : public RenderComponent {
public:
  SpriteRenderComponent(ResourceManager* resourceManager,
                        ResourceManager::Texture texture);

  // Override: RenderComponent
  ca::Rect<f32> getBounds() const override;
  void render(ca::Canvas* canvas, const ca::Mat4& transform,
              const ca::Vec2& pos) const override;

private:
  // The texture we render.
  ca::Texture* m_texture{nullptr};

  // The sprite we use to render the texture.
  ca::Sprite m_sprite;

  DISALLOW_IMPLICIT_CONSTRUCTORS(SpriteRenderComponent);
};

#endif  // UNIVERSE_COMPONENTS_RENDER_COMPONENT_H_

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

#include "universe/objects/structures/command_center.h"

#include "canvas/math/transform.h"

#include "universe/universe.h"

DEFINE_STRUCTURE(CommandCenter, "Command Center", 2000, 0);

CommandCenter::CommandCenter(Universe* universe, const ca::Vec2& pos)
  : Structure(universe, ObjectType::CommandCenter, pos, 5000) {
  m_renderComponent = std::make_unique<SpriteRenderComponent>(
      universe->getResourceManager(), ResourceManager::Texture::CommandCenter);
}

CommandCenter::~CommandCenter() {
}

void CommandCenter::tick(float adjustment) {
  m_universe->adjustPower(1000);
}

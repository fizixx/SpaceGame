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

#include "game/resource_manager.h"

#include <nucleus/logging.h>

#include "resources/sfml_loaders.h"

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

bool ResourceManager::loadAll(const std::string& root) {
  if (!m_fontStore.load(Font::Default,
                        loaders::fromFile<sf::Font>(root + "fonts\\arial.ttf"))) {
    LOG(Error) << "Could not load default font.";
    return false;
  }

  return true;
}

sf::Font* ResourceManager::getFont(Font font) {
  return m_fontStore.get(font);
}

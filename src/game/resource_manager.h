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

#ifndef GAME_RESOURCE_MANAGER_H_
#define GAME_RESOURCE_MANAGER_H_

#include <elastic/resources/resource_store.h>
#include <nucleus/macros.h>
#include <SFML/Graphics/Font.hpp>

class ResourceManager {
public:
  enum class Font {
    Default,
  };

  enum class Texture {
    CommandCenter,
  };

  ResourceManager();
  ~ResourceManager();

  // Load all resources.
  bool loadAll(const std::string& root);

  // Return the requested font.
  sf::Font* getFont(Font font);

  // Return the requested texture.
  sf::Texture* getTexture(Texture texture);

private:
  el::ResourceStore<sf::Font, Font> m_fontStore;
  el::ResourceStore<sf::Texture, Texture> m_textureStore;

  DISALLOW_COPY_AND_ASSIGN(ResourceManager);
};

#endif  // GAME_RESOURCE_MANAGER_H_

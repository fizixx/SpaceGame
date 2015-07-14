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

#include "nucleus/files/file_path.h"
#include "nucleus/logging.h"

#include "resources/sfml_loaders.h"

namespace {

static const struct {
  ResourceManager::Texture texture;
  const nu::FilePath::CharType* fileName;
} kTextures[] = {
    {ResourceManager::Texture::Unknown,
     FILE_PATH_LITERAL("images/unknown.png")},
    {ResourceManager::Texture::MouseCursor,
     FILE_PATH_LITERAL("images/mouse_cursor.png")},
    {ResourceManager::Texture::CameraTarget,
     FILE_PATH_LITERAL("images/camera_target.png")},
    {ResourceManager::Texture::CommandCenter,
     FILE_PATH_LITERAL("images/objects/command_center.png")},
    {ResourceManager::Texture::Asteroid1,
     FILE_PATH_LITERAL("images/objects/asteroid_1.png")},
    {ResourceManager::Texture::Asteroid2,
     FILE_PATH_LITERAL("images/objects/asteroid_2.png")},
    {ResourceManager::Texture::Asteroid3,
     FILE_PATH_LITERAL("images/objects/asteroid_3.png")},

    {ResourceManager::Texture::EnemyShip,
     FILE_PATH_LITERAL("images/objects/enemy_ship.png")},
    {ResourceManager::Texture::Bullet,
     FILE_PATH_LITERAL("images/objects/bullet.png")},
};

}  // namespace

ResourceManager::ResourceManager() {
}

ResourceManager::~ResourceManager() {
}

bool ResourceManager::loadAll(const nu::FilePath& root) {
  if (!m_fontStore.load(Font::Default,
                        loaders::fromFile<ca::Font>(
                            root.append(FILE_PATH_LITERAL("fonts"))
                                .append(FILE_PATH_LITERAL("arial.ttf"))))) {
    LOG(Error) << "Could not load default font.";
    return false;
  }

  for (usize i = 0; i < ARRAY_SIZE(kTextures); ++i) {
    if (!m_textureStore.load(kTextures[i].texture,
                             loaders::fromFile<ca::Texture>(
                                 root.append(kTextures[i].fileName)))) {
      LOG(Error) << "Could not load texture. (" << kTextures[i].fileName << ")";
      return false;
    }
  }

  return true;
}

ca::Font* ResourceManager::getFont(Font font) {
  return m_fontStore.get(font);
}

ca::Texture* ResourceManager::getTexture(Texture texture) {
  return m_textureStore.get(texture);
}

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

#ifndef RESOURCES_SFML_LOADERS_H_
#define RESOURCES_SFML_LOADERS_H_

#include <elastic/resources/resource_loader.h>
#include <nucleus/files/file_path.h>

namespace loaders {

namespace detail {

template <typename ResourceType, typename Func>
// ResourceType: The type of resource we want to load.
// Func: The signature of the function we must call.
el::ResourceLoader<ResourceType>
makeResourceLoader(Func func, const nu::FilePath::StringType &key) {
  auto loader = [=]() -> std::unique_ptr<ResourceType> {
    std::unique_ptr<ResourceType> resource(new ResourceType{});
    if (func(*resource)) {
      return resource;
    }
    return nullptr;
  };

  return el::ResourceLoader<ResourceType>(loader, key);
}

} // namespace detail

template <typename ResourceType>
// ResourceType: The type of resource we load.
el::ResourceLoader<ResourceType>
fromFile(const nu::FilePath::StringType &filename) {
  return detail::makeResourceLoader<ResourceType>([=](ResourceType &resource) {
#if OS(POSIX)
    return resource.loadFromFile(filename);
#else
    std::string s(filename.begin(), filename.end());
    return resource.loadFromFile(s);
#endif // OS(POSIX)
  }, filename);
}

template <>
el::ResourceLoader<sf::Texture>
fromFile(const nu::FilePath::StringType &filename) {
  return detail::makeResourceLoader<sf::Texture>([=](sf::Texture &texture) {
#if OS(POSIX)
    bool success = texture.loadFromFile(filename);
#else
    std::string s(filename.begin(), filename.end());
    bool success = texture.loadFromFile(s);
#endif // OS(POSIX)

    texture.setSmooth(true);
    return success;
  }, filename);
}

} // namespace loaders

#endif // RESOURCES_SFML_LOADERS_H_

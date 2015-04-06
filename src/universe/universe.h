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

#ifndef UNIVERSE_H_
#define UNIVERSE_H_

#include <memory>

#include <base/macros.h>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/View.hpp>
#include <SFML/Window/Event.hpp>

#include "universe/camera.h"

class Link;
class Object;

class Universe : public sf::Drawable {
public:
  // Construct the universe with the specified viewport size.
  explicit Universe(const sf::Vector2f& viewportSize);
  ~Universe();

  // Add or remove objects from the universe.
  void addObject(Object* object);
  void removeObject(Object* object);

  // Add or remove links.
  void addLink(Object* source, Object* destination);
  void removeLink(Object* any);

  // Handle any input events.
  void handleInput(sf::Event& event);

  // Tick the universe.
  void tick(float adjustment);

  // Override: sf::Drawable
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

private:
  sf::Vector2f mousePosToUniversePos(const sf::Vector2f& mousePos) const;

  // The camera we use to look into the universe.
  Camera m_camera;

  // The entire list of links that exist in the universe.
  std::vector<Link*> m_links;

  // The entire list of objects that exist in the universe.
  std::vector<Object*> m_objects;

  sf::CircleShape m_mousePosShape;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Universe);
};

#endif  // UNIVERSE_H_

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
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/VertexArray.hpp>
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
  // Find the object that is currently under the mouse.
  void updateHoverObject();

  // The camera we use to look into the universe.
  Camera m_camera;

  // The entire list of links that exist in the universe.
  std::vector<Link*> m_links;

  // The entire list of objects that exist in the universe.
  std::vector<Object*> m_objects;

  // The current mouse position in the world.  Updated on every input event.
  sf::Vector2f m_mousePos;

  // The current object that is being hovered over.
  Object* m_hoverObject{nullptr};

  // The current object that is selected.
  Object* m_selectedObject{nullptr};

  // A shape to show where the current mouse position is in the universe.
  sf::CircleShape m_mousePosShape;

  // The rectangle we use to draw the hover outline.
  sf::RectangleShape m_hoverShape;

  // The rectangle we use to draw the selected outline.
  sf::RectangleShape m_selectedShape;

  // The background texture.
  sf::Texture m_backgroundTexture;

  // The geometry of the background image.
  sf::VertexArray m_backgroundShape;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Universe);
};

#endif  // UNIVERSE_H_

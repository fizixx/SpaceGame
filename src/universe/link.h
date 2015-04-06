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

#ifndef UNIVERSE_LINK_H_
#define UNIVERSE_LINK_H_

#include <base/macros.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Universe;
class Object;

class Link : public sf::Drawable {
public:
  Link(Universe* universe, Object* source, Object* destination);
  virtual ~Link() override;

  // Getters/Setters
  Object* getSource() { return m_source; }
  Object* getDestination() { return m_destination; }

  // Override: sf::Drawable
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

private:
  // Calculate the position and size of the shape.
  void calculateShape();

  // The universe we belong to.
  Universe* m_universe;

  // The source and destination that the link connects.
  Object* m_source{nullptr};
  Object* m_destination{nullptr};

  // The shape we draw for the link.
  sf::RectangleShape m_shape;

  DISALLOW_IMPLICIT_CONSTRUCTORS(Link);
};

#endif  // UNIVERSE_LINK_H_

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

#include "canvas/rendering/canvas.h"
#include "nucleus/macros.h"

class Object;
class Universe;

class Link {
public:
  Link(Universe* universe, Object* source, Object* destination);
  ~Link();

  // Return the source and destination of the link.
  Object* getSource() const { return m_source; }
  Object* getDestination() const { return m_destination; }

  void tick(float adjustment);

  // Override: sf::Drawable
  void render(ca::Canvas* canvas) const;

private:
  // The universe we belong to.
  Universe* m_universe;

  // The source object of the link.
  Object* m_source;

  // The destination object of the link.
  Object* m_destination;

  // The shape we use to render the link.
#if 0
  sf::RectangleShape m_shape;
#endif  // 0

  DISALLOW_IMPLICIT_CONSTRUCTORS(Link);
};

#endif  // UNIVERSE_LINK_H_

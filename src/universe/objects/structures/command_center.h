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

#ifndef UNIVERSE_OBJECTS_STRUCTURES_COMMAND_CENTER_H_
#define UNIVERSE_OBJECTS_STRUCTURES_COMMAND_CENTER_H_

#include "canvas/rendering/texture.h"
#include "canvas/rendering/sprite.h"

#include "universe/objects/structures/structure.h"

class CommandCenter : public Structure {
  DECLARE_STRUCTURE(CommandCenter);

public:
  CommandCenter(Universe* universe, const ca::Vec2& pos);
  ~CommandCenter() override;

  // Override: Object
  void tick(f32 adjustment) override;

private:
  DISALLOW_IMPLICIT_CONSTRUCTORS(CommandCenter);
};

#endif  // UNIVERSE_OBJECTS_STRUCTURES_COMMAND_CENTER_H_

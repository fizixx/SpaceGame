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

#ifndef UTILS_COMPONENT_H_
#define UTILS_COMPONENT_H_

#include "canvas/rendering/canvas.h"
#include "canvas/windows/event.h"

class Component {
public:
  virtual ~Component();

  // Tick the component with the supplied adjustment to make a smooth 60fps.
  virtual void tick(float adjustment) = 0;

  // Render the component.
  virtual void render(ca::Canvas* canavs) const = 0;
};

class InputComponent : public Component {
public:
  ~InputComponent() override;

  // Handle input on the component.
  virtual void onMouseMoved(const ca::MouseEvent& evt) = 0;
  virtual void onMousePressed(const ca::MouseEvent& evt) = 0;
  virtual void onMouseReleased(const ca::MouseEvent& evt) = 0;
  virtual void onMouseWheel(const ca::MouseWheelEvent& evt) = 0;
  virtual void onKeyPressed(const ca::KeyEvent& evt) = 0;
  virtual void onKeyReleased(const ca::KeyEvent& evt) = 0;
};

#endif  // UTILS_COMPONENT_H_

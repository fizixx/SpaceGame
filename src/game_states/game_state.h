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

#ifndef GAME_STATES_GAME_STATE_H_
#define GAME_STATES_GAME_STATE_H_

#include <memory>

#include "elastic/context.h"
#include "nucleus/macros.h"
#include "canvas/windows/event.h"

#include "utils/component.h"

class GameState : public InputComponent {
public:
  explicit GameState(el::Context* context);
  ~GameState() override = default;

  // Override: Component
  void onMouseMoved(const ca::MouseEvent& evt) override;
  void onMousePressed(const ca::MouseEvent& evt) override;
  void onMouseReleased(const ca::MouseEvent& evt) override;
  void onMouseWheel(const ca::MouseWheelEvent& evt) override;
  void tick(float adjustment) override;
  void render(ca::Canvas* canvas) const override;

protected:
  // Every game state has a UI component.
  el::Context* m_uiContext;

private:
  DISALLOW_COPY_AND_ASSIGN(GameState);
};

#endif  // GAME_STATES_GAME_STATE_H_

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

#ifndef GAME_STATES_GAME_STATE_UNIVERSE_H_
#define GAME_STATES_GAME_STATE_UNIVERSE_H_

#include <memory>

#include "game_states/game_state.h"
#include "universe/universe.h"
#include "ui/views/button.h"

class GameStateUniverse : public GameState, public ui::Button::OnClickListener {
public:
  explicit GameStateUniverse(const sf::Vector2f& viewportSize);
  virtual ~GameStateUniverse() override;

  // Override: ui::Button::OnClickListener
  virtual void onButtonClicked(ui::Button* sender) override;

private:
  // Create the user interface and add it to the parent ui specified.
  void createUserInterface(ui::Context* context, ui::GroupView* parent);

  // The universe for this game state.
  std::unique_ptr<Universe> m_universe;

  // Mapped UI controls.
  ui::Button* m_testButton{nullptr};

  ui::Button* m_createPowerGeneratorButton{nullptr};

  DISALLOW_COPY_AND_ASSIGN(GameStateUniverse);
};

#endif  // GAME_STATES_GAME_STATE_UNIVERSE_H_

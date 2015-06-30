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

#include "game_states/game_state.h"

GameState::GameState(el::Context* context) : m_uiContext(context) {
}

void GameState::onMouseMoved(const ca::MouseEvent& evt) {
  m_uiContext->onMouseMoved(evt);
}

void GameState::onMousePressed(const ca::MouseEvent& evt) {
  m_uiContext->onMousePressed(evt);
}

void GameState::onMouseReleased(const ca::MouseEvent& evt) {
  m_uiContext->onMouseReleased(evt);
}

void GameState::onMouseWheel(const ca::MouseWheelEvent& evt) {
  m_uiContext->onMouseWheel(evt);
}

void GameState::tick(float adjustment) {
  m_uiContext->tick(adjustment);
}

void GameState::render(ca::Canvas* canvas) const {
  m_uiContext->render(canvas);
}

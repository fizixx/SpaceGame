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

#include "game_states/game_state_universe.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "ui/views/color_view.h"

GameStateUniverse::GameStateUniverse(const sf::Vector2f& viewportSize)
  : m_universe(std::make_unique<Universe>(viewportSize)) {
  // Build the UI.
  ColorView* colorView = new ColorView(m_uiContext.get(), sf::Color(255, 0, 0));
  colorView->setMinSize(sf::Vector2i{100, 100});

  m_uiContext->getRoot()->addChild(colorView);
}

GameStateUniverse::~GameStateUniverse() {
}

void GameStateUniverse::handleInput(sf::Event& event) {
  m_universe->handleInput(event);
  GameState::handleInput(event);
}

void GameStateUniverse::tick(float adjustment) {
  m_universe->tick(adjustment);
  GameState::tick(adjustment);
}

void GameStateUniverse::draw(sf::RenderTarget& target,
                             sf::RenderStates states) const {
  target.draw(*m_universe, states);
  GameState::draw(target, states);
}

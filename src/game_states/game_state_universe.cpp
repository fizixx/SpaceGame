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

#include <elastic/views/button.h>
#include <elastic/views/linear_sizer_view.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/objects/power_generator.h"
#include "universe/universe_view.h"

GameStateUniverse::GameStateUniverse(ResourceManager* resourceManager,
                                     el::Context* context)
  : GameState(context),
    m_universe(std::make_unique<Universe>(resourceManager)) {
  // Add the user interface to the UI tree.
  createUserInterface(m_uiContext, m_uiContext->getRoot());
}

GameStateUniverse::~GameStateUniverse() {
}

void GameStateUniverse::onButtonClicked(el::Button* sender) {
  if (sender == m_createPowerGeneratorButton) {
    // Create the power generator.
    std::unique_ptr<Object> powerGenerator =
        std::make_unique<PowerGenerator>(m_universe.get());
    m_universeView->startPlacingObject(std::move(powerGenerator));
    return;
  }
}

void GameStateUniverse::createUserInterface(el::Context* context,
                                            el::GroupView* parent) {
  // Add the universe view.
  m_universeView = new UniverseView(context, m_universe.get());
  m_universeView->setName("universe");
  m_universeView->setExpand(el::View::ExpandBoth);
  parent->addChild(m_universeView);

  // Create a container for all the buttons.
  el::LinearSizerView* buttonContainer = new el::LinearSizerView(
      context, el::LinearSizerView::OrientationVertical);
  buttonContainer->setName("buttonContainer");
  buttonContainer->setVerticalAlign(el::View::AlignTop);
  buttonContainer->setHorizontalAlign(el::View::AlignLeft);

  m_createPowerGeneratorButton =
      new el::Button(context, "Power Generator", this);
  m_createPowerGeneratorButton->setName("createPowerGenerator");
  m_createPowerGeneratorButton->setMinSize(sf::Vector2i{300, 0});
  buttonContainer->addChild(m_createPowerGeneratorButton);

  parent->addChild(buttonContainer);
}

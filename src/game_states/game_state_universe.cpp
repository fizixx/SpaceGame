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

#include <elastic/views/button_view.h>
#include <elastic/views/linear_sizer_view.h>
#include <SFML/Graphics/RenderTarget.hpp>

#include "universe/objects/structures/miner.h"
#include "universe/objects/structures/power_generator.h"
#include "universe/objects/structures/turret.h"
#include "universe/universe_view.h"

namespace {

template <typename ObjectType>
std::string buttonLabelForObject() {
  std::string label{ObjectType::typeName};
  label += " (";
  label += std::to_string(ObjectType::mineralCost);
  label += ")";

  return std::move(label);
}

}  // namespace

GameStateUniverse::GameStateUniverse(ResourceManager* resourceManager,
                                     el::Context* context)
  : GameState(context),
    m_universe(std::make_unique<Universe>(resourceManager)) {
  // Add the user interface to the UI tree.
  createUserInterface(m_uiContext, m_uiContext->getRoot());
}

GameStateUniverse::~GameStateUniverse() {
}

void GameStateUniverse::tick(float adjustment) {
  GameState::tick(adjustment);

  // Forward the tick to the universe as well.
  m_universe->tick(adjustment);

  // Update the total power label.
  m_totalPowerText->setLabel(std::to_string(m_universe->getPower()));

  // Update the total minerals label.
  m_totalMineralsText->setLabel(std::to_string(m_universe->getMinerals()));
}

void GameStateUniverse::onButtonClicked(el::ButtonView* sender) {
  if (sender == m_createPowerGeneratorButton) {
    // Create the power generator.
    m_universeView->startPlacingObject(
        std::make_unique<PowerGenerator>(m_universe.get()));
    return;
  }

  if (sender == m_createMinerButton) {
    // Create the Miner.
    m_universeView->startPlacingObject(
        std::make_unique<Miner>(m_universe.get()));
    return;
  }

  if (sender == m_createTurretButton) {
    // Create the Turret.
    m_universeView->startPlacingObject(
        std::make_unique<Turret>(m_universe.get()));
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

  // Create a vertical linear sizer to hold the game title and the button
  // container.
  el::LinearSizerView* mainSizer = new el::LinearSizerView{
      context, el::LinearSizerView::OrientationVertical};
  mainSizer->setExpand(el::View::ExpandBoth);

  // Create the info bar.
  auto infoBar = new el::LinearSizerView{
      context, el::LinearSizerView::OrientationHorizontal};
  infoBar->setExpand(el::View::ExpandHorizontal);

  m_totalPowerText = new el::TextView(context);
  m_totalPowerText->setName("totalPowerText");
  m_totalPowerText->setHorizontalAlign(el::View::AlignRight);
  m_totalPowerText->setProportion(1);
  infoBar->addChild(m_totalPowerText);

  auto spacer1 = new el::View(context);
  spacer1->setMinSize(sf::Vector2i{50, 0});
  infoBar->addChild(spacer1);

  m_totalMineralsText = new el::TextView(context);
  m_totalMineralsText->setName("totalMineralsText");
  m_totalMineralsText->setHorizontalAlign(el::View::AlignLeft);
  m_totalMineralsText->setProportion(1);
  infoBar->addChild(m_totalMineralsText);

  mainSizer->addChild(infoBar);

  // Create a container for all the buttons.
  el::LinearSizerView* buttonContainer = new el::LinearSizerView(
      context, el::LinearSizerView::OrientationVertical);
  buttonContainer->setName("buttonContainer");
  buttonContainer->setVerticalAlign(el::View::AlignTop);
  buttonContainer->setHorizontalAlign(el::View::AlignLeft);
  buttonContainer->setProportion(1);

  m_createPowerGeneratorButton =
      new el::ButtonView(context, buttonLabelForObject<PowerGenerator>(), this);
  m_createPowerGeneratorButton->setName("createPowerGenerator");
  m_createPowerGeneratorButton->setMinSize(sf::Vector2i{300, 0});
  buttonContainer->addChild(m_createPowerGeneratorButton);

  m_createMinerButton =
      new el::ButtonView(context, buttonLabelForObject<Miner>(), this);
  m_createMinerButton->setName("createMinerButton");
  m_createMinerButton->setMinSize(sf::Vector2i{300, 0});
  buttonContainer->addChild(m_createMinerButton);

  m_createTurretButton =
      new el::ButtonView(context, buttonLabelForObject<Turret>(), this);
  m_createTurretButton->setName("createTurretButton");
  m_createTurretButton->setMinSize(sf::Vector2i{300, 0});
  buttonContainer->addChild(m_createTurretButton);

  mainSizer->addChild(buttonContainer);

  parent->addChild(mainSizer);

  // Set the universe view as the view that receives keyboard focus.
  m_uiContext->setFocusView(m_universeView);
}

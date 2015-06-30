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

#include <cstdlib>
#include <ctime>
#include <chrono>

#include "canvas/app.h"
#include "nucleus/logging.h"

#include "game/resource_manager.h"
#include "game/ui_context.h"
#include "game_states/game_state_universe.h"

class SpaceGame : public ca::WindowDelegate {
public:
  SpaceGame() = default;

  // Override: ca::WindowDelegate
  bool onWindowCreated() override {
    LOG(Info) << "Starting SpaceGame";
    // Seed the random number generator.
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Initialize the resource manager.
    if (!m_resourceManager.loadAll(nu::FilePath(
            FILE_PATH_LITERAL("C:\\Workspace\\SpaceGame\\res\\")))) {
      return false;
    }

    // Create a user interface context for the universe game state.
    m_uiContext = std::make_unique<UiContext>(&m_resourceManager);

    // Construct the universe game state.
    m_gameState = std::make_unique<GameStateUniverse>(&m_resourceManager,
                                                      m_uiContext.get());

    using Clock = std::chrono::high_resolution_clock;
    auto lastTick = Clock::now();

    return ca::WindowDelegate::onWindowCreated();
  }

  void onMouseMoved(const ca::MouseEvent& evt) override {
    m_gameState->onMouseMoved(evt);
  }

  void onMousePressed(const ca::MouseEvent& evt) override {
    m_gameState->onMousePressed(evt);
  }

  void onMouseReleased(const ca::MouseEvent& evt) override {
    m_gameState->onMouseReleased(evt);
  }

  void onMouseWheel(const ca::MouseWheelEvent& evt) override {
    m_gameState->onMouseWheel(evt);
  }

  void onKeyPressed(const ca::KeyEvent& evt) override {
    m_gameState->onKeyPressed(evt);
  }

  void onKeyReleased(const ca::KeyEvent& evt) override {
    m_gameState->onKeyReleased(evt);
  }

  void onPaint(ca::Canvas* canvas) override {
    m_gameState->tick(1.f);

    // Clear the viewport with black.
    canvas->clear(ca::Color{31, 63, 95, 255});

    // Draw the universe.
    m_gameState->render(canvas);
  }

private:
  ResourceManager m_resourceManager;
  std::unique_ptr<el::Context> m_uiContext;
  std::unique_ptr<GameStateUniverse> m_gameState;

  DISALLOW_COPY_AND_ASSIGN(SpaceGame);
};

CANVAS_APP(SpaceGame);

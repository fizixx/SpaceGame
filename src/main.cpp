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
#include <chrono>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <nucleus/files/file_path.h>
#include <nucleus/logging.h>

#include "game/resource_manager.h"
#include "game/ui_context.h"
#include "game_states/game_state_universe.h"

int main() {
  LOG(Info) << "Starting SpaceGame";

  // Seed the random number generator.
  std::srand(static_cast<unsigned int>(std::time(nullptr)));

  sf::ContextSettings settings{32, 0, 4};
  sf::RenderWindow window{sf::VideoMode{1600, 900, 32}, "SpaceGame",
                          sf::Style::Default, settings};
  window.setVerticalSyncEnabled(true);
  window.setKeyRepeatEnabled(false);

  // Initialize the resource manager.
  ResourceManager resourceManager;

  if (!resourceManager.loadAll(nu::FilePath(FILE_PATH_LITERAL("res")))) {
    return 1;
  }

  // Create a user interface context for the universe game state.
  auto context = std::make_unique<UiContext>(&resourceManager);

  // Construct the universe game state.
  std::unique_ptr<GameState> gameState =
      std::make_unique<GameStateUniverse>(&resourceManager, context.get());

  using Clock = std::chrono::high_resolution_clock;
  auto lastTick = Clock::now();

  while (window.isOpen()) {
    sf::Event evt;
    if (window.pollEvent(evt)) {
      switch (evt.type) {
        case sf::Event::Closed:
          window.close();
          break;
      }

      // Let the universe also handle events.
      gameState->handleInput(evt);
    }

    // We expect that 16.666ms went by since the last tick.  (60fps)
    auto now = Clock::now();
    std::chrono::duration<float, std::chrono::milliseconds::period> timePassed =
        std::chrono::duration_cast<std::chrono::microseconds>(now - lastTick);
    lastTick = now;

    // We calculate the adjust we must make to get a smooth 60fps tick.
    float adjustment = timePassed.count() * 60.f / 1000.f;
    gameState->tick(adjustment);

    // Clear the viewport with black.
    window.clear(sf::Color(31, 31, 31, 255));

    // Draw the universe.
    window.draw(*gameState);

    window.display();
  }

  return 0;
}

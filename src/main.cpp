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

#include <chrono>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>
#include <base/logging.h>

#include "universe.h"

int main() {
  LOG(Info) << "Starting SpaceGame";

  sf::RenderWindow window(sf::VideoMode(1600, 900, 32), "SpaceGame");
  window.setVerticalSyncEnabled(true);

  sf::Vector2u windowSize{window.getSize()};
  Universe universe{sf::Vector2f(static_cast<float>(windowSize.x),
                                 static_cast<float>(windowSize.y))};

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
      universe.handleInput(evt);
    }

    // We expect that 16.666ms went by since the last tick.  (60fps)
    auto now = Clock::now();
    std::chrono::duration<float, std::chrono::milliseconds::period> timePassed =
        std::chrono::duration_cast<std::chrono::microseconds>(now - lastTick);
    lastTick = now;

    // We calculate the adjust we must make to get a smooth 60fps tick.
    float adjustment = timePassed.count() * 60.f / 1000.f;
    universe.tick(adjustment);

    // Clear the viewport with black.
    window.clear(sf::Color(0, 0, 0));

    // Draw the universe.
    window.draw(universe);

    window.display();
  }

  return 0;
}

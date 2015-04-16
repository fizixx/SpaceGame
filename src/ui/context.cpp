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

#include "ui/context.h"

namespace ui {

Context::Context() : m_contextView(this) {
}

Context::~Context() {
}

void Context::handleInput(sf::Event& event) {
}

void Context::tick(float adjustment) {
}

void Context::draw(sf::RenderTarget& target, sf::RenderStates states) const {
  // Get the size of the render target in pixels for the UI to render.
  sf::IntRect layoutRect{0, 0, static_cast<int>(target.getSize().x),
                         static_cast<int>(target.getSize().y)};

  m_contextView.layout(layoutRect);
  target.draw(m_contextView, states);
}

}  // namespace ui

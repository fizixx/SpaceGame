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

#include "ui/view.h"

#include "ui/context.h"

View::View(Context* context)
  : m_context(context),
    m_horizontalAlign(AlignCenter),
    m_verticalAlign(AlignCenter),
    m_expand(ExpandNone) {
}

View::~View() {
}

void View::setName(const std::string& name) {
  m_name = name;
}

void View::setMinSize(const sf::Vector2i& minSize) {
  m_minSize = minSize;
}

void View::setHorizontalAlign(AlignType align) {
  m_horizontalAlign = align;
}

void View::setVerticalAling(AlignType align) {
  m_verticalAlign = align;
}

void View::setExpand(ExpandType expand) {
  m_expand = expand;
}

sf::Vector2i View::calculateMinSize() const {
  return m_minSize;
}

void View::layout(const sf::IntRect& rect) {
  m_rect = rect;
}

void View::render(sf::RenderTarget* target) {
}

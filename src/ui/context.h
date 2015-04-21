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

#ifndef UI_CONTEXT_H_
#define UI_CONTEXT_H_

#include <string>

#include <SFML/Graphics/Font.hpp>

#include "resources/resource_store.h"
#include "ui/views/context_view.h"
#include "utils/component.h"

namespace ui {

class View;

class Context : public Component {
public:
  Context();
  ~Context();

  // Get the root view of the context.
  ContextView* getRoot() { return &m_contextView; }

  // Resources
  virtual sf::Font* getFont(const std::string& name) = 0;

  // Override: Component
  virtual void handleInput(sf::Event& event) override;
  virtual void tick(float adjustment) override;
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

protected:
  friend class View;

  // The root view of our hierarchy.
  mutable ContextView m_contextView;

private:
  DISALLOW_COPY_AND_ASSIGN(Context);
};

}  // namespace ui

#endif  // UI_CONTEXT_H_

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

#ifndef GAME_UI_CONTEXT_H_
#define GAME_UI_CONTEXT_H_

#include <elastic/context.h>

#include "game/resource_manager.h"

class ResourceManager;

class UiContext : public el::Context {
public:
  explicit UiContext(ResourceManager* resourceManager);
  ~UiContext();

  // Override: ui::Context
  sf::Font* getFont(const std::string& name) override;

private:
  // The resource manager we use to serve resources to the UI.
  ResourceManager* m_resourceManager;

  DISALLOW_COPY_AND_ASSIGN(UiContext);
};

#endif  // UTILS_UI_CONTEXT_H_

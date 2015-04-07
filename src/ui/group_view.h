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

#ifndef UI_GROUP_VIEW_H_
#define UI_GROUP_VIEW_H_

#include <vector>

#include "ui/view.h"

class GroupView : public View {
public:
  static sf::IntRect layoutControlInRect(View* view, const sf::IntRect& rect);

  explicit GroupView(Context* context);
  virtual ~GroupView();

  void addChild(View* view);
  void removeChild(View* view);

  // Override: View
  virtual void render(sf::RenderTarget* target) override;

protected:
  // This view's child views.
  std::vector<View*> m_children;

  DISALLOW_IMPLICIT_CONSTRUCTORS(GroupView);
};

#endif  // UI_GROUP_VIEW_H_

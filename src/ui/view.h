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

#ifndef UI_VIEW_H_
#define UI_VIEW_H_

#include <string>

#include <base/macros.h>
#include <SFML/Graphics/RenderTarget.hpp>

class Context;

class View {
public:
  enum AlignType {
    AlignLeft,
    AlignTop,
    AlignCenter,
    AlignRight,
    AlignBottom,
  };

  enum ExpandType {
    ExpandNone,
    ExpandHorizontal,
    ExpandVertical,
    ExpandBoth,
  };

  explicit View(Context* context);
  virtual ~View();

  // name
  const std::string getName() const { return m_name; }
  void setName(const std::string& name);

  // minsize
  const sf::Vector2i& getMinSize() const { return m_minSize; }
  void setMinSize(const sf::Vector2i& minSize);

  // horizontalalign
  AlignType getHorizontalAlign() const { return m_horizontalAlign; }
  void setHorizontalAlign(AlignType align);

  // verticalalign
  AlignType getVerticalAlign() const { return m_verticalAlign; }
  void setVerticalAling(AlignType align);

  // expand
  ExpandType getExpand() const { return m_expand; }
  void setExpand(ExpandType expand);

  virtual sf::Vector2i calculateMinSize() const;
  virtual void layout(const sf::IntRect& rect);
  virtual void render(sf::RenderTarget* target);

protected:
  // The context we belong to.
  Context* m_context;

  // The rect where this view has been layed out to.
  sf::IntRect m_rect;

  // The name of the control.  This can remain empty for anonymous controls.
  std::string m_name;

  // The minimum size of the view.
  sf::Vector2i m_minSize;

  // Horizontal/vertical align.
  AlignType m_horizontalAlign;
  AlignType m_verticalAlign;

  // Expand type of the view.
  ExpandType m_expand;

  DISALLOW_IMPLICIT_CONSTRUCTORS(View);
};

#endif  // UI_VIEW_H_

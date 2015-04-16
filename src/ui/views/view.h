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

#include <nucleus/macros.h>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

namespace ui {

class Context;
class GroupView;

class View : public sf::Drawable {
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

  // Return the parent of the view, if any.
  View* getParent() const { return m_parent; }

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

  // Virtual Interface

  virtual View* getViewAtPosition(const sf::Vector2i& pos);

  virtual void tick(float adjustment);
  virtual sf::Vector2i calculateMinSize() const;
  virtual void layout(const sf::IntRect& rect);

  // Events

  virtual bool onMousePressed(sf::Event& event);
  virtual bool onMouseDragged(sf::Event& event);
  virtual void onMouseMoved(sf::Event& event);
  virtual void onMouseReleased(sf::Event& event);

  // Override: sf::Drawable
  virtual void draw(sf::RenderTarget& target,
                    sf::RenderStates states) const override;

protected:
  friend class GroupView;

  // The context we belong to.
  Context* m_context;

  // The parent of this view.
  View* m_parent{nullptr};

  // The rect where this view has been layed out to.
  sf::IntRect m_rect;

  // The minimum size of the view.
  sf::Vector2i m_minSize;

  // Horizontal/vertical align.
  AlignType m_horizontalAlign;
  AlignType m_verticalAlign;

  // Expand type of the view.
  ExpandType m_expand;

  DISALLOW_IMPLICIT_CONSTRUCTORS(View);
};

}  // namespace ui

#endif  // UI_VIEW_H_

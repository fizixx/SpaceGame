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

#include "ui/views/context_view.h"

namespace ui {

ContextView::ContextView(Context* context) : StackedSizerView(context) {
}

ContextView::~ContextView() {
}

bool ContextView::onMousePressed(sf::Event& event) {
  // If m_mousePressedHandler is not null, we are currently processing a
  // pressed->drag->released session.  In that case we send the event to
  // m_mousePressedHandler.
  if (m_mousePressedHandler) {
    m_mousePressedHandler->onMousePressed(event);
    return true;
  }

  bool hitDisabledView = false;
  sf::Vector2i mousePos{event.mouseButton.x, event.mouseButton.y};

#if 0
  for (m_mousePressedHandler = getViewAtPosition(mousePos);
       m_mousePressedHandler && (m_mousePressedHandler != this);
       m_mousePressedHandler = m_mousePressedHandler->getParent()) {
  }
#endif  // 0

#if 0
  bool hitDisabledView = false;

  for (m_mousePressedHandler = GetControlAtPosition(evt.GetPosition());
       m_mousePressedHandler && (m_mousePressedHandler != this);
       m_mousePressedHandler = m_mousePressedHandler->GetParent()) {
    // TODO(tiaan.louw): We are not checking |m_mousePressedHandler| for 0 here.

    // Disabled controls should eat the events and not propagate them any
    // further.
    if (!m_mousePressedHandler->IsEnabled()) {
      hitDisabledView = true;
      break;
    }

    // See if this control wants to handle the mouse press.
    MouseEvent mousePressedEvent(evt, this, m_mousePressedHandler);

    // Remove the double-click flag if the handler is different than the one
    // which got the first click part of the double-click.
    if (m_mousePressedHandler != m_lastClickHandler)
      mousePressedEvent.SetFlags(evt.GetFlags() &
                                 ~MouseEvent::EF_IS_DOUBLE_CLICK);

    m_dragInfo.Reset();

    bool handled = m_mousePressedHandler->ProcessMousePressed(mousePressedEvent,
                                                              &m_dragInfo);

    // The control could have removed itself from the tree when handling
    // OnMousePressed(). In this case, the removal notification will have
    // reset m_mousePressedHandler to NULL out from under us. Detect this case
    // and stop.
    // NOTE: We don't return true here, because we don't want the frame to
    // forward future events to us when there is no handler.
    if (!m_mousePressedHandler)
      break;

    // If thw control handled the event, leave m_mousePressedHandler set and
    // return true, which will cause subsequent drag/release events to get
    // forwarded to that view.
    if (handled) {
      m_lastClickHandler = m_mousePressedHandler;
      return true;
    }
  }

  // Reset m_mousePressedHandler to indicate that no processing is occurring.
  m_mousePressedHandler = NULL;

  //	if (focus_on_mouse_pressed_) {
  //	#if defined(OS_WIN)
  //	  HWND hwnd = GetWidget()->GetNativeView();
  //	  if (::GetFocus() != hwnd)
  //	    ::SetFocus(hwnd);
  //	#else
  //	  GtkWidget* widget = GetWidget()->GetNativeView();
  //	  if (!gtk_widget_is_focus(widget))
  //	    gtk_widget_grab_focus(widget);
  //	#endif
  //	}

  // In the event that a double-click is not handled after traversing the
  // entire hierarchy (even as a single-click when send to a different view),
  // it must be marked as handled to avoid anything happening from default
  // processing if it the first click-part was handled by us.
  if (m_lastClickHandler &&
      (evt.GetFlags() & MouseEvent::EF_IS_DOUBLE_CLICK) != 0)
    hitDisabledView = true;

  m_lastClickHandler = NULL;
  return hitDisabledView;
#endif  // 0

  return false;
}

bool ContextView::processMousePressed(sf::Event& event, bool isDouble) {
  m_lastMouseEventWasMove = false;

  if (onMousePressed(event)) {
    m_isMouseDown = true;

    if (!m_hasCapture) {
      m_hasCapture = true;
    }

    return true;
  }

  return false;
}

void ContextView::processMouseDragged(sf::Event& event) {
  m_lastMouseEventWasMove = false;

  onMouseDragged(event);
}

void ContextView::processMouseReleased(sf::Event& event) {
  m_lastMouseEventWasMove = false;

  if (m_hasCapture) {
    m_hasCapture = false;
  }

  m_isMouseDown = false;

  onMouseReleased(event);
}

void ContextView::processMouseMoved(sf::Event& event) {
  if (m_hasCapture && m_isMouseDown) {
    processMouseDragged(event);
  } else {
    sf::Vector2i mousePos{event.mouseMove.x, event.mouseMove.y};
    if (m_lastMouseEventWasMove && (m_lastMouseMovePos == mousePos)) {
      return;
    }
    m_lastMouseMovePos = mousePos;
    m_lastMouseEventWasMove = true;
    onMouseMoved(event);
  }
}

void ContextView::processMouseWheel(sf::Event& event) {
#if 0
  // Get the control that is at the top, below the mouse position.
  Control* top = GetControlAtPosition(pos);

  if (!top)
    return;

  MouseWheelEvent evt(delta, pos.x, pos.y, 0);
  top->OnMouseWheel(evt);
#endif  // 0
}

}  // namespace ui

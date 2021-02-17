#include <M5EPD.h>

#include "widget_main.hpp"

void WidgetContext::Draw() {
  const auto& top_view = view_stack_.top();

  if (M5.TP.avaliable()) {
    M5.TP.update();
    bool is_touch = !M5.TP.isFingerUp();
    uint8_t numFingers = M5.TP.getFingerNum();
    // Only support single finger touch
    if (numFingers == 1) {
      log_d("Event %d, %d, %d, %d", event_.x2, event_.y2, state_, is_touch);

      // If currently no active touch event and finger is down, start touch
      if (state_ == TS_NONE && is_touch) {
        tp_finger_t tp = M5.TP.readFinger(0);
        event_.x1 = tp.x;
        event_.y1 = tp.y;
        event_.x2 = tp.x;
        event_.y2 = tp.y;
        event_.type = EventType::TOUCH_DOWN;
        event_.id = tp.id;
        event_.size = tp.size;
        state_ = TS_TOUCH;
        if (top_view->EventInside(event_.x1, event_.y1)) {
          top_view->HandleEvent(event_);
        }
      }

      if (state_ == TS_TOUCH && is_touch) {
        tp_finger_t tp = M5.TP.readFinger(0);
        event_.x2 = tp.x;
        event_.y2 = tp.y;
        if (top_view->EventInside(event_.x1, event_.y1)) {
          top_view->HandleEvent(event_);
        }
      }

      if (state_ == TS_TOUCH && !is_touch) {
        state_ = TS_NONE;
        event_.type = EventType::TOUCH_UP;
        if (top_view->EventInside(event_.x1, event_.y1)) {
          top_view->HandleEvent(event_);
        }
      }
    }
  }
  top_view->Draw();

}

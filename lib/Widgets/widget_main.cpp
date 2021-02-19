#include <M5EPD.h>

#include "widget_main.hpp"

void WidgetContext::HandleTouchEvents() {
  if (M5.TP.avaliable()) {
    M5.TP.update();
    bool is_touch = !M5.TP.isFingerUp();
    // Wrap the raw device event in a struct to be able to run a simple
    // comparison.
    RawEvent current_event;
    if (is_touch) {
      current_event.evt = M5.TP.readFinger(0);
    } else {
      current_event.evt = {0, 0, 0, 0};
    }
    current_event.touch = is_touch;
    current_event.fingers = M5.TP.getFingerNum();
    // Only support single finger touch
    if (current_event != last_raw_event_) {
      // If currently no active touch event and finger is down, start touch
      if (state_ == TS_NONE && is_touch) {
        tp_finger_t tp = current_event.evt;
        event_.x1 = tp.x;
        event_.y1 = tp.y;
        event_.x2 = tp.x;
        event_.y2 = tp.y;
        event_.type = EventType::TOUCH_DOWN;
        event_.id = tp.id;
        event_.size = tp.size;
        state_ = TS_TOUCH;
      }

      if (state_ == TS_TOUCH && is_touch) {
        tp_finger_t tp = current_event.evt;
        event_.x2 = tp.x;
        event_.y2 = tp.y;
      }

      if (state_ == TS_TOUCH && !is_touch) {
        state_ = TS_NONE;
        event_.type = EventType::TOUCH_UP;
      }

      log_d("New Event %s, %d, %d", event_.str().c_str(), state_, is_touch);
      std::vector<View::ptr_t> tmp(view_stack_.begin(), view_stack_.end());
      for (const auto &v : tmp) {
        if (v->EventInside(event_.x1, event_.y1)) {
          v->HandleEvent(event_);
        }
      }
      last_raw_event_ = current_event;
    }
    M5.TP.flush();
  }
}

void WidgetContext::Draw() {
  HandleTouchEvents();

  // Run any modifications on the view stack
  if (remove_last_) {
    log_d("Remove last view...");
    view_stack_.pop_back();
    remove_last_ = false;
  }

  // Add new views
  if (!view_changes_.empty()) {
    log_d("Add new views.");
    view_stack_.insert(view_stack_.end(), view_changes_.begin(),
                       view_changes_.end());
    view_changes_.clear();
  }

  ScreenUpdateMode mode = ScreenUpdateMode::NONE;
  for (const auto &v : view_stack_) {
    mode |= v->Draw();
  }

  if (mode == ScreenUpdateMode::FULL) {
    log_d("Update full screen.");
    if (M5.EPD.UpdateCount() > 4) {
      M5.EPD.UpdateFull(UPDATE_MODE_GC16);
      M5.EPD.ResetUpdateCount();
    }
  }
}

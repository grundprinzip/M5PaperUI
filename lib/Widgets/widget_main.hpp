#pragma once
#include <cstdint>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <M5EPD.h>

#include "widget_constants.hpp"
#include "widget_frame.hpp"

class WidgetContext {
public:
  using ptr_t = std::shared_ptr<WidgetContext>;

  // Push frame by name
  void PushFrame(const std::string &n) {
    log_d("Pushing new frame.");

    assert(view_registry_.count(n) > 0);
    const auto &v = view_registry_[n];
    v->Init(this);
    view_changes_.push_back(v);
  }

  // Initial registration
  bool RegisterFrame(const View::ptr_t &f) {
    assert(!f->name().empty());
    if (view_registry_.count(f->name()) > 0) {
      log_e("View %s already registered.", f->name());
      return false;
    }
    f->Prepare(this);
    view_registry_[f->name()] = f;
    return true;
  }

  void PopFrame() { remove_last_ = true; }

  void Draw();

private:
  /// At least 10px touch distance
  static const int16_t TOUCH_EPSILON = 10;

  enum TouchState { TS_NONE = 0, TS_TOUCH };

  struct RawEvent {
    tp_finger_t evt;
    bool touch;
    uint8_t fingers;

    // Only compare the main properties.
    bool operator==(const RawEvent &o) const {
      return fingers == o.fingers && touch == o.touch && evt.x == o.evt.x &&
             evt.y == o.evt.y;
    }

    bool operator!=(const RawEvent &o) const { return !(*this == o); }
  };

  void HandleTouchEvents();

  std::vector<View::ptr_t> view_stack_;

  std::vector<View::ptr_t> view_changes_;

  std::unordered_map<std::string, View::ptr_t> view_registry_;

  TouchState state_ = TS_NONE;

  TouchEvent event_;

  RawEvent last_raw_event_;

  bool remove_last_ = false;
};

#pragma once

#include <cstdint>
#include <memory>
#include <string>

#include "widget.hpp"
#include "widget_label.hpp"

/// A button is fundamentally a label with a specific width and potentially
/// background etc.
class WButton : public Label {
public:
  using ptr_t = std::shared_ptr<WButton>;

  WButton(int16_t x, int16_t y, int16_t width, int16_t height,
          const std::string &text)
      : Label(x, y, width, height, text) {}

  virtual ~WButton() {}

  static ptr_t Create(int16_t x, int16_t y, int16_t width, int16_t height,
                      const std::string &text) {
    const auto &p = std::make_shared<WButton>(x, y, width, height, text);
    return p;
  }

  virtual void Init() override;

  virtual void Draw() override;

  bool EventInside(int16_t x, int16_t y) const override;

  virtual void InternalEventHandler(TouchEvent evt) override;

protected:
  EventType last_event_ = EventType::NONE;
};
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
      : Label(x, y, width, height, text), downCanvas_(&M5.EPD) {}

  virtual ~WButton() {}

  static ptr_t Create(int16_t x, int16_t y, int16_t width, int16_t height,
                      const std::string &text) {
    const auto &p = std::make_shared<WButton>(x, y, width, height, text);
    p->NeedsOwnCanvas();
    return p;
  }

  /// When initialized, the button creates two canvas. The primary canvas
  /// behaves as expected  from the parent Label class, but in addition, it
  /// creates a secondary canvas that is only pushed when the button is pressed.
  virtual void Init() override;

  /// The button class overrides Draw to be able to switch between the two
  /// different UI states. For simplicity, the background of a button when
  /// pressed simply defaults to a black canvas.
  virtual bool Draw() override;

  bool EventInside(int16_t x, int16_t y) const override;

  virtual void InternalEventHandler(TouchEvent evt) override;

protected:
  EventType last_event_ = EventType::NONE;

  M5EPD_Canvas downCanvas_;
};
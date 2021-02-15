#pragma once
#include <string>
#include <memory>
#include <cstdint>

#include "widget.hpp"

class Label : public Widget {
public:

  enum HAlignment {
    LEFT, MIDDLE, RIGHT
  };

  enum VAlignment {
    TOP, CENTER, BOTTOM
  };

  using ptr_t = std::shared_ptr<Label>;

  Label(int16_t x, int16_t y, int16_t width, int16_t height, const std::string& text, int16_t size = 3):
    Widget(x, y, width, height), text_(text), text_size_(size) {}


  virtual ~Label() {}

  static ptr_t Create(int16_t x, int16_t y, int16_t width, int16_t height, const std::string& text, int16_t size = 3) {
    const auto& ptr = std::make_shared<Label>(x, y, width, height, text, size);
    return ptr;
  }

  virtual void Init() override;

  virtual void Draw() override;

  void VAlign(VAlignment a) {
    v_align_ = a;
  }

  void HAlign(HAlignment a) {
    h_align_ = a;
  }

protected:
  std::string text_;
  int16_t text_size_;

  HAlignment h_align_ = HAlignment::LEFT;
  VAlignment v_align_ = VAlignment::TOP;

private:
  uint8_t convert_alignment() const;
};
#pragma once
#include <M5EPD_Canvas.h>
#include <cstdint>

class Grayscale {
public:
  enum Value : int16_t {
    G0 = 0, // White
    G1,
    G2,
    G3,
    G4,
    G5,
    G6,
    G7,
    G8,
    G9,
    G10,
    G11,
    G12,
    G13,
    G14,
    G15 // Black
  };

  Grayscale() = default;
  constexpr Grayscale(Value val) : value_(val) {}

  operator Value() const { return value_; }
  explicit operator bool() = delete;

  constexpr bool operator==(Grayscale a) const { return value_ == a.value_; }
  constexpr bool operator!=(Grayscale a) const { return value_ != a.value_; }

  int16_t toInt() const { return static_cast<int16_t>(value_); }

  inline bool monochrome() const { return value_ == G0 || value_ == G15; }

private:
  Value value_;
};

class WidgetStyle {
public:
  enum Value : int16_t {
    NONE = 0,
    BORDER = 1,
    FILL = 2,
    FILL_W_BORDER = 4
  };
  WidgetStyle() = default;
  constexpr WidgetStyle(Value val) : value_(val) {}

  operator Value() const { return value_; }
  explicit operator bool() = delete;

  inline bool ShouldDraw(WidgetStyle::Value v) {
    return value_ & v;
  }

  constexpr bool operator==(WidgetStyle a) const { return value_ == a.value_; }
  constexpr bool operator==(WidgetStyle::Value a) const { return value_ == a; }
  constexpr bool operator!=(WidgetStyle a) const { return value_ != a.value_; }
  constexpr bool operator!=(WidgetStyle::Value a) const { return value_ != a; }

private:
  Value value_;
};

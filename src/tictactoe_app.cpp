#include <algorithm>

#include "tictactoe_app.hpp"

void TicTacToe::ResetBoard() {
  std::generate(std::begin(board_), std::end(board_),
                []() { return Field::NONE; });
}

TicTacToe::Field TicTacToe::get(int16_t id) const { return board_[id]; }

void TicTacToe::Set(int16_t id, Field f) { board_[id] = f; }

void TicTacToe::Prepare(WidgetContext *ctx) {
  auto field_size = (width_ - (2 * PADDING)) / 3;

  // Add all buttons to the field
  for (int16_t c = 0; c < 3; ++c) {
    for (int16_t r = 0; r < 3; ++r) {
      auto b = Widget::Create(PADDING + c * field_size,
                              PADDING + r * field_size, field_size, field_size);
      b->Style(WidgetStyle::BORDER);
      b->SetBorderStyle(BorderStyle::NORMAL);
      b->BorderWidth(3);
      b->BorderColor(Grayscale::GS_BLACK);
      b->Name("Field");
      AddWidget(b);
    }
  }
}

TicTacToe::TicTacToe(int16_t x, int16_t y, int16_t w, int16_t h)
    : Frame(x, y, w, h) {}
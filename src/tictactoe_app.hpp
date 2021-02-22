#pragma once

#include <array>
#include <memory>

#include "widgetlib.hpp"

/**
 * An Implementation of the Tic Tac Toe game. The screen is divided into two
 * main parts. The top part represents the game board and the bottom part shows
 * the current score.
 * */
class TicTacToe : public Frame {
public:
  using ptr_t = std::shared_ptr<TicTacToe>;
  // Virtual destructor needed
  virtual ~TicTacToe() {}

  TicTacToe(int16_t x, int16_t y, int16_t w, int16_t h);

  static ptr_t Create(int16_t x, int16_t y, int16_t w, int16_t h) {
    return std::make_shared<TicTacToe>(x, y, w, h);
  }

  /// Draw the following game board. Since the screen is taller than wide we can
  /// assume that it will allow us to have sufficient space below.
  ///
  ///  -----------width-----------------------
  ///  | 50px | Field | Field | Field | 50px |
  ///
  ///
  virtual void Prepare(WidgetContext *) override;

private:
  enum class Field { NONE, X, O };

  static const uint16_t PADDING = 50;

  void ResetBoard();

  Field get(int16_t id) const;
  void Set(int16_t id, Field f);

  int16_t score_x_ = 0;
  int16_t score_o_ = 0;

  std::array<Field, 9> board_;
};
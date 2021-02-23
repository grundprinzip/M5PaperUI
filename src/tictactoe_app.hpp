#pragma once

#include <array>
#include <memory>

#include "widgetlib.hpp"

/**
 * An Implementation of the Tic Tac Toe game. The screen is divided into two
 * main parts. The top part represents the game board and the bottom part shows
 * the current score.
 * */
class TicTacToe : public std::enable_shared_from_this<TicTacToe>, public Frame {
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

  virtual void Reset() override;

private:
  enum class Element : uint8_t { NONE = 0, X, O };

  static const uint16_t PADDING = 50;

  class Field : public Widget {
  public:
    Field(int16_t x, int16_t y, int16_t w, int16_t h);
    virtual void Init() override;
    void Reset() override;
    bool Draw() override;
    virtual void InternalEventHandler(TouchEvent evt) override;
    inline void Update(Element s) {
      state_ = s;
      view_dirty_ = true;
    }

  private:
    M5EPD_Canvas canvas_x_;
    M5EPD_Canvas canvas_o_;
    Element state_;
  };

  void ResetBoard();

  void Next();

  Element get(int16_t id) const;

  void Set(int16_t id, Element f);

  Element won() const;

  inline void NextPlayer() {
    current_player_ = current_player_ == Element::X ? Element::O : Element::X;
  }

  int16_t val_score_x_ = 0;
  int16_t val_score_o_ = 0;

  std::array<Element, 9> board_;

  Element current_player_ = Element::X;

  Label::ptr_t score_x_;

  Label::ptr_t score_o_;

  Label::ptr_t lbl_msg_;
  std::string msg_;
};
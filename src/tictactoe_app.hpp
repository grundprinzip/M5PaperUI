#pragma once

#include <array>
#include <memory>

#include "widgetlib.hpp"

/**
 * An Implementation of the Tic Tac Toe game. The screen is divided into two
 * main parts. The top part represents the game board and the bottom part shows
 * the current score.
 *
 * This game is a good show case for epaper displays as it highlights the need
 * to track display state at two levels. On the one hand, we don't want to
 * update the screen on every touch event, but only for some of the items, but
 * when a game is one and the screen is reset, we need to update the display
 * fully.
 *
 * To achieve that, this class provides a custom widget that uses two additional
 * canvas to display state.
 *
 * Generally, this frame does not use it's own update mode for the display, but
 * simply pushes the FULL_UPDATE mode of the parent implementation, this avoids
 * additional screen flickering.
 *
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

  /// Reset the state of the frame. This will in turn reset the message of the
  /// winning user and force redrawing the screen.
  virtual void Reset() override;

private:
  /// A simple enum that is used in different places to distinguish the state of
  /// the game field and the players.
  enum class Element : uint8_t { NONE = 0, X, O };

  /// The padding used on the left and right of the screen.
  static const uint16_t PADDING = 50;

  /// The `Field` is the custom widget implementation that represents a game
  /// field on the screen. It leverages the canvas of the parent frame to draw
  /// it's initial outline and on subsequent state changes pushes a custom
  /// canvas for the different players.
  class Field : public Widget {
  public:
    Field(int16_t x, int16_t y, int16_t w, int16_t h);
    virtual void Init() override;
    void Reset() override;
    bool Draw() override;
    virtual void InternalEventHandler(TouchEvent evt) override;
    /// When Update() is called, it will set the state of the widget to the one
    /// passed in and in addition require re-drawing the widget.
    inline void Update(Element s) {
      state_ = s;
      view_dirty_ = true;
    }

  private:
    /// Pre-drawn canvas for the X player.
    M5EPD_Canvas canvas_x_;
    /// Pre-drawn canvas for the O player.
    M5EPD_Canvas canvas_o_;
    /// State that can be accessed to check the state of the game.
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
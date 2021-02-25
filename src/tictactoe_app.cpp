#include <algorithm>
#include <sstream>

#include "tictactoe_app.hpp"

void TicTacToe::ResetBoard() {
  std::generate(std::begin(board_), std::end(board_),
                []() { return Element::NONE; });
}

void TicTacToe::Reset() {
  // Randomize first player

  // Reset board
  ResetBoard();

  msg_ = "";
  lbl_msg_->Text(msg_);
  lbl_msg_->Init();
  lbl_msg_->UpdateMode(UPDATE_MODE_NONE);
  lbl_msg_->Reset();
  score_o_->UpdateMode(UPDATE_MODE_NONE);
  score_o_->Reset();
  score_x_->UpdateMode(UPDATE_MODE_NONE);
  score_x_->Reset();

  RequireRedraw();
}

TicTacToe::Element TicTacToe::get(int16_t id) const { return board_[id]; }

void TicTacToe::Set(int16_t id, Element f) {
  if (board_[id] != Element::NONE)
    return;

  if (board_[id] == f)
    return;
  board_[id] = f;

  log_d("Check for win? %d", won());
}

TicTacToe::Element TicTacToe::won() const {
  // There are 8 solutions.
  // 0 1 2
  // 3 4 5
  // 6 7 8
  // Check all columns
  for (int16_t c = 0; c < 3; ++c) {
    if (get(c) == get(c + 3) && get(c) == get(c + 6)) {
      return get(c);
    }
  }
  // Check all the rows
  for (int16_t r = 0; r < 3; ++r) {
    if (get(r * 3) == get(r * 3 + 1) && get(r * 3) == get(r * 3 + 2)) {
      return get(r * 3);
    }
  }

  // Check the diagonals
  if (get(0) == get(4) && get(0) == get(8)) {
    return get(0);
  }

  if (get(2) == get(4) && get(2) == get(6)) {
    return get(2);
  }

  return Element::NONE;
}

void TicTacToe::Prepare(WidgetContext *ctx) {
  ResetBoard();
  auto field_size = (width_ - (2 * PADDING)) / 3;

  // Add all buttons to the field
  auto self = shared_from_this();
  for (int16_t c = 0; c < 3; ++c) {
    for (int16_t r = 0; r < 3; ++r) {
      auto b = std::make_shared<Field>(PADDING + c * field_size,
                                       PADDING + r * field_size, field_size,
                                       field_size);
      b->Name("Field");
      b->RegisterHandler([self, this, c, r](TouchEvent evt, Widget *w) {
        // Only react on Touch up, if the game is not won and if the field is
        // empty.
        if (evt.type != EventType::TOUCH_UP || self->won() != Element::NONE ||
            self->get(r * 3 + c) != Element::NONE)
          return;
        log_d("Pushing %d %d", c, r);
        // This is no dynamic cast because of of no-rtti
        Field *field = reinterpret_cast<Field *>(w);
        self->Set(r * 3 + c, self->current_player_);
        field->Update(self->current_player_);
        self->Next();
        if (self->won() == Element::NONE) {
          self->NextPlayer();
        }
      });
      AddWidget(b);
    }
  }

  int16_t y_offset = 2 * PADDING + 3 * field_size;
  std::string pl = current_player_ == Element::X ? "X" : "O";
  std::ostringstream buf;
  buf << "Next Player: " << pl;
  AddWidget(Label::Create(50, y_offset, 300, 30, buf.str().c_str()));

  // Drawing the score for the X player.
  AddWidget(Label::Create(50, y_offset + 50, 300, 30, "Player X Score: "));
  buf = std::ostringstream();
  buf << val_score_x_;
  score_x_ = Label::Create(300, y_offset + 50, 20, 30, buf.str().c_str());
  score_x_->NeedsOwnCanvas();
  score_x_->Style(WidgetStyle::FILL);
  score_x_->BackgroundColor(Grayscale::GS_WHITE);
  score_x_->UpdateMode(UPDATE_MODE_NONE);
  score_x_->SetChildUpdateMode(true);
  AddWidget(score_x_);

  // Drawing score for the O player.
  AddWidget(Label::Create(50, y_offset + 100, 300, 30, "Player O Score: "));
  buf = std::ostringstream();
  buf << val_score_o_;
  score_o_ = Label::Create(300, y_offset + 100, 20, 30, buf.str().c_str());
  score_o_->NeedsOwnCanvas();
  score_o_->UpdateMode(UPDATE_MODE_NONE);
  score_o_->SetChildUpdateMode(true);
  score_o_->Style(WidgetStyle::FILL);
  score_o_->BackgroundColor(Grayscale::GS_WHITE);
  AddWidget(score_o_);

  lbl_msg_ = Label::Create(50, y_offset + 150, 300, 60, msg_.c_str(), 30);
  lbl_msg_->NeedsOwnCanvas();
  lbl_msg_->UpdateMode(UPDATE_MODE_NONE);
  lbl_msg_->SetChildUpdateMode(true);
  lbl_msg_->Style(WidgetStyle::FILL);
  lbl_msg_->BackgroundColor(Grayscale::GS_WHITE);
  AddWidget(lbl_msg_);

  auto btn = WButton::Create(200, y_offset + 200, 150, 50, "New Game");
  btn->TextSize(26);
  btn->Style(WidgetStyle::BORDER);
  btn->BorderWidth(1);
  btn->BorderColor(Grayscale::GS_BLACK);
  btn->HAlign(Label::MIDDLE);
  btn->VAlign(Label::CENTER);
  btn->RegisterHandler([self, this](TouchEvent e, Widget *) { self->Reset(); });
  AddWidget(btn);
}

void TicTacToe::Next() {
  Element end = won();
  if (end != Element::NONE) {

    std::ostringstream buf;

    if (end == Element::X) {
      val_score_x_++;
      msg_ = "Player X won!";
      buf << val_score_x_;
      score_x_->Text(buf.str());
      score_x_->Init();
      score_x_->UpdateMode(UPDATE_MODE_GC16);
      score_x_->Reset();
    } else {
      val_score_o_++;
      msg_ = "Player O won!";
      buf << val_score_o_;
      score_o_->Text(buf.str());
      score_o_->Init();
      score_o_->UpdateMode(UPDATE_MODE_GC16);
      score_o_->Reset();
    }
    lbl_msg_->Text(msg_);
    lbl_msg_->Init();
    lbl_msg_->UpdateMode(UPDATE_MODE_GC16);
    lbl_msg_->Reset();
  }
}

TicTacToe::TicTacToe(int16_t x, int16_t y, int16_t w, int16_t h)
    : Frame(x, y, w, h) {
  update_mode_ = UPDATE_MODE_NONE;
}

TicTacToe::Field::Field(int16_t x, int16_t y, int16_t w, int16_t h)
    : Widget(x, y, w, h), canvas_x_(&M5.EPD), canvas_o_(&M5.EPD) {
  BorderWidth(3);
  Style(WidgetStyle::BORDER);
  BorderColor(Grayscale::GS_BLACK);
  // has_own_canvas_ = true;
  state_ = Element::NONE;
}
void TicTacToe::Field::Init() {
  Widget::Init();

  int16_t padding = 10;
  canvas_o_.createCanvas(width_ - 2 * border_width_,
                         height_ - 2 * border_width_);
  canvas_x_.createCanvas(width_ - 2 * border_width_,
                         height_ - 2 * border_width_);

  // For the x canvas draw an X
  canvas_x_.drawLine(padding, padding, width_ - padding, height_ - padding, 3,
                     Grayscale::GS_BLACK);
  canvas_x_.drawLine(padding, height_ - padding, width_ - padding, padding, 3,
                     Grayscale::GS_BLACK);

  // For the o canvas draw the circle
  int16_t radius = (width_ - 2 * padding) / 2;
  canvas_o_.drawCircle(width_ / 2, height_ / 2, radius, Grayscale::GS_BLACK);
  canvas_o_.drawCircle(width_ / 2, height_ / 2, radius - 1,
                       Grayscale::GS_BLACK);
  canvas_o_.drawCircle(width_ / 2, height_ / 2, radius - 2,
                       Grayscale::GS_BLACK);
}

void TicTacToe::Field::Reset() {
  Widget::Reset();
  state_ = Element::NONE;
}

bool TicTacToe::Field::Draw() {
  if (view_dirty_) {
    log_d("Drawing canvas of field with %d", state_);
    if (state_ == Element::NONE) {
      // canvas_->pushCanvas(x_offset_, y_offset_, UPDATE_MODE_NONE);
    } else if (state_ == Element::X) {
      auto dim = parent_->dimension();
      canvas_x_.pushCanvas(dim.x + x_ + border_width_,
                           dim.y + y_ + border_width_, UPDATE_MODE_DU4);
    } else {
      auto dim = parent_->dimension();
      canvas_o_.pushCanvas(dim.x + x_ + border_width_,
                           dim.y + y_ + border_width_, UPDATE_MODE_DU4);
    }
    view_dirty_ = false;
  }
  return view_dirty_;
}

void TicTacToe::Field::InternalEventHandler(TouchEvent evt) {}
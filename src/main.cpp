#include <Arduino.h>
#include <M5EPD.h>

#include "widgetlib.hpp"

WidgetContext::ptr_t ctx;

void setup() {

  M5.begin();
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);
  M5.RTC.begin();

  // put your setup code here, to run once:
  ctx = std::make_shared<WidgetContext>(&(M5.EPD));
  auto frame = Frame::Create(10, 50, 300, 200);
  frame->UpdateMode(UPDATE_MODE_GC16);

  //auto w = Widget::Create(0, 0, 100, 100);
  auto w = Label::Create(0, 0, 100, 100, "Hello World there is");

    //w->BackgroundColor(Grayscale::G5);
  //w->BorderColor(Grayscale::G15);
  //w->Style(WidgetStyle::FILL);

  frame->AddWidget(w);
  ctx->AddFrame(frame);

}

void loop() {
  // put your main code here, to run repeatedly:
  WidgetMainLoop(ctx);
}
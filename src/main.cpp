#include <Arduino.h>
#include <M5EPD.h>

#include "binaryttf.h"
#include "image_resources.h"
#include "main_window.hpp"
#include "widgetlib.hpp"

WidgetContext::ptr_t ctx;

// Prepare some stuff.
void initializeSystem() {
  log_d("Prepare SysInit");

  M5.begin();
  M5.EPD.SetRotation(90);
  M5.TP.SetRotation(90);
  M5.EPD.Clear(true);
  M5.RTC.begin();

  // Loading Font
  M5EPD_Canvas font(&M5.EPD);
  font.loadFont(binaryttf, sizeof(binaryttf));
  font.createRender(26, 128);
  font.createRender(20, 128);

  // Pre-Loading Images
  /*const uint8_t* preload[] { PAINTBRUSH, SETTINGS_128_128 };
  M5EPD_Canvas pld(&M5.EPD);
  pld.createCanvas(128,128);
  for (int i=0; i < 2; ++i) {
    pld.pushImage(0,0, 128, 128, preload[i]);
    pld.pushCanvas(220, 548, UPDATE_MODE_DU4 );
  }*/

  // Loading fonts

  log_d("Done SysInit");
}

void setup() {

  initializeSystem();
  // put your setup code here, to run once:
  ctx = std::make_shared<WidgetContext>();
  auto win = std::make_shared<MainWindow>();
  win->Start(ctx);
}

void loop() {
  // put your main code here, to run repeatedly:
  ctx->Draw();
}
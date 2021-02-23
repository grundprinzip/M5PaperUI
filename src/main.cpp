#include <Arduino.h>
#include <M5EPD.h>

#include "binaryttf.h"
#include "image_resources.h"
#include "main_window.hpp"
#include "settings.hpp"
#include "widgetlib.hpp"

WidgetContext::ptr_t ctx;

Settings global_settings;

// Prepare some stuff.
void initializeSystem() {
  log_d("Prepare SysInit");

  M5.begin();
  M5.EPD.SetRotation(90);
  M5.TP.SetRotation(90);
  M5.EPD.Clear(true);
  M5.RTC.begin();

  // NVS
  Settings::Stats();

  // Loading Font
  M5EPD_Canvas font(&M5.EPD);
  font.loadFont(binaryttf, sizeof(binaryttf));
  font.createRender(26, 128);
  font.createRender(20, 128);
  font.createRender(30, 128);

  global_settings = Settings::Load();
  log_d("Done SysInit");
}

void setup() {

  initializeSystem();
  // put your setup code here, to run once:
  ctx = std::make_shared<WidgetContext>();
  auto win = std::make_shared<MainWindow>(&global_settings);
  win->Start(ctx);
}

void loop() {
  // put your main code here, to run repeatedly:
  ctx->Draw();
}
#pragma once
#include <nvs.h>
#include <string>

/**
 * This class is a small wrapper around the NVS libary to persist the settings
 * of our application.
 */
class Settings {
public:
  static Settings Load();

  static nvs_stats_t Stats();

  esp_err_t Save();

  void SetSSID(const std::string &s) { ssid_ = s; }
  std::string ssid() const { return ssid_; }

  void SetWifiPassword(const std::string &p) { wifi_password_ = p; }
  std::string wifi_password() const { return wifi_password_; }

private:
  std::string ssid_;
  std::string wifi_password_;
};
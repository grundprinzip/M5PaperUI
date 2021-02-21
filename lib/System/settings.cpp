#include "settings.hpp"
#include "esp32-hal-log.h"

esp_err_t __err__;
#define CHECK_ERROR(val, h)                                                    \
  {                                                                            \
    __err__ = (h);                                                             \
    if (__err__ != ESP_OK) {                                                   \
      log_e("Check err %d", __err__);                                          \
      return val;                                                              \
    }                                                                          \
  }

#define KEY_SSID "ssid"
#define KEY_PASSWORD "password"

Settings Settings::Load() {
  Settings s;
  nvs_handle nvs;

  CHECK_ERROR(s, nvs_open("Settings", NVS_READONLY, &nvs));
  char *buf;
  size_t len;
  CHECK_ERROR(s, nvs_get_str(nvs, KEY_SSID, buf, &len))
  s.ssid_ = std::string(buf, len);

  CHECK_ERROR(s, nvs_get_str(nvs, KEY_PASSWORD, buf, &len))
  s.wifi_password_ = std::string(buf, len);

  nvs_close(nvs);
  return s;
}

nvs_stats_t Settings::Stats() {
  nvs_stats_t stats;
  nvs_get_stats("nvs", &stats);
  log_d("NVS Stats: %d %d %d %d", stats.used_entries, stats.free_entries,
        stats.total_entries, stats.namespace_count);
  return stats;
}

esp_err_t Settings::Save() {
  nvs_handle nvs;
  CHECK_ERROR(__err__, nvs_open("Settings", NVS_READONLY, &nvs));
  CHECK_ERROR(__err__, nvs_set_str(nvs, KEY_SSID, ssid_.c_str()));
  CHECK_ERROR(__err__, nvs_set_str(nvs, KEY_PASSWORD, wifi_password_.c_str()));

  nvs_close(nvs);
  return ESP_OK;
}

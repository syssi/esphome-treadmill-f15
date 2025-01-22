#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/number/number.h"

#ifdef USE_ESP32

#include <esp_gattc_api.h>

namespace esphome {
namespace treadmill_f15 {

namespace espbt = esphome::esp32_ble_tracker;

class TreadmillF15 : public esphome::ble_client::BLEClientNode, public PollingComponent {
 public:
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_running_binary_sensor(binary_sensor::BinarySensor *running_binary_sensor) {
    running_binary_sensor_ = running_binary_sensor;
  }
  void set_starting_binary_sensor(binary_sensor::BinarySensor *starting_binary_sensor) {
    starting_binary_sensor_ = starting_binary_sensor;
  }
  void set_stopping_binary_sensor(binary_sensor::BinarySensor *stopping_binary_sensor) {
    stopping_binary_sensor_ = stopping_binary_sensor;
  }
  void set_stopped_binary_sensor(binary_sensor::BinarySensor *stopped_binary_sensor) {
    stopped_binary_sensor_ = stopped_binary_sensor;
  }

  void set_operation_mode_id_sensor(sensor::Sensor *operation_mode_id_sensor) {
    operation_mode_id_sensor_ = operation_mode_id_sensor;
  }
  void set_speed_sensor(sensor::Sensor *speed_sensor) { speed_sensor_ = speed_sensor; }
  void set_incline_sensor(sensor::Sensor *incline_sensor) { incline_sensor_ = incline_sensor; }
  void set_elapsed_time_sensor(sensor::Sensor *elapsed_time_sensor) { elapsed_time_sensor_ = elapsed_time_sensor; }
  void set_distance_sensor(sensor::Sensor *distance_sensor) { distance_sensor_ = distance_sensor; }
  void set_calories_sensor(sensor::Sensor *calories_sensor) { calories_sensor_ = calories_sensor; }
  void set_heart_rate_sensor(sensor::Sensor *heart_rate_sensor) { heart_rate_sensor_ = heart_rate_sensor; }

  void set_elapsed_time_formatted_text_sensor(text_sensor::TextSensor *elapsed_time_formatted_text_sensor) {
    elapsed_time_formatted_text_sensor_ = elapsed_time_formatted_text_sensor;
  }
  void set_operation_mode_text_sensor(text_sensor::TextSensor *operation_mode_text_sensor) {
    operation_mode_text_sensor_ = operation_mode_text_sensor;
  }

  void set_speed_number(number::Number *speed_number) { speed_number_ = speed_number; }
  void set_incline_number(number::Number *incline_number) { incline_number_ = incline_number; }

  float get_speed_number_state() const { return speed_number_ ? speed_number_->state : 0.8f; }
  float get_incline_number_state() const { return incline_number_ ? incline_number_->state : 0.0f; }

  void on_treadmill_f15_data(const std::vector<uint8_t> &data);
  bool send_command(uint16_t function);
  bool send_command(uint8_t register_addr, uint8_t payload);
  bool send_command(uint8_t register_addr, const std::vector<uint8_t> &payload);
  bool send_raw_command(const std::vector<uint8_t> &command);
  bool send_speed_incline_command(float speed, float incline);

 protected:
  binary_sensor::BinarySensor *running_binary_sensor_;
  binary_sensor::BinarySensor *starting_binary_sensor_;
  binary_sensor::BinarySensor *stopping_binary_sensor_;
  binary_sensor::BinarySensor *stopped_binary_sensor_;

  sensor::Sensor *operation_mode_id_sensor_;
  sensor::Sensor *speed_sensor_;
  sensor::Sensor *incline_sensor_;
  sensor::Sensor *elapsed_time_sensor_;
  sensor::Sensor *distance_sensor_;
  sensor::Sensor *calories_sensor_;
  sensor::Sensor *heart_rate_sensor_;

  number::Number *speed_number_;
  number::Number *incline_number_;

  text_sensor::TextSensor *elapsed_time_formatted_text_sensor_;
  text_sensor::TextSensor *operation_mode_text_sensor_;

  uint16_t char_notify_handle_;
  uint16_t char_command_handle_;

  void decode_status_data_(const std::vector<uint8_t> &data);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(number::Number *number, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);

  std::string format_total_runtime_(const uint16_t value) {
    int seconds = (int) value;
    int years = seconds / (24 * 3600 * 365);
    seconds = seconds % (24 * 3600 * 365);
    int days = seconds / (24 * 3600);
    seconds = seconds % (24 * 3600);
    int hours = seconds / 3600;
    return (years ? to_string(years) + "y " : "") + (days ? to_string(days) + "d " : "") +
           (hours ? to_string(hours) + "h" : "");
  }
};

}  // namespace treadmill_f15
}  // namespace esphome

#endif

#include "treadmill_f15.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace treadmill_f15 {

static const char *const TAG = "treadmill_f15";

static const uint16_t TREADMILL_BMS_SERVICE_UUID = 0xFFF0;

static const uint16_t TREADMILL_BMS_NOTIFY_CHARACTERISTIC_UUID = 0xFFF1;
static const uint16_t TREADMILL_BMS_CONTROL_CHARACTERISTIC_UUID = 0xFFF2;

static const uint8_t MAX_RESPONSE_SIZE = 108;

uint8_t crc(const uint8_t data[], const uint8_t len) {
  uint8_t crc = 0;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
  }
  return crc;
}

void TreadmillF15::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                       esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT: {
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT: {
      this->node_state = espbt::ClientState::IDLE;

      // this->publish_state_(this->voltage_sensor_, NAN);

      if (this->char_notify_handle_ != 0) {
        auto status = esp_ble_gattc_unregister_for_notify(this->parent()->get_gattc_if(),
                                                          this->parent()->get_remote_bda(), this->char_notify_handle_);
        if (status) {
          ESP_LOGW(TAG, "esp_ble_gattc_unregister_for_notify failed, status=%d", status);
        }
      }
      this->char_notify_handle_ = 0;
      this->char_command_handle_ = 0;

      break;
    }
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      auto *char_notify =
          this->parent_->get_characteristic(TREADMILL_BMS_SERVICE_UUID, TREADMILL_BMS_NOTIFY_CHARACTERISTIC_UUID);
      if (char_notify == nullptr) {
        ESP_LOGE(TAG, "[%s] No notify service found at device, not an Treadmill BMS..?",
                 this->parent_->address_str().c_str());
        break;
      }

      //  ESP_GATTC_SEARCH_CMPL_EVT
      //  Service UUID: 0x1800
      //   start_handle: 0x1  end_handle: 0x3
      //  Service UUID: 0x1801
      //   start_handle: 0x4  end_handle: 0x6
      //  Service UUID: 0x180A
      //   start_handle: 0x7  end_handle: 0xf
      //  Service UUID: 0xFFF0
      //   start_handle: 0x10  end_handle: 0x18
      //  Connected
      //   characteristic 0xFFF1, handle 0x12, properties 0x10
      //   characteristic 0xFFF2, handle 0x15, properties 0x4
      //   characteristic 0xFFF3, handle 0x17, properties 0x14
      //  gattc_event_handler: event=18 gattc_if=3
      //  cfg_mtu status 0, mtu 515
      //  gattc_event_handler: event=38 gattc_if=3
      //  ESP_GATTC_REG_FOR_NOTIFY_EVT

      this->char_notify_handle_ = char_notify->handle;

      auto status = esp_ble_gattc_register_for_notify(this->parent()->get_gattc_if(), this->parent()->get_remote_bda(),
                                                      char_notify->handle);
      if (status) {
        ESP_LOGW(TAG, "esp_ble_gattc_register_for_notify failed, status=%d", status);
      }

      auto *char_command =
          this->parent_->get_characteristic(TREADMILL_BMS_SERVICE_UUID, TREADMILL_BMS_CONTROL_CHARACTERISTIC_UUID);
      if (char_command == nullptr) {
        ESP_LOGE(TAG, "[%s] No control service found at device, not an Treadmill BMS..?",
                 this->parent_->address_str().c_str());
        break;
      }
      this->char_command_handle_ = char_command->handle;
      break;
    }
    case ESP_GATTC_REG_FOR_NOTIFY_EVT: {
      this->node_state = espbt::ClientState::ESTABLISHED;

      this->send_command(0x00);
      break;
    }
    case ESP_GATTC_NOTIFY_EVT: {
      ESP_LOGV(TAG, "Notification received (handle 0x%02X): %s", param->notify.handle,
               format_hex_pretty(param->notify.value, param->notify.value_len).c_str());

      std::vector<uint8_t> data(param->notify.value, param->notify.value + param->notify.value_len);

      this->on_treadmill_f15_data(data);
      break;
    }
    default:
      break;
  }
}

void TreadmillF15::update() {
  if (this->node_state != espbt::ClientState::ESTABLISHED) {
    ESP_LOGW(TAG, "[%s] Not connected", this->parent_->address_str().c_str());
    return;
  }

  this->send_command(0x00);
}

void TreadmillF15::on_treadmill_f15_data(const std::vector<uint8_t> &data) {
  if (data[0] != 0x02 || data.back() != 0x03 || data.size() > MAX_RESPONSE_SIZE) {
    ESP_LOGW(TAG, "Invalid response received: %s", format_hex_pretty(&data.front(), data.size()).c_str());
    return;
  }

  uint8_t computed_crc = crc(data.data() + 1, data.size() - 3);
  uint8_t remote_crc = data[data.size() - 2];
  if (computed_crc != remote_crc) {
    ESP_LOGW(TAG, "CRC Check failed! 0x%02X != 0x%02X", computed_crc, remote_crc);
    return;
  }

  uint8_t frame_type = data[2];

  switch (frame_type) {
    case 0x00:  // stopped
    case 0x02:  // starting
    case 0x03:  // running
    case 0x04:  // stopping
      this->decode_status_data_(data);
      break;
    default:
      ESP_LOGW(TAG, "Unhandled response received (frame_type 0x%02X): %s", frame_type,
               format_hex_pretty(&data.front(), data.size()).c_str());
  }
}

void TreadmillF15::decode_status_data_(const std::vector<uint8_t> &data) {
  auto treadmill_get_16bit = [&](size_t i) -> uint16_t {
    return (uint16_t(data[i + 0]) << 0) | (uint16_t(data[i + 1]) << 8);
  };

  ESP_LOGI(TAG, "Status frame received");
  ESP_LOGD(TAG, "  %s", format_hex_pretty(&data.front(), data.size()).c_str());

  if (data.size() < 5) {
    ESP_LOGW(TAG, "Invalid status frame length: %d", data.size());
    return;
  }

  // Byte Len Payload              Description                      Unit  Precision
  //  0    1  0x02                 Start of frame
  //  1    1  0x51
  //  2    1  0x03                 Operation mode
  this->publish_state_(this->starting_binary_sensor_, data[2] == 0x02);
  this->publish_state_(this->running_binary_sensor_, data[2] == 0x03);
  this->publish_state_(this->stopping_binary_sensor_, data[2] == 0x04);
  this->publish_state_(this->stopped_binary_sensor_, data[2] == 0x00);
  this->publish_state_(this->operation_mode_id_sensor_, data[2] * 1.0f);
  this->publish_state_(this->operation_mode_text_sensor_, data[2] == 0x00   ? "Stopped"
                                                          : data[2] == 0x02 ? "Starting"
                                                          : data[2] == 0x03 ? "Running"
                                                          : data[2] == 0x04 ? "Stopping"
                                                                            : "Unknown");

  if (data.size() < 17) {
    this->publish_state_(this->speed_sensor_, 0.0f);
    this->publish_state_(this->heart_rate_sensor_, 0.0f);
    return;
  }

  //  3    2  0x08 0x00                 Speed
  ESP_LOGVV(TAG, "Speed:        %.1f km/h", data[3] * 0.1f);
  this->publish_state_(this->speed_sensor_, data[3] * 0.1f);

  ESP_LOGVV(TAG, "Incline:      %d %%", data[4]);
  this->publish_state_(this->incline_sensor_, data[4] * 0.1f);

  //  5    2  0x55 0x02            Elapsed time
  ESP_LOGVV(TAG, "Elapsed time: %d s", treadmill_get_16bit(5));
  this->publish_state_(this->elapsed_time_sensor_, treadmill_get_16bit(5) * 1.0f);
  this->publish_state_(this->elapsed_time_formatted_text_sensor_, format_total_runtime_(treadmill_get_16bit(5)));

  //  7    2  0x84 0x00            Distance
  ESP_LOGVV(TAG, "Distance:     %.3f km", treadmill_get_16bit(7) * 0.001f);
  this->publish_state_(this->distance_sensor_, treadmill_get_16bit(7) * 0.001f);

  //  9    2  0x34 0x00            Calories
  ESP_LOGVV(TAG, "Calories:     %.1f cal", treadmill_get_16bit(9) * 0.1f);
  this->publish_state_(this->calories_sensor_, treadmill_get_16bit(9) * 0.1f);

  //  11   2  0x00 0x00
  //  13   2  0x00 0x00            Pulse
  ESP_LOGVV(TAG, "Heart rate:   %d bpm", treadmill_get_16bit(13));
  this->publish_state_(this->heart_rate_sensor_, treadmill_get_16bit(13) * 1.0f);

  //  15   1  0xBD                  Checksum
  //  16   1  0x03                  End of frame
}

void TreadmillF15::dump_config() {  // NOLINT(google-readability-function-size,readability-function-size)
  ESP_LOGCONFIG(TAG, "TreadmillF15:");

  LOG_BINARY_SENSOR("", "Running", this->running_binary_sensor_);
  LOG_BINARY_SENSOR("", "Stopping", this->stopping_binary_sensor_);
  LOG_BINARY_SENSOR("", "Stopped", this->stopped_binary_sensor_);

  LOG_SENSOR("", "Operation Mode ID", this->operation_mode_id_sensor_);
  LOG_SENSOR("", "Speed", this->speed_sensor_);
  LOG_SENSOR("", "Elapsed time", this->elapsed_time_sensor_);
  LOG_SENSOR("", "Distance", this->distance_sensor_);
  LOG_SENSOR("", "Calories", this->calories_sensor_);
  LOG_SENSOR("", "Heart rate", this->heart_rate_sensor_);

  LOG_TEXT_SENSOR("", "Elapsed time formatted", this->elapsed_time_formatted_text_sensor_);
  LOG_TEXT_SENSOR("", "Operation mode", this->operation_mode_text_sensor_);
}

void TreadmillF15::publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state) {
  if (binary_sensor == nullptr)
    return;

  binary_sensor->publish_state(state);
}

void TreadmillF15::publish_state_(sensor::Sensor *sensor, float value) {
  if (sensor == nullptr)
    return;

  sensor->publish_state(value);
}

void TreadmillF15::publish_state_(switch_::Switch *obj, const bool &state) {
  if (obj == nullptr)
    return;

  obj->publish_state(state);
}

void TreadmillF15::publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state) {
  if (text_sensor == nullptr)
    return;

  text_sensor->publish_state(state);
}

bool TreadmillF15::send_command(uint16_t function) {
  uint8_t frame[4];

  frame[0] = 0x02;
  frame[1] = 0x51;
  frame[2] = 0x51;
  frame[3] = 0x03;

  ESP_LOGD(TAG, "Send command (handle 0x%02X): %s", this->char_command_handle_,
           format_hex_pretty(frame, sizeof(frame)).c_str());

  auto status =
      esp_ble_gattc_write_char(this->parent_->get_gattc_if(), this->parent_->get_conn_id(), this->char_command_handle_,
                               sizeof(frame), frame, ESP_GATT_WRITE_TYPE_NO_RSP, ESP_GATT_AUTH_REQ_NONE);

  if (status) {
    ESP_LOGW(TAG, "[%s] esp_ble_gattc_write_char failed, status=%d", this->parent_->address_str().c_str(), status);
  }

  return (status == 0);
}

}  // namespace treadmill_f15
}  // namespace esphome

#include "treadmill_number.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace treadmill_f15 {

static const char *const TAG = "treadmill_f15.number";

void TreadmillNumber::setup() { this->publish_state(this->initial_value_); }

void TreadmillNumber::dump_config() {
  ESP_LOGCONFIG(TAG, "TreadmillF15 Number");
  ESP_LOGCONFIG(TAG, "  Command Type: %s", this->command_type_.c_str());
  ESP_LOGCONFIG(TAG, "  Holding Register: 0x%02X", this->holding_register_);
  LOG_NUMBER("", "TreadmillF15 Number", this);
}

void TreadmillNumber::control(float value) {
  if (this->command_type_ == "speed") {
    ESP_LOGI(TAG, "Setting treadmill speed to %.1f km/h", value);

    // Send combined speed/incline command with current incline value
    if (this->parent_->send_speed_incline_command(value, this->parent_->get_incline_number_state())) {
      this->publish_state(value);
    }
  } else if (this->command_type_ == "incline") {
    ESP_LOGI(TAG, "Setting treadmill incline to %.1f%%", value);

    // Send combined speed/incline command with current speed value
    if (this->parent_->send_speed_incline_command(this->parent_->get_speed_number_state(), value)) {
      this->publish_state(value);
    }
  } else {
    ESP_LOGW(TAG, "Unknown command type: %s", this->command_type_.c_str());
  }
}

}  // namespace treadmill_f15
}  // namespace esphome

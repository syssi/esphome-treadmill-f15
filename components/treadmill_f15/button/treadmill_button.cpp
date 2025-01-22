#include "treadmill_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace treadmill_f15 {

static const char *const TAG = "treadmill_f15.button";

void TreadmillButton::dump_config() {
  ESP_LOGCONFIG(TAG, "TreadmillF15 Button");
  ESP_LOGCONFIG(TAG, "  Button Type: %s", this->button_type_.c_str());
  ESP_LOGCONFIG(TAG, "  Register: 0x%02X", this->register_address_);
  ESP_LOGCONFIG(TAG, "  Payload Length: %d bytes", this->command_payload_.size());
  LOG_BUTTON("", "TreadmillF15 Button", this);
}

void TreadmillButton::press_action() {
  ESP_LOGI(TAG, "Sending %s command", this->button_type_.c_str());

  if (this->command_payload_.size() == 1) {
    this->parent_->send_command(this->register_address_, this->command_payload_[0]);
  } else if (this->command_payload_.size() > 1) {
    this->parent_->send_command(this->register_address_, this->command_payload_);
  } else {
    ESP_LOGW(TAG, "Empty payload for button type: %s", this->button_type_.c_str());
    return;
  }

  if (this->button_type_ == "start") {
    this->set_timeout(6000, [this]() {
      ESP_LOGI(TAG, "Sending speed/incline command after start");
      this->parent_->send_speed_incline_command(this->parent_->get_speed_number_state(),
                                                this->parent_->get_incline_number_state());
    });
  }
}

}  // namespace treadmill_f15
}  // namespace esphome

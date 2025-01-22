#include "treadmill_switch.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace treadmill_f15 {

static const char *const TAG = "treadmill_f15.switch";

void TreadmillSwitch::dump_config() { LOG_SWITCH("", "TreadmillF15 Switch", this); }
void TreadmillSwitch::write_state(bool state) {
  if (this->parent_->send_command(state ? this->command_turn_on_ : this->command_turn_off_)) {
    this->publish_state(state);
  }
}

}  // namespace treadmill_f15
}  // namespace esphome

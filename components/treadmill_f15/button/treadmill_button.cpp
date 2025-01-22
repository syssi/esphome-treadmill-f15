#include "treadmill_button.h"
#include "esphome/core/log.h"
#include "esphome/core/application.h"

namespace esphome {
namespace treadmill_f15 {

static const char *const TAG = "treadmill_f15.button";

void TreadmillButton::dump_config() { LOG_BUTTON("", "TreadmillF15 Button", this); }
void TreadmillButton::press_action() { this->parent_->send_command(this->holding_register_); }

}  // namespace treadmill_f15
}  // namespace esphome

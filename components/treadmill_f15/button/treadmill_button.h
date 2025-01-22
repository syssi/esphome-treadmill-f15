#pragma once

#include "../treadmill_f15.h"
#include "esphome/core/component.h"
#include "esphome/components/button/button.h"

namespace esphome {
namespace treadmill_f15 {

class TreadmillF15;
class TreadmillButton : public button::Button, public Component {
 public:
  void set_parent(TreadmillF15 *parent) { this->parent_ = parent; };
  void set_holding_register(uint16_t holding_register) { this->holding_register_ = holding_register; };
  void dump_config() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void press_action() override;
  TreadmillF15 *parent_;
  uint16_t holding_register_;
};

}  // namespace treadmill_f15
}  // namespace esphome

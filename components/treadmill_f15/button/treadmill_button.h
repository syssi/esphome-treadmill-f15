#pragma once

#include "../treadmill_f15.h"
#include "esphome/core/component.h"
#include "esphome/components/button/button.h"
#include <vector>

namespace esphome {
namespace treadmill_f15 {

class TreadmillF15;

class TreadmillButton : public button::Button, public Component {
 public:
  void set_parent(TreadmillF15 *parent) { this->parent_ = parent; };
  void set_button_type(const std::string &button_type) { this->button_type_ = button_type; };
  void set_register_address(uint8_t register_addr) { this->register_address_ = register_addr; };
  void set_command_payload(const std::vector<uint8_t> &payload) { this->command_payload_ = payload; };
  void dump_config() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void press_action() override;
  TreadmillF15 *parent_;
  std::string button_type_;
  uint8_t register_address_;
  std::vector<uint8_t> command_payload_;
};

}  // namespace treadmill_f15
}  // namespace esphome

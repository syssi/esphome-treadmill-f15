#pragma once

#include "../treadmill_f15.h"
#include "esphome/core/component.h"
#include "esphome/components/number/number.h"

namespace esphome {
namespace treadmill_f15 {

class TreadmillF15;

class TreadmillNumber : public number::Number, public Component {
 public:
  void set_parent(TreadmillF15 *parent) { this->parent_ = parent; };
  void set_command_type(const std::string &command_type) { this->command_type_ = command_type; };
  void set_holding_register(uint8_t holding_register) { this->holding_register_ = holding_register; };
  void set_initial_value(float initial_value) { this->initial_value_ = initial_value; };
  void setup() override;
  void dump_config() override;
  void loop() override {}
  float get_setup_priority() const override { return setup_priority::DATA; }

 protected:
  void control(float value) override;
  TreadmillF15 *parent_;
  std::string command_type_;
  uint8_t holding_register_;
  float initial_value_;
};

}  // namespace treadmill_f15
}  // namespace esphome

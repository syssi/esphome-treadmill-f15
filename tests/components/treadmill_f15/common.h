#pragma once
#include <cstdint>
#include <vector>
#include "esphome/components/treadmill_f15/treadmill_f15.h"

namespace esphome::treadmill_f15::testing {

class TestableTreadmillF15 : public TreadmillF15 {
 public:
  void update() override {}
  using TreadmillF15::decode_status_data_;
};

}  // namespace esphome::treadmill_f15::testing

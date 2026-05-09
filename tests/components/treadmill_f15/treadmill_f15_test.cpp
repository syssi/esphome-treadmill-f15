#include <gtest/gtest.h>
#include "common.h"
#include "frames.h"

namespace esphome::treadmill_f15::testing {

TEST(TreadmillF15StoppedTest, BinarySensors) {
  TestableTreadmillF15 treadmill;
  binary_sensor::BinarySensor stopped, running, starting, stopping;
  treadmill.set_stopped_binary_sensor(&stopped);
  treadmill.set_running_binary_sensor(&running);
  treadmill.set_starting_binary_sensor(&starting);
  treadmill.set_stopping_binary_sensor(&stopping);
  treadmill.on_treadmill_f15_data(FRAME_STOPPED);
  EXPECT_TRUE(stopped.state);
  EXPECT_FALSE(running.state);
  EXPECT_FALSE(starting.state);
  EXPECT_FALSE(stopping.state);
}

TEST(TreadmillF15StoppedTest, OperationMode) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor mode_id;
  text_sensor::TextSensor mode_text;
  treadmill.set_operation_mode_id_sensor(&mode_id);
  treadmill.set_operation_mode_text_sensor(&mode_text);
  treadmill.on_treadmill_f15_data(FRAME_STOPPED);
  EXPECT_FLOAT_EQ(mode_id.state, 0.0f);
  EXPECT_EQ(mode_text.state, "Stopped");
}

TEST(TreadmillF15StoppedTest, SpeedAndHeartRateAreZero) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor speed, heart_rate;
  treadmill.set_speed_sensor(&speed);
  treadmill.set_heart_rate_sensor(&heart_rate);
  treadmill.on_treadmill_f15_data(FRAME_STOPPED);
  EXPECT_FLOAT_EQ(speed.state, 0.0f);
  EXPECT_FLOAT_EQ(heart_rate.state, 0.0f);
}

TEST(TreadmillF15StartingTest, BinarySensors) {
  TestableTreadmillF15 treadmill;
  binary_sensor::BinarySensor stopped, running, starting, stopping;
  treadmill.set_stopped_binary_sensor(&stopped);
  treadmill.set_running_binary_sensor(&running);
  treadmill.set_starting_binary_sensor(&starting);
  treadmill.set_stopping_binary_sensor(&stopping);
  treadmill.on_treadmill_f15_data(FRAME_STARTING);
  EXPECT_TRUE(starting.state);
  EXPECT_FALSE(running.state);
  EXPECT_FALSE(stopped.state);
  EXPECT_FALSE(stopping.state);
}

TEST(TreadmillF15StartingTest, OperationMode) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor mode_id;
  text_sensor::TextSensor mode_text;
  treadmill.set_operation_mode_id_sensor(&mode_id);
  treadmill.set_operation_mode_text_sensor(&mode_text);
  treadmill.on_treadmill_f15_data(FRAME_STARTING);
  EXPECT_FLOAT_EQ(mode_id.state, 2.0f);
  EXPECT_EQ(mode_text.state, "Starting");
}

TEST(TreadmillF15RunningTest, BinarySensors) {
  TestableTreadmillF15 treadmill;
  binary_sensor::BinarySensor stopped, running, starting, stopping;
  treadmill.set_stopped_binary_sensor(&stopped);
  treadmill.set_running_binary_sensor(&running);
  treadmill.set_starting_binary_sensor(&starting);
  treadmill.set_stopping_binary_sensor(&stopping);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_TRUE(running.state);
  EXPECT_FALSE(stopped.state);
  EXPECT_FALSE(starting.state);
  EXPECT_FALSE(stopping.state);
}

TEST(TreadmillF15RunningTest, OperationMode) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor mode_id;
  text_sensor::TextSensor mode_text;
  treadmill.set_operation_mode_id_sensor(&mode_id);
  treadmill.set_operation_mode_text_sensor(&mode_text);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_FLOAT_EQ(mode_id.state, 3.0f);
  EXPECT_EQ(mode_text.state, "Running");
}

TEST(TreadmillF15RunningTest, Speed) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor speed;
  treadmill.set_speed_sensor(&speed);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_NEAR(speed.state, 1.3f, 0.001f);
}

TEST(TreadmillF15RunningTest, Incline) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor incline;
  treadmill.set_incline_sensor(&incline);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_NEAR(incline.state, 0.0f, 0.001f);
}

TEST(TreadmillF15RunningTest, ElapsedTime) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor elapsed;
  treadmill.set_elapsed_time_sensor(&elapsed);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_FLOAT_EQ(elapsed.state, 32.0f);
}

TEST(TreadmillF15RunningTest, Distance) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor distance;
  treadmill.set_distance_sensor(&distance);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_NEAR(distance.state, 0.009f, 0.0001f);
}

TEST(TreadmillF15RunningTest, Calories) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor calories;
  treadmill.set_calories_sensor(&calories);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_NEAR(calories.state, 0.3f, 0.001f);
}

TEST(TreadmillF15RunningTest, HeartRate) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor heart_rate;
  treadmill.set_heart_rate_sensor(&heart_rate);
  treadmill.on_treadmill_f15_data(FRAME_RUNNING);
  EXPECT_FLOAT_EQ(heart_rate.state, 83.0f);
}

TEST(TreadmillF15StoppingTest, BinarySensors) {
  TestableTreadmillF15 treadmill;
  binary_sensor::BinarySensor stopped, running, starting, stopping;
  treadmill.set_stopped_binary_sensor(&stopped);
  treadmill.set_running_binary_sensor(&running);
  treadmill.set_starting_binary_sensor(&starting);
  treadmill.set_stopping_binary_sensor(&stopping);
  treadmill.on_treadmill_f15_data(FRAME_STOPPING);
  EXPECT_TRUE(stopping.state);
  EXPECT_FALSE(running.state);
  EXPECT_FALSE(stopped.state);
  EXPECT_FALSE(starting.state);
}

TEST(TreadmillF15StoppingTest, OperationMode) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor mode_id;
  text_sensor::TextSensor mode_text;
  treadmill.set_operation_mode_id_sensor(&mode_id);
  treadmill.set_operation_mode_text_sensor(&mode_text);
  treadmill.on_treadmill_f15_data(FRAME_STOPPING);
  EXPECT_FLOAT_EQ(mode_id.state, 4.0f);
  EXPECT_EQ(mode_text.state, "Stopping");
}

TEST(TreadmillF15StoppingTest, SpeedAndHeartRate) {
  TestableTreadmillF15 treadmill;
  sensor::Sensor speed, heart_rate;
  treadmill.set_speed_sensor(&speed);
  treadmill.set_heart_rate_sensor(&heart_rate);
  treadmill.on_treadmill_f15_data(FRAME_STOPPING);
  EXPECT_NEAR(speed.state, 1.0f, 0.001f);
  EXPECT_FLOAT_EQ(heart_rate.state, 84.0f);
}

}  // namespace esphome::treadmill_f15::testing

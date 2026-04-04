"""Schema structure tests for treadmill_f15 ESPHome component modules."""

import os
import sys

sys.path.insert(0, os.path.join(os.path.dirname(__file__), ".."))

import components.treadmill_f15 as hub  # noqa: E402
from components.treadmill_f15 import (  # noqa: E402
    binary_sensor,
    button,  # noqa: E402
    number,  # noqa: E402
    sensor,
    text_sensor,
)


class TestHubConstants:
    def test_conf_id_defined(self):
        assert hub.CONF_TREADMILL_F15_ID == "treadmill_f15_id"


class TestSensorLists:
    def test_sensor_defs_completeness(self):
        assert sensor.CONF_OPERATION_MODE_ID in sensor.SENSOR_DEFS
        assert "speed" in sensor.SENSOR_DEFS
        assert sensor.CONF_INCLINE in sensor.SENSOR_DEFS
        assert sensor.CONF_ELAPSED_TIME in sensor.SENSOR_DEFS
        assert "distance" in sensor.SENSOR_DEFS
        assert sensor.CONF_CALORIES in sensor.SENSOR_DEFS
        assert sensor.CONF_HEART_RATE in sensor.SENSOR_DEFS
        assert len(sensor.SENSOR_DEFS) == 7

    def test_sensor_keys_are_strings(self):
        for key in sensor.SENSOR_DEFS:
            assert isinstance(key, str)


class TestBinarySensorConstants:
    def test_binary_sensor_defs_dict(self):
        assert binary_sensor.CONF_STARTING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_RUNNING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_STOPPING in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_STOPPED in binary_sensor.BINARY_SENSOR_DEFS
        assert len(binary_sensor.BINARY_SENSOR_DEFS) == 4


class TestTextSensorConstants:
    def test_text_sensors_list(self):
        assert text_sensor.CONF_ELAPSED_TIME_FORMATTED in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_OPERATION_MODE in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 2


class TestButtonConstants:
    def test_button_commands_dict(self):
        assert button.CONF_START in button.BUTTON_COMMANDS
        assert "stop" in button.BUTTON_COMMANDS
        assert len(button.BUTTON_COMMANDS) == 2

    def test_button_commands_have_register_and_payload(self):
        for cmd in button.BUTTON_COMMANDS.values():
            assert "register" in cmd
            assert "payload" in cmd
            assert isinstance(cmd["register"], int)
            assert isinstance(cmd["payload"], list)


class TestNumberConstants:
    def test_numbers_dict(self):
        assert "speed" in number.NUMBERS
        assert number.CONF_INCLINE in number.NUMBERS
        assert len(number.NUMBERS) == 2

    def test_number_addresses_are_unique(self):
        addresses = list(number.NUMBERS.values())
        assert len(addresses) == len(set(addresses))

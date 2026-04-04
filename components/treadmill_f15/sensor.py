import esphome.codegen as cg
from esphome.components import sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_DISTANCE,
    CONF_SPEED,
    DEVICE_CLASS_DISTANCE,
    DEVICE_CLASS_DURATION,
    DEVICE_CLASS_ENERGY,
    DEVICE_CLASS_SPEED,
    STATE_CLASS_MEASUREMENT,
    UNIT_BEATS_PER_MINUTE,
    UNIT_KILOMETER,
    UNIT_KILOMETER_PER_HOUR,
    UNIT_PERCENT,
    UNIT_SECOND,
)

from . import CONF_TREADMILL_F15_ID, TREADMILL_F15_COMPONENT_SCHEMA

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

CONF_OPERATION_MODE_ID = "operation_mode_id"
# CONF_SPEED = "speed"
CONF_INCLINE = "incline"
CONF_ELAPSED_TIME = "elapsed_time"
# CONF_DISTANCE = "distance"
CONF_CALORIES = "calories"
CONF_HEART_RATE = "heart_rate"

UNIT_KILO_CALORIE = "kcal"

# key: sensor_schema kwargs
SENSOR_DEFS = {
    CONF_OPERATION_MODE_ID: {
        "icon": "mdi:numeric",
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_SPEED: {
        "unit_of_measurement": UNIT_KILOMETER_PER_HOUR,
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_SPEED,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_INCLINE: {
        "unit_of_measurement": UNIT_PERCENT,
        "icon": "mdi:slope-uphill",
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_ELAPSED_TIME: {
        "unit_of_measurement": UNIT_SECOND,
        "accuracy_decimals": 0,
        "device_class": DEVICE_CLASS_DURATION,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_DISTANCE: {
        "unit_of_measurement": UNIT_KILOMETER,
        "accuracy_decimals": 3,
        "device_class": DEVICE_CLASS_DISTANCE,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_CALORIES: {
        "unit_of_measurement": UNIT_KILO_CALORIE,
        "icon": "mdi:fire",
        "accuracy_decimals": 1,
        "device_class": DEVICE_CLASS_ENERGY,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
    CONF_HEART_RATE: {
        "unit_of_measurement": UNIT_BEATS_PER_MINUTE,
        "icon": "mdi:heart-pulse",
        "accuracy_decimals": 0,
        "state_class": STATE_CLASS_MEASUREMENT,
    },
}

SENSORS = list(SENSOR_DEFS)

CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): sensor.sensor_schema(**kwargs)
        for key, kwargs in SENSOR_DEFS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for key in SENSORS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))

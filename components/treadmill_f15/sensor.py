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

SENSORS = [
    CONF_OPERATION_MODE_ID,
    CONF_SPEED,
    CONF_INCLINE,
    CONF_ELAPSED_TIME,
    CONF_DISTANCE,
    CONF_CALORIES,
    CONF_HEART_RATE,
]

# pylint: disable=too-many-function-args
CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_OPERATION_MODE_ID): sensor.sensor_schema(
            icon="mdi:numeric",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_SPEED): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_SPEED,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_INCLINE): sensor.sensor_schema(
            unit_of_measurement=UNIT_PERCENT,
            icon="mdi:slope-uphill",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_ELAPSED_TIME): sensor.sensor_schema(
            unit_of_measurement=UNIT_SECOND,
            accuracy_decimals=0,
            device_class=DEVICE_CLASS_DURATION,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_DISTANCE): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILOMETER,
            accuracy_decimals=3,
            device_class=DEVICE_CLASS_DISTANCE,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_CALORIES): sensor.sensor_schema(
            unit_of_measurement=UNIT_KILO_CALORIE,
            icon="mdi:fire",
            accuracy_decimals=1,
            device_class=DEVICE_CLASS_ENERGY,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_HEART_RATE): sensor.sensor_schema(
            unit_of_measurement=UNIT_BEATS_PER_MINUTE,
            icon="mdi:heart-pulse",
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for key in SENSORS:
        if key in config:
            conf = config[key]
            sens = await sensor.new_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_sensor")(sens))

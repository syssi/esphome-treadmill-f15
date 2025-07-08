import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID, ICON_TIMELAPSE

from . import CONF_TREADMILL_F15_ID, TREADMILL_F15_COMPONENT_SCHEMA

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

CONF_ELAPSED_TIME_FORMATTED = "elapsed_time_formatted"
CONF_OPERATION_MODE = "operation_mode"

TEXT_SENSORS = [
    CONF_ELAPSED_TIME_FORMATTED,
    CONF_OPERATION_MODE,
]

CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_ELAPSED_TIME_FORMATTED): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon=ICON_TIMELAPSE
        ),
        cv.Optional(CONF_OPERATION_MODE): text_sensor.text_sensor_schema(
            text_sensor.TextSensor, icon="mdi:run"
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for key in TEXT_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await text_sensor.register_text_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_text_sensor")(sens))

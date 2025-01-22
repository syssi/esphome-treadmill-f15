import esphome.codegen as cg
from esphome.components import text_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_ID, ICON_TIMELAPSE

from . import CONF_TREADMILL_F15_ID, TreadmillF15

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

CONF_ELAPSED_TIME_FORMATTED = "elapsed_time_formatted"
CONF_OPERATION_MODE = "operation_mode"

TEXT_SENSORS = [
    CONF_ELAPSED_TIME_FORMATTED,
    CONF_OPERATION_MODE,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_TREADMILL_F15_ID): cv.use_id(TreadmillF15),
        cv.Optional(CONF_ELAPSED_TIME_FORMATTED): text_sensor.TEXT_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(text_sensor.TextSensor),
                cv.Optional(CONF_ICON, default=ICON_TIMELAPSE): cv.icon,
            }
        ),
        cv.Optional(CONF_OPERATION_MODE): text_sensor.TEXT_SENSOR_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(text_sensor.TextSensor),
                cv.Optional(CONF_ICON, default="mdi:run"): cv.icon,
            }
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

import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

from . import CONF_TREADMILL_F15_ID, TreadmillF15

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

CONF_STARTING = "starting"
CONF_RUNNING = "running"
CONF_STOPPING = "stopping"
CONF_STOPPED = "stopped"

BINARY_SENSORS = [
    CONF_STARTING,
    CONF_RUNNING,
    CONF_STOPPING,
    CONF_STOPPED,
]

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_TREADMILL_F15_ID): cv.use_id(TreadmillF15),
        cv.Optional(CONF_STARTING): binary_sensor.binary_sensor_schema(
            icon="mdi:rocket-launch"
        ),
        cv.Optional(CONF_RUNNING): binary_sensor.binary_sensor_schema(
            icon="mdi:run-fast"
        ),
        cv.Optional(CONF_STOPPING): binary_sensor.binary_sensor_schema(icon="mdi:run"),
        cv.Optional(CONF_STOPPED): binary_sensor.binary_sensor_schema(icon="mdi:pause"),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for key in BINARY_SENSORS:
        if key in config:
            conf = config[key]
            sens = cg.new_Pvariable(conf[CONF_ID])
            await binary_sensor.register_binary_sensor(sens, conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))

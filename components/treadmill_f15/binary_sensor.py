import esphome.codegen as cg
from esphome.components import binary_sensor
import esphome.config_validation as cv
from esphome.const import ENTITY_CATEGORY_DIAGNOSTIC

from . import CONF_TREADMILL_F15_ID, TREADMILL_F15_COMPONENT_SCHEMA

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

CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_STARTING): binary_sensor.binary_sensor_schema(
            icon="mdi:rocket-launch",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_RUNNING): binary_sensor.binary_sensor_schema(
            icon="mdi:run-fast",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_STOPPING): binary_sensor.binary_sensor_schema(
            icon="mdi:run",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
        cv.Optional(CONF_STOPPED): binary_sensor.binary_sensor_schema(
            icon="mdi:pause",
            entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for key in BINARY_SENSORS:
        if key in config:
            conf = config[key]
            sens = await binary_sensor.new_binary_sensor(conf)
            cg.add(getattr(hub, f"set_{key}_binary_sensor")(sens))

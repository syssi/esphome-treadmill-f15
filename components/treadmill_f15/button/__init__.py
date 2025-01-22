import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_ID, CONF_STOP

from .. import CONF_TREADMILL_F15_ID, TREADMILL_F15_COMPONENT_SCHEMA, treadmill_f15_ns

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

CONF_START = "start"

ICON_START = "mdi:play"
ICON_STOP = "mdi:stop"

BUTTON_COMMANDS = {
    CONF_START: {
        "register": 0x53,
        "payload": [0x01, 0x00, 0x00, 0x00, 0x00, 0x00],
    },
    CONF_STOP: {
        "register": 0x53,
        "payload": [0x03],
    },
}

BUTTON_TYPES = [CONF_START, CONF_STOP]

TreadmillButton = treadmill_f15_ns.class_(
    "TreadmillButton", button.Button, cg.Component
)

CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_START): button.button_schema(
            TreadmillButton, icon=ICON_START
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_STOP): button.button_schema(
            TreadmillButton, icon=ICON_STOP
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for button_type in BUTTON_TYPES:
        if button_type in config:
            conf = config[button_type]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await button.register_button(var, conf)
            cg.add(var.set_parent(hub))
            cg.add(var.set_button_type(button_type))

            cmd_def = BUTTON_COMMANDS[button_type]
            cg.add(var.set_register_address(cmd_def["register"]))
            cg.add(var.set_command_payload(cmd_def["payload"]))

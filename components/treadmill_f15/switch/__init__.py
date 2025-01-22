import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import CONF_ICON, CONF_ID

from .. import CONF_TREADMILL_F15_ID, TREADMILL_F15_COMPONENT_SCHEMA, treadmill_f15_ns

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

CONF_CHARGING = "charging"
CONF_DISCHARGING = "discharging"

ICON_CHARGING = "mdi:battery-charging-50"
ICON_DISCHARGING = "mdi:battery-charging-50"

SWITCHES = {
    CONF_CHARGING: [0xD888, 0xD900],
    CONF_DISCHARGING: [0x8555, 0x8600],
}

TreadmillSwitch = treadmill_f15_ns.class_(
    "TreadmillSwitch", switch.Switch, cg.Component
)

CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_CHARGING): switch.SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(TreadmillSwitch),
                cv.Optional(CONF_ICON, default=ICON_CHARGING): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_DISCHARGING): switch.SWITCH_SCHEMA.extend(
            {
                cv.GenerateID(): cv.declare_id(TreadmillSwitch),
                cv.Optional(CONF_ICON, default=ICON_DISCHARGING): cv.icon,
            }
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for key, address in SWITCHES.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await switch.register_switch(var, conf)
            cg.add(getattr(hub, f"set_{key}_switch")(var))
            cg.add(var.set_parent(hub))
            cg.add(var.set_command_turn_on(address[0]))
            cg.add(var.set_command_turn_off(address[1]))

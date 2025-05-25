import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import CONF_FACTORY_RESET, CONF_ID

from .. import CONF_TREADMILL_F15_ID, TREADMILL_F15_COMPONENT_SCHEMA, treadmill_f15_ns

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

CONF_RETRIEVE_SETTINGS = "retrieve_settings"
# CONF_FACTORY_RESET = "factory_reset"
CONF_RESET_CHARGING_CYCLES = "reset_charging_cycles"
CONF_RESET_TOTAL_CHARGED_CAPACITY = "reset_total_charged_capacity"
CONF_RESET_TOTAL_DISCHARGED_CAPACITY = "reset_total_discharged_capacity"

ICON_RETRIEVE_SETTINGS = "mdi:cog"
ICON_FACTORY_RESET = "mdi:factory"
ICON_RESET_CHARGING_CYCLES = "mdi:counter"
ICON_RESET_TOTAL_CHARGED_CAPACITY = "mdi:counter"
ICON_RESET_TOTAL_DISCHARGED_CAPACITY = "mdi:counter"

BUTTONS = {
    CONF_RETRIEVE_SETTINGS: 0x5600,
    CONF_FACTORY_RESET: 0xCCCC,
    CONF_RESET_CHARGING_CYCLES: 0xAA55,
    CONF_RESET_TOTAL_CHARGED_CAPACITY: 0xCB00,
    CONF_RESET_TOTAL_DISCHARGED_CAPACITY: 0xCA00,
}

TreadmillButton = treadmill_f15_ns.class_(
    "TreadmillButton", button.Button, cg.Component
)

CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_RETRIEVE_SETTINGS): button.button_schema(
            TreadmillButton, icon=ICON_RETRIEVE_SETTINGS
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_FACTORY_RESET): button.button_schema(
            TreadmillButton, icon=ICON_FACTORY_RESET
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RESET_CHARGING_CYCLES): button.button_schema(
            TreadmillButton, icon=ICON_RESET_CHARGING_CYCLES
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RESET_TOTAL_CHARGED_CAPACITY): button.button_schema(
            TreadmillButton, icon=ICON_RESET_TOTAL_CHARGED_CAPACITY
        ).extend(cv.COMPONENT_SCHEMA),
        cv.Optional(CONF_RESET_TOTAL_DISCHARGED_CAPACITY): button.button_schema(
            TreadmillButton, icon=ICON_RESET_TOTAL_DISCHARGED_CAPACITY
        ).extend(cv.COMPONENT_SCHEMA),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])
    for key, address in BUTTONS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await button.register_button(var, conf)
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address))

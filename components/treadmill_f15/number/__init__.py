import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    CONF_ID,
    CONF_INITIAL_VALUE,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_MODE,
    CONF_RESTORE_VALUE,
    CONF_SPEED,
    CONF_STEP,
    DEVICE_CLASS_EMPTY,
    ENTITY_CATEGORY_CONFIG,
    UNIT_KILOMETER_PER_HOUR,
    UNIT_PERCENT,
)

from .. import CONF_TREADMILL_F15_ID, TREADMILL_F15_COMPONENT_SCHEMA, treadmill_f15_ns

DEPENDENCIES = ["treadmill_f15"]

CODEOWNERS = ["@syssi"]

# CONF_SPEED = "speed"
CONF_INCLINE = "incline"

ICON_SPEEDOMETER = "mdi:speedometer"
ICON_INCLINE = "mdi:angle-acute"

NUMBERS = {
    CONF_SPEED: 0x01,
    CONF_INCLINE: 0x02,
}

TreadmillNumber = treadmill_f15_ns.class_(
    "TreadmillNumber", number.Number, cg.Component
)

TREADMILL_NUMBER_SCHEMA = (
    number.number_schema(
        TreadmillNumber,
        entity_category=ENTITY_CATEGORY_CONFIG,
        device_class=DEVICE_CLASS_EMPTY,
    )
    .extend(
        {
            cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                number.NUMBER_MODES, upper=True
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = TREADMILL_F15_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_SPEED): number.number_schema(
            TreadmillNumber,
            icon=ICON_SPEEDOMETER,
            entity_category=ENTITY_CATEGORY_CONFIG,
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_KILOMETER_PER_HOUR,
        )
        .extend(cv.COMPONENT_SCHEMA)
        .extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.8): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=12.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.1): cv.float_,
                cv.Optional(CONF_INITIAL_VALUE, default=0.8): cv.float_,
                cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
                cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                    number.NUMBER_MODES, upper=True
                ),
            }
        ),
        cv.Optional(CONF_INCLINE): number.number_schema(
            TreadmillNumber,
            icon=ICON_INCLINE,
            entity_category=ENTITY_CATEGORY_CONFIG,
            device_class=DEVICE_CLASS_EMPTY,
            unit_of_measurement=UNIT_PERCENT,
        )
        .extend(cv.COMPONENT_SCHEMA)
        .extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=15.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.5): cv.float_,
                cv.Optional(CONF_INITIAL_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_RESTORE_VALUE, default=True): cv.boolean,
                cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                    number.NUMBER_MODES, upper=True
                ),
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_TREADMILL_F15_ID])

    for key, number_config in NUMBERS.items():
        if key in config:
            conf = config[key]
            var = cg.new_Pvariable(conf[CONF_ID])
            await cg.register_component(var, conf)
            await number.register_number(
                var,
                conf,
                min_value=conf[CONF_MIN_VALUE],
                max_value=conf[CONF_MAX_VALUE],
                step=conf[CONF_STEP],
            )
            cg.add(var.set_parent(hub))
            cg.add(var.set_command_type(key))
            cg.add(var.set_holding_register(number_config))
            cg.add(var.set_initial_value(conf[CONF_INITIAL_VALUE]))
            cg.add(getattr(hub, f"set_{key}_number")(var))

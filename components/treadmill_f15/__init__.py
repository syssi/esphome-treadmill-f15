import esphome.codegen as cg
from esphome.components import ble_client
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@syssi"]

AUTO_LOAD = ["binary_sensor", "button", "sensor", "switch", "text_sensor"]
MULTI_CONF = True

CONF_TREADMILL_F15_ID = "treadmill_f15_id"

treadmill_f15_ns = cg.esphome_ns.namespace("treadmill_f15")
TreadmillF15 = treadmill_f15_ns.class_(
    "TreadmillF15", ble_client.BLEClientNode, cg.PollingComponent
)

TREADMILL_F15_COMPONENT_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_TREADMILL_F15_ID): cv.use_id(TreadmillF15),
    }
)

CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(TreadmillF15),
        }
    )
    .extend(ble_client.BLE_CLIENT_SCHEMA)
    .extend(cv.polling_component_schema("10s"))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)

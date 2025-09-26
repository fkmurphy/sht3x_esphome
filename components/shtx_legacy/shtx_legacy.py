import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor
from esphome.const import CONF_ID, CONF_TEMPERATURE, CONF_HUMIDITY

ns = cg.esphome_ns.namespace("shtx_legacy")
SHTxLegacy = ns.class_("SHTxLegacy", cg.PollingComponent, sensor.Sensor)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SHTxLegacy),
    cv.Optional(CONF_TEMPERATURE): sensor.sensor_schema(),
    cv.Optional(CONF_HUMIDITY): sensor.sensor_schema(),
}).extend(cv.polling_component_schema("60s"))

def to_code(cfg):
    var = cg.new_Pvariable(cfg[CONF_ID])
    yield cg.register_component(var, cfg)
    if CONF_TEMPERATURE in cfg:
        sens = yield sensor.new_sensor(cfg[CONF_TEMPERATURE])
        cg.add(var.set_temperature_sensor(sens))
    if CONF_HUMIDITY in cfg:
        sens = yield sensor.new_sensor(cfg[CONF_HUMIDITY])
        cg.add(var.set_humidity_sensor(sens))

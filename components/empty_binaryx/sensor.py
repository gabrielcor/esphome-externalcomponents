import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import binary_sensor, i2c, sensor
from esphome.const import (
    CONF_ID, 
    CONF_TRIGGER_ID
)

AUTO_LOAD = [ "binary_sensor", "sensor" ]

empty_binaryx_sensor_ns = cg.esphome_ns.namespace('empty_binaryx')

EmptyBinaryxSensor = empty_binaryx_sensor_ns.class_('EmptyBinaryxSensor', binary_sensor.BinarySensor, cg.Component)

CONFIG_SCHEMA = binary_sensor.BINARY_SENSOR_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(EmptyBinaryxSensor),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)
    yield binary_sensor.register_binary_sensor(var, config)
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation, pins
from esphome.components import sensor
from esphome.const import (
    CONF_ID,
    CONF_PIN,
    CONF_NUMBER,
    CONF_VALUE,
    STATE_CLASS_TOTAL_INCREASING,
)
from esphome.core import CORE

CODEOWNERS = ["@gabrielcor"]

knock_pattern_detector_ns = cg.esphome_ns.namespace("knock_pattern_detector")
CustomKnockSensor = knock_pattern_detector_ns.class_("CustomKnockPatternDetector", sensor.Sensor, cg.Component)


CONF_KNOCK_PATTERN = "knock_pattern"
CONF_KNOCK_PATTERN_LENGTH = "knock_pattern_length"
CONF_KNOCK_SENSOR_THRESHOLD = "knock_sensor_threshold"
CONF_KNOCK_ERROR_TOLERANCE = "knock_error_tolerance"
CONF_KNOCK_AVERAGE_ERROR_TOLERANCE = "knock_average_error_tolerance"
CONF_KNOCK_PATTERN_MINGAP_BETWEEN_KNOCKS = "knock_pattern_mingap_between_knocks"
CONF_KNOCK_PATTERN_MAXGAP_BETWEEN_KNOCKS = "knock_pattern_maxgap_between_knocks"

def validate_sensor_threshold(value):
    if value < 1 or value > 1023:
        raise cv.Invalid("Sensor threshold must be between 1 and 1023")
    return value


CONFIG_SCHEMA = sensor.sensor_schema(
    CustomKnockSensor,
    unit_of_measurement="knocks",
    icon="mdi:gesture-double-tap",
    accuracy_decimals=0,
    state_class=STATE_CLASS_TOTAL_INCREASING,
).extend(
    {
        cv.Required(CONF_PIN): pins.gpio_input_pin_schema,
        cv.Optional(CONF_KNOCK_PATTERN, default=[512, 256, 256, 512, 1024, 512]): cv.ensure_list(cv.positive_int),
        cv.Optional(CONF_KNOCK_PATTERN_LENGTH, default=6): cv.positive_int,
        cv.Optional(CONF_KNOCK_SENSOR_THRESHOLD, default=1): validate_sensor_threshold,
        cv.Optional(CONF_KNOCK_ERROR_TOLERANCE, default=256): cv.positive_int,
        cv.Optional(CONF_KNOCK_AVERAGE_ERROR_TOLERANCE, default=256): cv.positive_int,
        cv.Optional(CONF_KNOCK_PATTERN_MINGAP_BETWEEN_KNOCKS, default=125): cv.positive_int,
        cv.Optional(CONF_KNOCK_PATTERN_MAXGAP_BETWEEN_KNOCKS, default=1500): cv.positive_int,
    }
)


async def to_code(config):
    var = await sensor.new_sensor(config)
    await cg.register_component(var, config)
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    cg.add(var.set_knock_pattern(config[CONF_KNOCK_PATTERN]))
    cg.add(var.set_knock_pattern_length(config[CONF_KNOCK_PATTERN_LENGTH]))
    cg.add(var.set_knock_sensor_threshold(config[CONF_KNOCK_SENSOR_THRESHOLD]))
    cg.add(var.set_knock_error_tolerance(config[CONF_KNOCK_ERROR_TOLERANCE]))
    cg.add(var.set_knock_average_error_tolerance(config[CONF_KNOCK_AVERAGE_ERROR_TOLERANCE]))
    cg.add(var.set_knock_pattern_mingap_between_knocks(config[CONF_KNOCK_PATTERN_MINGAP_BETWEEN_KNOCKS]))
    cg.add(var.set_knock_pattern_maxgap_between_knocks(config[CONF_KNOCK_PATTERN_MAXGAP_BETWEEN_KNOCKS]))



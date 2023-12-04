#include "esphome/core/log.h"
#include "empty_binaryx_sensor.h"

namespace esphome {
namespace empty_binaryx {

static const char *TAG = "empty_binaryx.binary_sensor";

void EmptyBinaryxSensor::setup() {
    
}
  
void EmptyBinaryxSensor::update() {

}

void EmptyBinaryxSensor::dump_config() {
    ESP_LOGCONFIG(TAG, "Custom binary sensor");
}

} //namespace empty_binaryx
} //namespace esphome
#pragma once
#include "esphome/components/adc/adc_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace knock_pattern_detector {

class CustomKnockPatternDetector : public sensor::Sensor, public Component {
 public:

  void set_maglock_pin(InternalGPIOPin *pin) { this->maglock_pin_ = pin; }
  void set_knock_pattern(std::vector<int> vector) { this->knock_pattern_ = vector; }
  void set_knock_pattern_length(uint32_t length) { this->knock_pattern_length_ = length; }
  void set_knock_sensor_threshold(uint32_t threshold) { this->knock_sensor_threshold_ = threshold; }
  void set_knock_error_tolerance(uint32_t tolerance) { this->knock_error_tolerance_ = tolerance; }    
  void set_knock_average_error_tolerance(uint32_t tolerance) { this->knock_average_error_tolerance_ = tolerance; }
  void set_knock_pattern_mingap_between_knocks(uint32_t mingap) { this->knock_pattern_mingap_between_knocks_ = mingap; }
  void set_knock_pattern_maxgap_between_knocks(uint32_t maxgap) { this->knock_pattern_maxgap_between_knocks_ = maxgap; }
  void set_adc(adc::ADCSensor *adc) { this->adc_ = adc; }



void setup() override;
void loop() override;
void dump_config() override;
void calc_mean_adc();
bool knockDetected();

protected:
  GPIOPin *maglock_pin_;
  std::vector<int> knock_pattern_;  
  int knock_pattern_length_;
  int knock_sensor_threshold_;
  int knock_error_tolerance_;
  int knock_average_error_tolerance_;
  int knock_pattern_mingap_between_knocks_;
  int knock_pattern_maxgap_between_knocks_;
  // Add ADCSensor
  adc::ADCSensor *adc_;


};
} // namespace knock_pattern_detector
}  // namespace esphome

#include "knock_pattern_detector.h"
namespace esphome {
namespace knock_pattern_detector {

static const char *const TAG = "knock_pattern_detector";

void CustomKnockPatternDetector::setup() {
 // Read 10 analog samples from ADC and calculate the average mean
  int adc_value = 0;
  int adc_sum = 0;
  int adc_mean = 0;
  int adc_samples = 10;
  for (int i = 0; i < adc_samples; i++) {
    adc_value = adc_-> get_raw_state();
    ESP_LOGD(TAG, "Iteration: %d value %d", i, adc_value );
    // adc_value = adc_->state;
    adc_sum += adc_value;
    delay(10);
  }
  adc_mean = adc_sum / adc_samples;
  ESP_LOGD(TAG, "ADC Mean: %d", adc_mean);

}

void CustomKnockPatternDetector::dump_config() {
  LOG_SENSOR("", "Pulse Meter", this);
  LOG_PIN("  Maglock Pin: ", this->maglock_pin_);
    
  ESP_LOGCONFIG("CustomKnockSensor", "Knock Sensor Threshold: %d", knock_sensor_threshold_);

  ESP_LOGCONFIG("CustomKnockSensor", "Knock Error Tolerance: %d", knock_error_tolerance_);

  ESP_LOGCONFIG("CustomKnockSensor", "Knock Average Error Tolerance: %d", knock_average_error_tolerance_);

  ESP_LOGCONFIG("CustomKnockSensor", "Knock Pattern Min Gap: %d", knock_pattern_mingap_between_knocks_);

  ESP_LOGCONFIG("CustomKnockSensor", "Knock Pattern Max Gap: %d", knock_pattern_maxgap_between_knocks_);

  ESP_LOGCONFIG("CustomKnockSensor", "Knock Pattern Values:");
  int pattern_count = 0;
  for (const auto &value : knock_pattern_) {
      pattern_count++;
      ESP_LOGCONFIG("CustomKnockSensor", "%d", value);
  }
  set_knock_pattern_length(pattern_count);
  ESP_LOGCONFIG("CustomKnockSensor", "Knock Pattern Length: %d", knock_pattern_length_);
  // show ADC Sensor configuration
  LOG_SENSOR("  ", "ADC Sensor", this->adc_);


}
/*
  std::vector<int> knock_pattern_;  
  int knock_pattern_length_;
  int knock_sensor_threshold_;
  int knock_error_tolerance_;
  int knock_average_error_tolerance_;
  int knock_pattern_mingap_between_knocks_;
  int knock_pattern_maxgap_between_knocks_;

*/
void CustomKnockPatternDetector::loop() {
  // Do nothing in update since this is just a demonstration
  // You can implement functionality to read sensor data periodically here if needed
}
}
}

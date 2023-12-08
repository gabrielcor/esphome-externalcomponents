#include "knock_pattern_detector.h"
namespace esphome {
namespace knock_pattern_detector {

void CustomKnockPatternDetector::setup() {
      ESP_LOGD("CustomKnockSensor", "Knock Pattern Length: %d", knock_pattern_length_);

      ESP_LOGD("CustomKnockSensor", "Knock Sensor Threshold: %d", knock_sensor_threshold_);

      ESP_LOGD("CustomKnockSensor", "Knock Error Tolerance: %d", knock_error_tolerance_);

      ESP_LOGD("CustomKnockSensor", "Knock Average Error Tolerance: %d", knock_average_error_tolerance_);

      ESP_LOGD("CustomKnockSensor", "Knock Pattern Min Gap: %d", knock_pattern_mingap_between_knocks_);

      ESP_LOGD("CustomKnockSensor", "Knock Pattern Max Gap: %d", knock_pattern_maxgap_between_knocks_);

      ESP_LOGD("CustomKnockSensor", "Knock Pattern Values:");
      for (const auto &value : knock_pattern_) {
          ESP_LOGD("CustomKnockSensor", "%d", value);
      }
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

#include "knock_pattern_detector.h"
namespace esphome {
namespace knock_pattern_detector {

static const char *const TAG = "knock_pattern_detector";

// The base reading of the sensor from which knocks will be measured.
// This value will be calibrated based on an average reading taken in calc_mean_adc()
float baseReading = 0;

int hasrun=0;
uint32_t start_time = 0;
uint32_t last_time = millis();
float realThreshold = 0;

void CustomKnockPatternDetector::setup() {

    start_time = millis();

}


// Calculate the average mean of 10 samples from ADC
void CustomKnockPatternDetector::calc_mean_adc()
{
  if (hasrun == 0)
  {
    // check if ten seconds have passed since the start of the program:
    if (millis() - start_time > 10000)
    {
      // Calculate the average mean of 10 samples from ADC
      float adc_sum = 0;
      int adc_samples = 10;

      for (int i = 0; i < adc_samples; i++)
      {
        float value_v = adc_->sample();
        ESP_LOGD(TAG, "Loop Iteration: %d Got voltage value=%.4fVoltios", i, value_v);
        adc_sum += value_v;
        delay(10);
      }
      baseReading = adc_sum / adc_samples;
      ESP_LOGD(TAG, "Loop ADC Mean: %.4f Voltios", baseReading);
      hasrun = 1;

      realThreshold = knock_sensor_threshold_ / knock_sensor_threshold_divider_;
      ESP_LOGD(TAG, "Real Threshold: %4f", realThreshold);

    }
  }
}

bool CustomKnockPatternDetector::knockDetected()
{
  // Read the ADC sensor
  float value_v = adc_->sample();

  // Calculate the difference between the current reading and the base reading
  float diff = value_v - baseReading;

  // If the difference is greater than the threshold, then a knock has been detected
  if (diff > realThreshold)
  {
    ESP_LOGD(TAG, "Kd-Got voltage value=%.4fVoltios", value_v);
    ESP_LOGD(TAG, "Kd-Diff: %.4f Voltios", diff);
    ESP_LOGD(TAG, "Kd-Knock detected");
    return true;
  }
  else
  {
    return false;
  }
}
void CustomKnockPatternDetector::loop() {
  calc_mean_adc();

  if ((baseReading != 0) && (millis () - last_time > 1000))
  {
    if (knockDetected())
    {
   
    }
    last_time = millis();
  }
}

void CustomKnockPatternDetector::dump_config() {
  LOG_SENSOR("", "Pulse Meter", this);
  LOG_PIN("  Maglock Pin: ", this->maglock_pin_);
    
  ESP_LOGCONFIG("CustomKnockSensor", "Knock Sensor Threshold: %d", knock_sensor_threshold_);

  ESP_LOGCONFIG("CustomKnockSensor", "Knock Sensor Threshold Divider: %d", knock_sensor_threshold_divider_);

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



} // namespace knock_pattern_detector
}  // namespace esphome



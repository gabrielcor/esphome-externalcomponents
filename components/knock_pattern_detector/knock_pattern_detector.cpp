#include "knock_pattern_detector.h"
namespace esphome {
namespace knock_pattern_detector {

static const char *const TAG = "knock_pattern_detector";
// Fastest I can knock is around 480BPM = 0.125secs between knocks. 400BPM = 0.15secs. 300BPM = 0.2secs
const int minGapBetweenKnocks = 125;
// The maximum gap after a knock (in ms) before the pattern is assumed to be finished
const int maxGapBetweenKnocks = 1500;

// The base reading of the sensor from which knocks will be measured.
// This value will be calibrated based on an average reading taken in calc_mean_adc()
float baseReading = 0;

int inputPattern[20];
int numKnocksInPattern = 0;

int secretKnockPattern[20];

int knockErrorTolerance;
int averageKnockErrorTolerance;

int hasrun=0;
uint32_t start_time = 0;

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

      realThreshold = static_cast<float>(knock_sensor_threshold_) / knock_sensor_threshold_divider_;

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
    ESP_LOGV(TAG, "Kd-Got voltage value=%.4fVoltios", value_v);
    ESP_LOGV(TAG, "Kd-Diff: %.4f Voltios", diff);
    ESP_LOGV(TAG, "Kd-Knock detected");
    return true;
  }
  else
  {
    return false;
  }
}

void CustomKnockPatternDetector::knockDelay(){
  // Rather than simply having using delay like this 
  // delay(minGapBetweenKnocks);
  // We'll break the delay into small chunks, reading the sensor pin each time to try to clear out
  // any erroneous readings caused by capacitance
  int iterations = (minGapBetweenKnocks / 10);
  float value_temp;
  for (int i=0; i<iterations; i++){
    delay(10);
    value_temp = adc_ -> sample();
  } 
}

void CustomKnockPatternDetector::listenToKnockPattern() {
  // Time at which the most recent knock was heard
  int lastKnockTime = millis();
  int now = millis();

  knockDelay();

  // Reset the input pattern array
  memset(inputPattern, 0, knock_pattern_length_ * sizeof(int));
      
  // How many knocks in total have been entered in this sequence?
  // (note that there must have been one already received in order to know a pattern was being input)
  int currentKnockNumber = 1;
      
  // Wait to hear if there's another knock heard in this sequence
  while((now - lastKnockTime < maxGapBetweenKnocks) && (currentKnockNumber < numKnocksInPattern)) {

    // Record the current time
    now = millis();

    // Has there been a new knock received?
    if(knockDetected()){
       
      // Record the time elapsed since the previous knock
      inputPattern[currentKnockNumber-1] = now - lastKnockTime; 
      // Increment the knock counter
      currentKnockNumber++;
      // And update the time at which last knock was received
      lastKnockTime = now;
     
      knockDelay();
    }
  }
 

}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    const long run = in_max - in_min;
    if(run == 0){
        return -1; // AVR returns -1, SAM returns 0
    }
    const long rise = out_max - out_min;
    const long delta = x - in_min;
    return (delta * rise) / run + out_min;
}


/** 
 * Scales the length of delays between knocks in an input pattern to a normalised range of values
 * This allows us to compare the rhythm of knock patterns irrespective of the tempo at which they were entered
 * 
 */
void normalizeKnockPattern(int knockPattern[]){
  // Keep track of the longest interval in the pattern
  int maxKnockInterval = 0;
    
  // Loop through the pattern
  for (int i=0; i<numKnocksInPattern-1; i++){
    // Keep track of the longest gap between knocks 
    if (knockPattern[i] > maxKnockInterval){ maxKnockInterval = knockPattern[i]; }
  }
  
  // Normalize the pattern by scaling all readings from 0-1024
  for (int i=0; i<numKnocksInPattern-1; i++){
    // Because arrays are passed by reference, changing the value here will change the value in the global variable
    knockPattern[i] = map(knockPattern[i], 0, maxKnockInterval, 0, 1024);
  }
}

void printPattern(int knockPattern[]){
  // Print the pattern
  for (int i=0; i<numKnocksInPattern-1; i++){
    ESP_LOGD(TAG, "Knock Pattern: %d", knockPattern[i]);
  }
}

// Compares two normalised knock patterns
// Returns true if the patterns match within given tolerance, or false otherwise
bool comparePattern(int inputPattern[], int secretKnockPattern[]) {

  // TEST #1: Has it got the correct number of knocks?
  for (int i=0; i<numKnocksInPattern-1; i++){
    if(inputPattern[i] == 0) {
      ESP_LOGD("TEST #1 FAILED. Input pattern had %d knocks. Secret pattern has %d knocks.", i+1, numKnocksInPattern);
      return false;
    }
  }

  // This variable records the total timing difference between patterns
  int totalDelta = 0;
      
  // TEST #2: Is the delay between any two successive knocks in the sequence outside the allowed tolerance?
  for (int i=0; i<numKnocksInPattern-1; i++){
    int knockDelta = abs(inputPattern[i] - secretKnockPattern[i]);
    if (knockDelta > knockErrorTolerance){
        ESP_LOGD("TEST #2 FAILED. Input pattern had delay after knock #%d of %dms. Secret pattern had %dms.", i+1, inputPattern[i], secretKnockPattern[i]);
        return false;
    }
    totalDelta += knockDelta;
  }
      
  // TEST #3: Is the whole pattern too sloppy?
  if (totalDelta / numKnocksInPattern > averageKnockErrorTolerance){
      ESP_LOGD("TEST #3 FAILED. Input pattern had average timing error of %dms. Secret pattern had %dms.", totalDelta / numKnocksInPattern, averageKnockErrorTolerance);
    return false; 
  }
      
  // If the code gets this far, all tests have passed!
  return true;
}

void onPuzzleSolved()
{}
void onIncorrectInput()
{}

/**
** Main Loop
*/
void CustomKnockPatternDetector::loop() {
  calc_mean_adc();

  if ((baseReading != 0) )
  {
    if (knockDetected())
    {
        listenToKnockPattern();
        normalizeKnockPattern(inputPattern);
        ESP_LOGD(TAG,"Knock pattern received");
        printPattern(inputPattern);
        ESP_LOGD(TAG,"Knock pattern expected");
        printPattern(secretKnockPattern);
      if(comparePattern(inputPattern, secretKnockPattern)){
        onPuzzleSolved(); 
      }      
      // Knock pattern entered did *not* match stored pattern
      else {
        onIncorrectInput();
      }

    }
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
  numKnocksInPattern = pattern_count+1;
  ESP_LOGCONFIG("CustomKnockSensor", "Knock Pattern Length: %d", knock_pattern_length_);
  // show ADC Sensor configuration
  LOG_SENSOR("  ", "ADC Sensor", this->adc_);
  // store knock_pattern_ in secretKnockPattern adjunsting also the length of secretKnockPattern with memset
  memset(secretKnockPattern, 0, knock_pattern_length_ * sizeof(int));
  for (int i=0; i<pattern_count; i++){
    secretKnockPattern[i] = knock_pattern_[i];
  }
  knockErrorTolerance = knock_error_tolerance_;
  averageKnockErrorTolerance  = knock_average_error_tolerance_;

}



} // namespace knock_pattern_detector
}  // namespace esphome



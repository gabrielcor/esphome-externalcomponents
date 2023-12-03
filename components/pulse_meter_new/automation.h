#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/pulse_meter_new/pulse_meter_new_sensor.h"

namespace esphome {

namespace pulse_meter_new {

template<typename... Ts> class SetTotalPulsesAction : public Action<Ts...> {
 public:
  SetTotalPulsesAction(PulseMeterSensor *pulse_meter_new) : pulse_meter_new_(pulse_meter_new) {}

  TEMPLATABLE_VALUE(uint32_t, total_pulses)

  void play(Ts... x) override { this->pulse_meter_new_->set_total_pulses(this->total_pulses_.value(x...)); }

 protected:
  PulseMeterSensor *pulse_meter_new_;
};

}  // namespace pulse_meter_new
}  // namespace esphome

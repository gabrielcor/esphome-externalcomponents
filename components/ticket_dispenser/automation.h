#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include "esphome/components/ticket_dispenser/ticket_dispenser.h"

namespace esphome {

namespace ticket_dispenser {

template<typename... Ts> class SetTotalPulsesAction : public Action<Ts...> {
 public:
  SetTotalPulsesAction(PulseMeterSensor *ticket_dispenser) : ticket_dispenser_(ticket_dispenser) {}

  TEMPLATABLE_VALUE(uint32_t, total_pulses)

  void play(Ts... x) override { this->ticket_dispenser_->set_total_pulses(this->total_pulses_.value(x...)); }

 protected:
  PulseMeterSensor *ticket_dispenser_;
};

}  // namespace ticket_dispenser
}  // namespace esphome

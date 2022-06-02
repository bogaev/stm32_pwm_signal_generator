#ifndef _SIGNAL_GENERATOR_H_
#define _SIGNAL_GENERATOR_H_

#include "main.h"
#include "App/pwm_types.h"
#include "App/signals/signals.h"
#include <cmath>

const uint16_t HALFWAVE_MAX_SIZE = (1<<13); //8192
const uint16_t POINTS_PER_HW_MIN = 128;
const uint16_t POINTS_PER_HW_MAX = HALFWAVE_MAX_SIZE;
const uint32_t DATA_BUFFER_SIZE = HALFWAVE_MAX_SIZE*2;
const uint16_t MAX_FREQ_COUNT = POINTS_PER_HW_MAX / POINTS_PER_HW_MIN;

struct tdSignalParams {
  uint16_t signal_type = (uint8_t)SIGNAL_TYPE_SINUS;
  uint16_t amp = 1;
  uint16_t freq = MAX_FREQ_COUNT;
};

class SignalGenerator {  
public:
  explicit SignalGenerator(Signal* carrier, Signal* fmod, Signal* amod)
    : carrier_(carrier), fmod_(fmod), amod_(amod)
    {}
  
  float operator()() {
    if(fmod_ && amod_) {
      GenerateFM().AddAM();
    }
    else if(fmod_) {
      GenerateFM();
    }
    else if(amod_) {
      GenerateCarrier().AddAM();
    }
    else {
      GenerateCarrier();
    }
    return value_;
  }

  SignalGenerator& GenerateCarrier() {
    value_ = (*carrier_)(time_);
    return *this;
  }  

  SignalGenerator& AddAM() {
    value_ *= (*amod_)(time_);
    return *this;
  }  
  
  SignalGenerator& GenerateFM() {
    value_ = carrier_->FM(time_, *fmod_);
    return *this;
  }  
  
private:
  Signal* carrier_ = nullptr;
  Signal* fmod_ = nullptr;
  Signal* amod_ = nullptr;
  float value_ = 0;
  uint32_t time_ = 0;
};

#endif // #ifndef _SIGNAL_GENERATOR_H_
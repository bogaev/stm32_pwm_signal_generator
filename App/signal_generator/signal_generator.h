#ifndef _SIGNAL_GENERATOR_H_
#define _SIGNAL_GENERATOR_H_

#include "main.h"
#include "App/pwm_types.h"
#include "App/signal_generator/signals.h"
#include <cmath>
#include <utility>
#include <memory>

const uint16_t HALFWAVE_MAX_SIZE = (1<<13); //8192
const uint16_t POINTS_PER_HW_MIN = 128;
const uint16_t POINTS_PER_HW_MAX = HALFWAVE_MAX_SIZE;
const uint32_t DATA_BUFFER_SIZE = HALFWAVE_MAX_SIZE*2;
const uint16_t MAX_FREQ_COUNT = POINTS_PER_HW_MAX / POINTS_PER_HW_MIN;

class SignalGenerator {  
public:
  
  SignalGenerator() :
    carrier_(Signal{}.Create(SIGNAL_TYPE_SINUS)),
    fmod_(Signal{}.Create(SIGNAL_TYPE_NONE)),
    amod_(Signal{}.Create(SIGNAL_TYPE_NONE))
    {}
  
  explicit SignalGenerator(tdSignalTypes_ carrier,
                           tdSignalTypes_ fmod = SIGNAL_TYPE_NONE,
                           tdSignalTypes_ amod = SIGNAL_TYPE_NONE)
    :
      carrier_(Signal{}.Create(carrier)),
      fmod_(Signal{}.Create(fmod)),
      amod_(Signal{}.Create(amod))
      {}
  
  explicit SignalGenerator(std::shared_ptr<Signal> carrier, 
                           std::shared_ptr<Signal> fmod = nullptr,
                           std::shared_ptr<Signal> amod = nullptr)
    : carrier_(carrier),
      fmod_(fmod),
      amod_(amod)
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
    if(carrier_) {
      value_ = (*carrier_)(time_);
    }
    return *this;
  }  

  SignalGenerator& AddAM() {
    if(amod_) {
      value_ *= (*amod_)(time_);
    }
    return *this;
  }  
  
  SignalGenerator& GenerateFM() {
    if(fmod_) {
      value_ = (*carrier_).FM(time_, *fmod_);
    }
    return *this;
  }
  
  void setSignal(uint8_t signal, uint8_t param, uint16_t value) {
    reset();
    if(signal == (uint8_t)UART_SIGNAL_CARRIER) {
      (*carrier_).setParam(param, value);
    }
    else if(signal == (uint8_t)UART_SIGNAL_AMP_MOD) {
      (*amod_).setParam(param, value);
    }
    else if(signal == (uint8_t)UART_SIGNAL_FREQ_MOD) {
      (*fmod_).setParam(param,value);
    }
  }

  std::shared_ptr<Signal> carrier_{};
  std::shared_ptr<Signal> amod_{};
  std::shared_ptr<Signal> fmod_{};
  
private:
  
  void reset() {
    time_ = 0;
    value_ = 0;
  }
  
  float value_ = 0;
  uint32_t time_ = 0;
};

#endif // #ifndef _SIGNAL_GENERATOR_H_
#include "signals.h"

// class Signal --------------------------------------------------------------

Signal& Signal::SetPeriod(float period) {
  period_ = period;
  freq_ = 1 / period_;
  return *this;
}
  
Signal& Signal::SetAmp(float amp) {
  amp_ = amp;
  return *this;
}
  
Signal& Signal::SetFreq(float freq) {
  freq_ = freq;
  period_ = 1 / freq_;
  return *this;
}

float Signal::GetAmp() {
  return amp_;
}

float Signal::GetFreq() {
  return freq_;
}
  
float Signal::GetPeriod() {
  return period_;
}
    
void Signal::setParam(uint8_t param, uint16_t value) {
  if(param == (uint8_t)UART_PARAM_SIGNAL_TYPE) {
//      p.signal_type = value;
  }
  else if(param == (uint8_t)UART_PARAM_AMP) {
    SetAmp(value);
  }
  else if(param == (uint8_t)UART_PARAM_FREQ) {
    SetFreq(value);
  }
}
  
std::unique_ptr<Signal> Signal::Create(tdSignalTypes_ sig_type) {
  if(sig_type == SIGNAL_TYPE_SINUS) {
    return std::make_unique<Sinus>();
  }
  return nullptr;
}

// class Sinus ---------------------------------------------------------------

float Sinus::operator()(uint32_t time) const {
  return amp_ * std::sin(pi * freq_ * time);
}
  
float Sinus::FM(uint32_t time, Signal& fmod) const {
    return amp_ * std::sin(pi * freq_ * time + fmod.GetIntegral(time));
}
  
float Sinus::GetIntegral(uint32_t time) const {
      return amp_ * std::cos(pi * freq_ * time);
}
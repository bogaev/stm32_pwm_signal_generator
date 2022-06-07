#include "signals.h"

// class Signal --------------------------------------------------------------

void Signal::SetParam(uint8_t param, uint16_t value) {
  if(param == (uint8_t)UART_PARAM_AMP) {
    SetAmp(value);
  }
  else if(param == (uint8_t)UART_PARAM_FREQ) {
    SetFreq(value);
  }
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

Signal& Signal::SetPeriod(float period) {
  period_ = period;
  freq_ = 1 / period_;
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
      
std::unique_ptr<Signal> Signal::Create(tdSignalTypes_ sig_type) {
  if(sig_type == SIGNAL_TYPE_SINUS) {
    return std::make_unique<Sinus>();
  }
  else if(sig_type == SIGNAL_TYPE_MEANDR) {
    return std::make_unique<Meandr>();
  }
  else if(sig_type == SIGNAL_TYPE_TRIANGLE) {
    return std::make_unique<Triangle>();
  }
  else if(sig_type == SIGNAL_TYPE_SAW) {
    return std::make_unique<Saw>();
  }
  return std::make_unique<Sinus>();
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

// class Meandr ---------------------------------------------------------------

float Meandr::operator()(uint32_t time) const {
  return amp_ * std::sin(pi * freq_ * time);
}
  
float Meandr::FM(uint32_t time, Signal& fmod) const {
    return amp_ * std::sin(pi * freq_ * time + fmod.GetIntegral(time));
}
  
float Meandr::GetIntegral(uint32_t time) const {
      return amp_ * std::cos(pi * freq_ * time);
}

// class Triangle ---------------------------------------------------------------

float Triangle::operator()(uint32_t time) const {
  return amp_ * std::sin(pi * freq_ * time);
}
  
float Triangle::FM(uint32_t time, Signal& fmod) const {
    return amp_ * std::sin(pi * freq_ * time + fmod.GetIntegral(time));
}
  
float Triangle::GetIntegral(uint32_t time) const {
      return amp_ * std::cos(pi * freq_ * time);
}

// class Saw ---------------------------------------------------------------

float Saw::operator()(uint32_t time) const {
  return amp_ * std::sin(pi * freq_ * time);
}
  
float Saw::FM(uint32_t time, Signal& fmod) const {
    return amp_ * std::sin(pi * freq_ * time + fmod.GetIntegral(time));
}
  
float Saw::GetIntegral(uint32_t time) const {
      return amp_ * std::cos(pi * freq_ * time);
}
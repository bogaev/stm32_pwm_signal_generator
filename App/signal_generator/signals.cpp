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
    return std::make_unique<Square>();
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

float Sinus::operator()(float x) {
  return amp_ * std::sin(pi * freq_ * x);
}
  
float Sinus::FM(float x, Signal& fmod) {
    return amp_ * std::sin(pi * freq_ * x + fmod.GetIntegral(x));
}
  
float Sinus::GetIntegral(float x) {
      return amp_ * std::cos(pi * freq_ * x);
}

// class Square ---------------------------------------------------------------

float Square::operator()(float time) {
  float time_ = (int)time % (int)period_;
  return square(time_);
}
  
float Square::FM(float time, Signal& fmod) {
  float time_ = (int)time % (int)period_;
  return square(time_ + fmod.GetIntegral(time_));
}
  
float Square::GetIntegral(float x) {
  if(x < period_ / 2) {
    return x;
  }
  return -x;
}

float Square::square(float x) {
  if(x < period_ / 2) {
    return amp_;
  }
  return -amp_;
}

// class Triangle ---------------------------------------------------------------

float Triangle::operator()(float x) {
  float time_ = (int)x % (int)period_;
  return amp_ * triangle(time_);
}

float Triangle::FM(float x, Signal& fmod) {
  float time_ = (int)x % (int)period_;
  return amp_ * triangle(time_ + fmod.GetIntegral(time_));
}

float Triangle::GetIntegral(float x) {
  if(x < period_ / 2) {
    return x * (2*x / period_ - 1);
  }
  return x * (3 - 2*x / period_);
}

float Triangle::triangle(float x) {
  if(x < period_ / 2) {
    return 4*x / period_ - 1;
  }
  return 3 - 4*x / period_;
}

// class Saw ---------------------------------------------------------------

float Saw::operator()(float x) {
  float time_ = (int)x % (int)period_;
  return amp_ * sawtooth(time_);
}

float Saw::FM(float x, Signal& fmod) {
  float time_ = (int)x % (int)period_;
  return amp_ * sawtooth(time_ + amp_ * fmod.GetIntegral(time_));
}

float Saw::GetIntegral(float x) {
  return x * (x - period_) / period_;
}

float Saw::sawtooth(float x) {
  return 2 * x / period_ - 1;
}
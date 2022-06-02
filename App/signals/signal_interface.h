#ifndef _SIGNALS_INTERFACE_H_
#define _SIGNALS_INTERFACE_H_

#include <cmath>

const float pi = std::acos(-1);

class Signal {
public:
  Signal& SetPeriod(float period) {
    period_ = period;
    freq_ = 1 / period_;
    return *this;
  }
  
  Signal& SetFreq(float freq) {
    freq_ = freq;
    period_ = 1 / freq_;
    return *this;
  }
  
  Signal& SetAmp(float amp) {
    amp_ = amp;
    return *this;
  }
  
  float GetFreq() {
    return freq_;
  }
  
  float GetPeriod() {
    return period_;
  }
  
  float GetAmp() {
    return amp_;
  }
  
  virtual float operator()(uint32_t time) const = 0;
  virtual float FM(uint32_t time, Signal& sig) const = 0;
  virtual float getIntegral(uint32_t time) const = 0;

protected:
  float freq_ = 1;
  float period_ = 1;
  float amp_ = 1;
};

#endif // #ifndef _SIGNALS_INTERFACE_H_
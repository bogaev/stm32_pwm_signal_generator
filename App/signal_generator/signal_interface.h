#ifndef _SIGNALS_INTERFACE_H_
#define _SIGNALS_INTERFACE_H_

#include <cmath>
#include <memory>

const float pi = std::acos(-1);

class Signal {
public:
  Signal& SetPeriod(float period) {
    period_ = period;
    freq_ = 1 / period_;
    return *this;
  }

  float GetAmp() {
    return amp_;
  }

  float GetFreq() {
    return freq_;
  }
  
  float GetPeriod() {
    return period_;
  }
  
  Signal& SetAmp(float amp) {
    amp_ = amp;
    return *this;
  }
  
  Signal& SetFreq(float freq) {
    freq_ = freq;
    period_ = 1 / freq_;
    return *this;
  }
  
  void setParam(uint8_t param, uint16_t value) {
    if(param == (uint8_t)UART_PARAM_SIGNAL_TYPE) {
//      p.signal_type = value;
    }
    else if(param == (uint8_t)UART_PARAM_AMP) {
      amp_ = value;
    }
    else if(param == (uint8_t)UART_PARAM_FREQ) {
      freq_ = value;
    }
  }

  virtual bool is_virtual() const {
    return true;
  }
  
  virtual float operator()(uint32_t time) const = 0;
  virtual float FM(uint32_t time, Signal& sig) const = 0;
  virtual float GetIntegral(uint32_t time) const = 0;
  virtual std::unique_ptr<Signal> Create() const = 0;

protected:
  float freq_ = 1;
  float period_ = 1;
  float amp_ = 1;
};

#endif // #ifndef _SIGNALS_INTERFACE_H_
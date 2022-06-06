#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include "App/pwm_types.h"

#include <cmath>
#include <memory>

const float pi = std::acos(-1);

class Signal {
public:
  Signal& SetPeriod(float period);
  Signal& SetAmp(float amp);
  Signal& SetFreq(float freq);
  float GetAmp();
  float GetFreq();
  float GetPeriod();
  void setParam(uint8_t param, uint16_t value);
  std::unique_ptr<Signal> Create(tdSignalTypes_ sig_type);
  
  virtual float operator()(uint32_t time) const { return 0; };
  virtual float FM(uint32_t time, Signal& sig) const { return 0; };
  virtual float GetIntegral(uint32_t time) const { return 0; };

protected:
  float freq_ = 1;
  float period_ = 1;
  float amp_ = 1;
};

class Sinus : public Signal {
public:
  float operator()(uint32_t time) const override;
  float FM(uint32_t time, Signal& fmod) const override;
  float GetIntegral(uint32_t time) const override;
};

#endif // #ifndef _SIGNALS_H_
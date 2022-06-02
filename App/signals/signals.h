#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include "signal_interface.h"

class Sinus : public Signal {
public:
  float operator()(uint32_t time) const override {
    return amp_ * std::sin(pi * freq_ * time);
  }
  
  float FM(uint32_t time, Signal& fmod) const override {
    return amp_ * std::sin(pi * freq_ * time + fmod.getIntegral(time));
  }
  
  float getIntegral(uint32_t time) const override {
      return amp_ * std::cos(pi * freq_ * time);
  }
  
private:
  
};

#endif // #ifndef _SIGNALS_H_
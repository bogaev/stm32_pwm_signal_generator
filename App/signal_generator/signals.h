#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include "signal_interface.h"

class NoSignal : public Signal {
public:
  float operator()(uint32_t time) const override {
    return 0;
  }
  
  float FM(uint32_t time, Signal& fmod) const override {
    return 0;
  }
  
  float GetIntegral(uint32_t time) const override {
    return 0;
  }
  
  bool is_virtual() const override {
    return true;
  }
  
  std::unique_ptr<Signal> Create() const override {
    return std::unique_ptr<NoSignal>(new NoSignal{});
  }
};

class Sinus : public Signal {
public:
  float operator()(uint32_t time) const override {
    return amp_ * std::sin(pi * freq_ * time);
  }
  
  float FM(uint32_t time, Signal& fmod) const override {
    return amp_ * std::sin(pi * freq_ * time + fmod.GetIntegral(time));
  }
  
  float GetIntegral(uint32_t time) const override {
      return amp_ * std::cos(pi * freq_ * time);
  }
  
  virtual bool is_virtual() const override {
    return false;
  }
  
  std::unique_ptr<Signal> Create() const override {
    return std::unique_ptr<Sinus>(new Sinus{});
  }
};

#endif // #ifndef _SIGNALS_H_
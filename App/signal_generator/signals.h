#ifndef _SIGNALS_H_
#define _SIGNALS_H_

#include "App/pwm_types.h"

#include <cmath>
#include <memory>

/*
https://math.stackexchange.com/questions/178079/integration-of-sawtooth-square-and-triangle-wave-functions
*/

const float pi = std::acos(-1);
float square(float x);
float triangle(float x);

class Signal {
public:
  virtual ~Signal() {};
  void SetParam(uint8_t param, uint16_t value);
  Signal& SetAmp(float amp);
  Signal& SetFreq(float freq);
  Signal& SetPeriod(float period);
  float GetAmp();
  float GetFreq();
  float GetPeriod();
  std::unique_ptr<Signal> Create(tdSignalTypes_ sig_type);
  
  virtual float operator()(float time) { return 0; };
  virtual float FM(float time, Signal& sig) { return 0; };
  virtual float GetIntegral(float time) { return 0; };

protected:
  float freq_ = 1;
  float period_ = 1;
  float amp_ = 1;
};

class Sinus : public Signal {
public:
  float operator()(float time) override;
  float FM(float time, Signal& fmod) override;
  float GetIntegral(float time) override;
};

class Square : public Signal {
public:
  float operator()(float time) override;
  float FM(float time, Signal& fmod) override;
  float GetIntegral(float time) override;
  
private:
  float square(float x);
};

class Triangle : public Signal {
public:
  float operator()(float time) override;
  float FM(float time, Signal& fmod) override;
  float GetIntegral(float time) override;
  
private:
  float triangle(float x);
};

class Saw : public Signal {
public:
  float operator()(float time) override;
  float FM(float time, Signal& fmod) override;
  float GetIntegral(float time) override;
  
private:
  float sawtooth(float x);
};

#endif // #ifndef _SIGNALS_H_
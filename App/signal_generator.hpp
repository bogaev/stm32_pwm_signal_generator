#ifndef _SIGNAL_GENERATOR_H_
#define _SIGNAL_GENERATOR_H_

#include "main.h"
#include "pwm_types.h"
#include <cmath>

const uint16_t HALFWAVE_MAX_SIZE = (1<<13); //8192
const uint16_t POINTS_PER_HW_MIN = 128;
const uint16_t POINTS_PER_HW_MAX = HALFWAVE_MAX_SIZE;
const uint32_t DATA_BUFFER_SIZE = HALFWAVE_MAX_SIZE*2;
const uint16_t MAX_FREQ_COUNT = POINTS_PER_HW_MAX / POINTS_PER_HW_MIN;
const float pi = std::acos(-1);

struct tdSignalParams {
  uint8_t signal_type = (uint8_t)SIGNAL_TYPE_SINUS;
  uint16_t amp = 1;
  uint16_t freq = MAX_FREQ_COUNT;
  
  inline float timeDelta() const {
    return 1.0f / (float)HALFWAVE_MAX_SIZE;
  }
};

class SignalGenerator {
  tdSignalParams p;
  uint32_t time = 0;
  uint16_t f = 1;
  uint16_t f_buf = 1;
  int16_t sign = -1;
  uint16_t amp = 1;
  
public:
  SignalGenerator(tdSignalParams params = {});
  
  float getNext();
  float generateSinus();
  float generateMeandr();
  float generateTriangle();
  float generateSaw();

  inline uint16_t* getAmpPtr() {
    return &p.amp;
  }
  inline uint16_t* getFreqPtr() {
    return &f;
  }
  inline void switchFreq() {
    if(f != f_buf) {
      f = f_buf;
    }
  }
  inline uint32_t getTime() const {
    return time;
  }
  inline uint8_t getSignalType() const {
    return p.signal_type;
  }

  void setParam(uint8_t param, uint16_t value);
  
  SignalGenerator& operator^(SignalGenerator* freq_mod) {
    if(freq_mod->getSignalType() != SIGNAL_TYPE_NONE) {
      if(f == f_buf) {
        f_buf = (uint16_t) ((float)p.freq * (1.f - (freq_mod->getNext() * 0.98f)) + 1.f);
      }
    }
    return *this;
  }
  
  float operator*(SignalGenerator* amp_mod) {
    if(amp_mod->getNext() != -1.f) {
      return getNext() * amp_mod->getNext();
    }
    return getNext();
  }
  
private:
  void timeStep();
  uint16_t getEvenHalfwavesSize();
  inline void toggleSign(uint8_t dev);
  inline void resetSignal();
};

#endif // #ifndef _SIGNAL_GENERATOR_H_
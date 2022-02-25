#ifndef _SIGNAL_GENERATOR_H_
#define _SIGNAL_GENERATOR_H_

#include "main.h"
#include <cmath>

const uint16_t SIGNAL_MAX_SIZE = (1<<13); //8192
const uint16_t SIGNAL_MIN_SAMPLE_RATE = 128;
const uint16_t SIGNAL_MAX_SAMPLE_RATE = SIGNAL_MAX_SIZE;
const uint32_t DMA_BUFFER_SIZE = SIGNAL_MAX_SIZE*2;
const float pi = std::acos(-1);

typedef enum tdSignalTypes_ {
  SIGNAL_TYPE_SINUS = 0,
  SIGNAL_TYPE_CONST
} tdSignalTypes;

struct tdSignalParams {
  tdSignalTypes signal_type = SIGNAL_TYPE_SINUS;
  float freq = (float)(SIGNAL_MAX_SAMPLE_RATE / SIGNAL_MIN_SAMPLE_RATE);
  
  inline float phaseDelta() const {
    return 1.0f / (float)SIGNAL_MAX_SIZE;
  }
};

class SignalGenerator {
  tdSignalParams p;
  uint32_t phase = 0;
  uint16_t f = 1;
  uint16_t f_buf = 1;
  
public:
  SignalGenerator(tdSignalParams params = {});
  
  float getNext();
  float generateNextSinus(const SignalGenerator* freq_mod = nullptr);

  inline uint16_t* getFreqPtr() {
    return &f;
  }
  
  inline uint32_t getPhase() const {
    return phase;
  }
  
  inline void setFreq(float freq) {
    p.freq = freq;
  }
  
  SignalGenerator& operator^(SignalGenerator* freq_mod) {
    if(f == f_buf) {
      f_buf = (int) (p.freq * (1.f - (freq_mod->getNext() * 0.98f)));
    }
    return *this;
  }
  
  float operator*(SignalGenerator* amp_mod) {
    return getNext() * amp_mod->getNext();
  }
  
private:
  inline void phaseStep() {
    ++phase;
    if(phase % getEvenHalfwavesCount() == 0) { 
      phase = 0;
      if(f != f_buf) {
        f = f_buf;
      }
    }
  }
  
  uint16_t getEvenHalfwavesCount() {
    uint16_t even_periods = f;
    if ((even_periods > 1U)
         && (((uint16_t)even_periods % 2U) != 0U)
        ) {
      --even_periods;
    }
    return ((SIGNAL_MAX_SIZE / f) * even_periods);
  }
};

#endif // #ifndef _SIGNAL_GENERATOR_H_
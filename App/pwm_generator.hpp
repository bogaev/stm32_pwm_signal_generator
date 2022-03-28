#ifndef _PWM_GENERATOR_H_
#define _PWM_GENERATOR_H_

#include "main.h"
#include <cmath>

#include "signal_generator.hpp"

const float TIMER_PERIOD = 656.f;

struct tdDataBuffers {
  uint16_t* buffer[2] = { nullptr, nullptr };
};

const float amp_coeff[3][2] = {
  {1.0f, 0.15f},
  {0.6f, 1.7f},
  {0.45f, 1.7f}
};

#define AMP_GAIN amp_coeff[(int)*a-1][0]
#define FREQ_SLOPE amp_coeff[(int)*a-1][1]

struct tdDutyCycle {
  float cmin;
  float cmax;
  float timer_period;
  float* freq = nullptr;
  float* a = nullptr;
  float freq_slope = 0.15f;
  
  inline float freq_corr(float slope) const {
    return (*freq / MAX_FREQ_COUNT) * slope + (1.f - slope);
  }
  inline float amp_corr() const {
    return *a * AMP_GAIN + (1.f - AMP_GAIN) * freq_corr(FREQ_SLOPE);
  }
  inline float min() const {
    return cmin * freq_corr(freq_slope);
  }
  inline float max() const {
    return cmax * freq_corr(freq_slope);
  }
  inline float range() const {
    return max() - min();
  }
};

class PwmGenerator {
    SignalGenerator* carrier = nullptr;
    SignalGenerator* amp_mod = nullptr;
    SignalGenerator* freq_mod = nullptr;
    tdDataBuffers& buffers_;
    tdDutyCycle dutyCycle_;
    uint8_t is_negative_halfwave = 0;
    uint32_t buffer_index = 0;
    
public:
    PwmGenerator(SignalGenerator* params[], tdDutyCycle dutyCycle, tdDataBuffers& buffers);
    void setSignal(uint8_t signal, uint8_t param, uint16_t value);
    float getNext();
    inline uint16_t getDutyCycle();
    void updateBuffer();
    void generateNextHalfbuffer();
    
private:
    inline void zeroCrossingCheck() {
      uint16_t value = 
        carrier->getTime() % (uint16_t)((float)HALFWAVE_MAX_SIZE / *(dutyCycle_.freq));
      if(value == 0) {
        is_negative_halfwave = !is_negative_halfwave;
      }
    }

};

#endif // #ifndef _PWM_GENERATOR_H_
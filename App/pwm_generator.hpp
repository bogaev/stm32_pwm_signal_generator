#ifndef _PWM_GENERATOR_H_
#define _PWM_GENERATOR_H_

#include "main.h"
#include <cmath>

#include "signal_generator.hpp"

const float TIMER_PERIOD = 656.f;

struct tdDMABuffers {
  uint16_t* buffer[2] = { nullptr, nullptr };
};

struct tdDutyCycle {
  uint16_t cmin;
  uint16_t cmax;
  uint16_t* freq = nullptr;
  float min_coeff = 0.25f;
  float max_coeff = 0.10f;
  const float FREQ_RANGE = SIGNAL_MAX_SAMPLE_RATE / SIGNAL_MIN_SAMPLE_RATE - 1;
  
  inline uint16_t min() const {
    return (uint16_t)((float)cmin
                       * (((*freq-1.f) / FREQ_RANGE) 
                             * min_coeff + 1.f
                         )
                      );
  }
  
  inline uint16_t max() const {
    return (uint16_t)((float)cmax
                       * (((*freq-1.f) / FREQ_RANGE) 
                             * max_coeff + 1.f
                         )
                      );
  }
  
  inline uint16_t range() const {
    return max() - min();
  }
};

class PwmGenerator {
    SignalGenerator* carrier = nullptr;
    SignalGenerator* amp_mod = nullptr;
    SignalGenerator* freq_mod = nullptr;
    tdDMABuffers& dma_;
    tdDutyCycle dutyCycle_;
    uint8_t is_negative_halfwave = 0;
    uint32_t buffer_index = 0;
    
public:
    PwmGenerator(SignalGenerator* params[], tdDutyCycle dutyCycle, tdDMABuffers& dma);
    float getNext();
    inline uint16_t getDutyCycle();
    void updateBuffer();
    void generateNextHalfbuffer();
    
private:
    inline void zeroCrossingCheck() {
      uint16_t value = 
        carrier->getPhase() % (uint16_t)((float)SIGNAL_MAX_SIZE / *(dutyCycle_.freq));
      if(value == 0) {
        is_negative_halfwave = !is_negative_halfwave;
      }
    }

};

#endif // #ifndef _PWM_GENERATOR_H_
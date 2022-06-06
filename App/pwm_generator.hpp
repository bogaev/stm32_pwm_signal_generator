#ifndef _PWM_GENERATOR_H_
#define _PWM_GENERATOR_H_

#include "main.h"
#include <cmath>

#include "App/signal_generator/signal_generator.h"

const float TIMER_PERIOD = 656.f;

struct tdDataBuffers {
  uint16_t* buffer[2] = { nullptr, nullptr };
};

const float amp_coeff[3][2] = {
  {1.0f, 0.15f},
  {0.6f, 1.7f},
  {0.45f, 1.7f}
};

#define AMP_GAIN amp_coeff[(int)sig_.carrier_->GetAmp()-1][0]
#define FREQ_SLOPE amp_coeff[(int)sig_.carrier_->GetAmp()-1][1]

struct tdDutyCycleSettings {
  float min;
  float max;
  float timer_period;
};

struct tdCorrector {
  SignalGenerator& sig_;
  
  inline float freq_corr(float slope) const {
    return (sig_.carrier_->GetFreq() / MAX_FREQ_COUNT) * slope + (1.f - slope);
  }
  
  inline float amp_corr() const {
    return sig_.carrier_->GetAmp() * AMP_GAIN + (1.f - AMP_GAIN) * freq_corr(FREQ_SLOPE);
  }
};

class tdDutyCycle {
public:
  tdDutyCycle(tdDutyCycleSettings settings, tdCorrector& c)
    : min_(settings.min),
      max_(settings.max),
      timer_period_(settings.timer_period),
      c_(c)
  {
    min_ /= 100.f * timer_period_;
    max_ /= 100.f * timer_period_;
  }
  
  inline float min() const {
    return min_ * c_.freq_corr(freq_slope_);
  }
  
  inline float max() const {
    return max_ * c_.freq_corr(freq_slope_);
  }
  
  inline float range() const {
    return max() - min();
  }
  
  inline float amp_corr() const {
    return c_.amp_corr();
  }
  
private:
  float min_;
  float max_;
  float timer_period_;
  tdCorrector& c_;
  float freq_slope_ = 0.15f;
};

class PwmGenerator {    
public:
  PwmGenerator(SignalGenerator& sig_generator,
                const tdDutyCycleSettings dutyCycle, tdDataBuffers& buffers);
  void setSignal(uint8_t signal, uint8_t param, uint16_t value);
  inline uint16_t getDutyCycle();
  void updateBuffer();
  void generateNextHalfbuffer();
  
private:
  inline void zeroCrossingCheck(float value);
  void reset();
  
  SignalGenerator& sig_generator_;
  tdCorrector corrector_;
  tdDutyCycle dc_;
  tdDataBuffers& buffers_;
  uint8_t is_negative_halfwave = 0;
  uint32_t buffer_index = 0;
};

#endif // #ifndef _PWM_GENERATOR_H_
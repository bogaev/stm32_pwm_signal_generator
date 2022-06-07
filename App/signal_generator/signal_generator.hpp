#ifndef _SIGNAL_GENERATOR_H_
#define _SIGNAL_GENERATOR_H_

#include "main.h"

#include "App/pwm_types.h"
#include "App/signal_generator/signals.hpp"

#include <cmath>
#include <utility>
#include <memory>

const uint16_t HALFWAVE_MAX_SIZE = (1<<13); //8192
const uint16_t POINTS_PER_HW_MIN = 128;
const uint16_t POINTS_PER_HW_MAX = HALFWAVE_MAX_SIZE;
const uint32_t DATA_BUFFER_SIZE = HALFWAVE_MAX_SIZE*2;
const uint16_t MAX_FREQ_COUNT = POINTS_PER_HW_MAX / POINTS_PER_HW_MIN;

class SignalGenerator {  
public:
  
  SignalGenerator();
  
  explicit SignalGenerator(tdSignalTypes_ carrier,
                           tdSignalTypes_ fmod = SIGNAL_TYPE_NONE,
                           tdSignalTypes_ amod = SIGNAL_TYPE_NONE);
  
  explicit SignalGenerator(std::shared_ptr<Signal> carrier, 
                           std::shared_ptr<Signal> fmod = nullptr,
                           std::shared_ptr<Signal> amod = nullptr);
  
  float operator()();
  SignalGenerator& GenerateCarrier();
  SignalGenerator& AddAM();
  SignalGenerator& GenerateFM();
  float GetAmp();
  float GetFreq();
  void SetSignal(uint8_t signal, uint8_t param, uint16_t value);

  std::shared_ptr<Signal> carrier_{};
  std::shared_ptr<Signal> amod_{};
  std::shared_ptr<Signal> fmod_{};
  
private:
  
  void Reset();
  
  float value_ = 0;
  uint32_t time_ = 0;
};

#endif // #ifndef _SIGNAL_GENERATOR_H_
#include "signal_generator.hpp"
#include <assert.h>

SignalGenerator::SignalGenerator(tdSignalParams params)
  : p(params)
{
  assert(p.freq >= 1.f);
  assert(p.freq <= POINTS_PER_HW_MAX / POINTS_PER_HW_MIN);
  assert(p.amp <= 1.0f && p.amp > 0.0f);
  f = (int)p.freq;
  f_buf = f;
}

float SignalGenerator::getNext() {
  if(p.signal_type == SIGNAL_TYPE_SINUS) {
    return generateNextSinus();
  }
  else if (p.signal_type == SIGNAL_TYPE_NONE) {
    return -1.f;
  }
  else {
    return generateNextSinus();
  }
}

float SignalGenerator::generateNextSinus(const SignalGenerator* freq_mod) {
  float value = std::abs(p.amp * sin(pi * (float)f * p.timeDelta() * (float)time));
  timeStep();
  return value;
}

void SignalGenerator::timeStep() {
  ++time;
  if(time % getEvenHalfwavesCount() == 0) { 
    time = 0;
    if(f != f_buf) {
      f = f_buf;
    }
  }
}
  
uint16_t SignalGenerator::getEvenHalfwavesCount() {
  uint16_t even_periods = f;
  if ((even_periods > 1U)
       && (((uint16_t)even_periods % 2U) != 0U)
      ) {
    --even_periods;
  }
  return ((HALFWAVE_MAX_SIZE / f) * even_periods);
}
    
void SignalGenerator::setParam(uint8_t param, uint16_t value) {
  if(param == (uint8_t)UART_PARAM_SIGNAL_TYPE) {
    p.signal_type = value;
  }
  else if(param == (uint8_t)UART_PARAM_AMP) {
    p.amp = value;
  }
  else if(param == (uint8_t)UART_PARAM_FREQ) {
    p.freq = value;
    f_buf = (uint16_t)p.freq;
  }
}
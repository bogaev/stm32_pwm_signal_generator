#include "signal_generator.hpp"
#include <assert.h>

SignalGenerator::SignalGenerator(tdSignalParams params)
  : p(params)
{
  assert(p.freq >= 1.f);
  assert(p.freq <= POINTS_PER_HW_MAX / POINTS_PER_HW_MIN);
//  assert(p.amp <= 1.0f && p.amp > 0.0f);
  f = (uint16_t)p.freq;
  f_buf = f;
}

float SignalGenerator::getNext() {
  if(p.signal_type == SIGNAL_TYPE_SINUS) {
    return generateSinus();
  }
  else if(p.signal_type == SIGNAL_TYPE_MEANDR) {
    return generateMeandr();
  }
  else if(p.signal_type == SIGNAL_TYPE_TRIANGLE) {
    return generateTriangle();
  }
  else if(p.signal_type == SIGNAL_TYPE_SAW) {
    return generateSaw();
  }
  else if (p.signal_type == SIGNAL_TYPE_NONE) {
    return -1.f;
  }
  else {
    return generateSinus();
  }
}

float SignalGenerator::generateSinus() {
  float value = std::abs(p.amp * sin(pi * (float)f * p.timeDelta() * (float)time));
  timeStep();
  return value;
}

float SignalGenerator::generateMeandr() {
  float value = std::abs((time % f * p.timeDelta()) - p.amp);
  timeStep();
  return value;
}

float SignalGenerator::generateTriangle() {
  float value = std::abs(time * p.timeDelta() * p.amp + sign * amp) * 2.f;
  toggleSign(2);
  timeStep();
  return value;
}

float SignalGenerator::generateSaw() {
  float value = std::abs(time * p.timeDelta() * p.amp + sign * amp);
  toggleSign(1);
  timeStep();
  return value;
}

void SignalGenerator::timeStep() {
  ++time;
  if(time % getEvenHalfwavesSize() == 0) { 
    time = 0;
    if(f != f_buf) {
      f = f_buf;
    }
  }
}
  
uint16_t SignalGenerator::getEvenHalfwavesSize() {
  uint16_t even_periods = (uint16_t)f;
  if ((even_periods > 1U)
       && (((uint16_t)even_periods % 2U) != 0U)
      ) {
    --even_periods;
  }
  return ((HALFWAVE_MAX_SIZE / (uint16_t)f) * even_periods);
}

void SignalGenerator::setParam(uint8_t param, uint16_t value) {
  resetSignal();
  if(param == (uint8_t)UART_PARAM_SIGNAL_TYPE) {
    p.signal_type = value;
  }
  else if(param == (uint8_t)UART_PARAM_AMP) {
    p.amp = value;
  }
  else if(param == (uint8_t)UART_PARAM_FREQ) {
    p.freq = value;
//    f_buf = (uint16_t)p.freq;
//    f = f_buf;
  }
}

void SignalGenerator::toggleSign(uint8_t dev) {
  if(time % (getEvenHalfwavesSize() / f / dev) == 0) {
    sign *= -1;
    amp = (sign > 0 ? 0 : p.amp);
  }
}

void SignalGenerator::resetSignal() {
  sign = -1;
  amp = 1;
  f_buf = (uint16_t)p.freq;
  f = f_buf;
  time = 0;
}

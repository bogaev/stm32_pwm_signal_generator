#include "signal_generator.hpp"
#include <assert.h>
#include <cmath>

SignalGenerator::SignalGenerator(tdSignalParams params)
  : p(params)
{
  assert(p.freq >= 1.f);
  assert(p.freq <= POINTS_PER_HW_MAX / POINTS_PER_HW_MIN);
  resetSignal();
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
  float value = std::abs(p.amp * sin(pi * f * ampStep() * time));
  timeStep();
  return value;
}

float SignalGenerator::generateMeandr() {
  float value = std::abs(time % f * ampStep() - p.amp);
  timeStep();
  return value;
}

float SignalGenerator::generateTriangle() {
  float value = std::abs((time % period())
                            * ampStep() * p.amp
                                + sign * amp) * 2;
  timeStep();
  checkSign(2);
  return value;
}

float SignalGenerator::generateSaw() {
  float value = std::abs((time % period())
                            * ampStep() * p.amp
                                + sign * amp);
  timeStep();
  checkSign(1);
  return value;
}

void SignalGenerator::checkSign(uint16_t dev) {
  if((time % (int)(ceil((float)period() / (float)dev))) == 0) {
    sign *= -1;
    amp = (sign > 0 ? 0 : p.amp);
  }
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

bool SignalGenerator::zeroCrossingCheck() {
  if(time % period() == 0) { 
    return true;
  }
  return false;
}

uint16_t SignalGenerator::getEvenHalfwavesSize() {
  uint16_t even_periods = f;
  if ((even_periods > 1U)
       && (((uint16_t)even_periods % 2U) != 0U)
      ) {
    --even_periods;
  }
  return ((HALFWAVE_MAX_SIZE / f) * even_periods);
}

uint16_t SignalGenerator::period() {
  return HALFWAVE_MAX_SIZE / f;
}

float SignalGenerator::ampStep() {
  if(p.signal_type == SIGNAL_TYPE_SINUS
     || p.signal_type == SIGNAL_TYPE_MEANDR) {
        return 1.f / (float)HALFWAVE_MAX_SIZE;
     } 
  else {
        return 1.f / (float)period();
  }
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
  }
  resetSignal();
}

void SignalGenerator::resetSignal() {
  (p.signal_type == SIGNAL_TYPE_TRIANGLE) ? initTriangle() : initDefault();
  f_buf = (uint16_t)p.freq;
  f = f_buf;
  time = 0;
}

void SignalGenerator::initTriangle() {
  sign = 1;
  amp = 0;
}

void SignalGenerator::initDefault() {
  sign = -1;
  amp = 1;
}

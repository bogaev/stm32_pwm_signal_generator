#include "signal_generator.hpp"
#include <assert.h>

SignalGenerator::SignalGenerator(tdSignalParams params)
  :
    p(params)
{
  assert(p.freq >= 1.f);
  assert(p.freq <= SIGNAL_MAX_SAMPLE_RATE / SIGNAL_MIN_SAMPLE_RATE);
  f = (int)p.freq;
  f_buf = f;
}

float SignalGenerator::getNext() {
  if(p.signal_type == SIGNAL_TYPE_SINUS) {
    return generateNextSinus();
  }
  else if (p.signal_type == SIGNAL_TYPE_CONST) {
    return 1.f;
  }
  else {
    return generateNextSinus();
  }
}

float SignalGenerator::generateNextSinus(const SignalGenerator* freq_mod) {
  float value = std::abs(sin(pi * (float)f * p.phaseDelta() * (float)phase));
  phaseStep();
  return value;
}

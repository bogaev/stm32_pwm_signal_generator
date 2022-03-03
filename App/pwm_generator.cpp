#include "pwm_generator.hpp"

PwmGenerator::PwmGenerator(SignalGenerator* params[],
                           tdDutyCycle dutyCycle, tdDataBuffers& dma) :
  carrier(params[0]),
  amp_mod(params[1]),
  freq_mod(params[2]),
  dma_(dma)
{
  dutyCycle_.cmin = (uint16_t)((float)dutyCycle.cmin / 100.f * TIMER_PERIOD);
  dutyCycle_.cmax = (uint16_t)((float)dutyCycle.cmax / 100.f * TIMER_PERIOD);
  dutyCycle_.freq = carrier->getFreqPtr();
  generateNextHalfbuffer();
  generateNextHalfbuffer();
}

float PwmGenerator::getNext() {
  return ((*carrier) ^ freq_mod) * amp_mod;
}

uint16_t PwmGenerator::getDutyCycle() {
  uint16_t dc = 
    (uint16_t)((float)dutyCycle_.min() + (float)dutyCycle_.range() * getNext());
  zeroCrossingCheck();
  return dc;
}

void PwmGenerator::updateBuffer() {
  dma_.buffer[is_negative_halfwave][buffer_index] = getDutyCycle();
  dma_.buffer[!is_negative_halfwave][buffer_index] = 0;
  ++buffer_index;
  if(buffer_index >= DATA_BUFFER_SIZE) {
    buffer_index = 0;
  }
}

void PwmGenerator::generateNextHalfbuffer() {
  for(uint16_t i = 0; i < DATA_BUFFER_SIZE / 2; ++i) {
    updateBuffer();
  }
}

#include "pwm_generator.hpp"

PwmGenerator::PwmGenerator(SignalGenerator* params[],
                           const tdDutyCycle dutyCycle, tdDataBuffers& buffers) :
  carrier(params[0]),
  amp_mod(params[1]),
  freq_mod(params[2]),
  buffers_(buffers)
{
  dutyCycle_.timer_period = dutyCycle.timer_period;
  dutyCycle_.cmin = dutyCycle.cmin / 100.f * dutyCycle.timer_period;
  dutyCycle_.cmax = dutyCycle.cmax / 100.f * dutyCycle.timer_period;
  dutyCycle_.a = carrier->getAmpPtr();
  dutyCycle_.freq = carrier->getFreqPtr();
  generateNextHalfbuffer();
  generateNextHalfbuffer();
}

float PwmGenerator::getNext() {
  return ((*carrier) ^ freq_mod) * amp_mod;
}

uint16_t PwmGenerator::getDutyCycle() {
  uint16_t dc = 
    (uint16_t)(dutyCycle_.min() + dutyCycle_.range() * dutyCycle_.amp_corr() * getNext());
  zeroCrossingCheck();
  return dc;
}

void PwmGenerator::updateBuffer() {
  buffers_.buffer[is_negative_halfwave][buffer_index] = getDutyCycle();
  buffers_.buffer[!is_negative_halfwave][buffer_index] = 0;
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

void PwmGenerator::setSignal(uint8_t signal, uint8_t param, uint16_t value) {
  if(signal == (uint8_t)UART_SIGNAL_CARRIER) {
    carrier->setParam(param, value);
  }
  else if(signal == (uint8_t)UART_SIGNAL_AMP_MOD) {
    amp_mod->setParam(param, value);
  }
  else if(signal == (uint8_t)UART_SIGNAL_FREQ_MOD) {
    freq_mod->setParam(param,value);
  }
}
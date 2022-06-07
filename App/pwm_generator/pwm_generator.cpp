#include "pwm_generator.hpp"

PwmGenerator::PwmGenerator(SignalGenerator& sig_generator,
                           const tdDutyCycleSettings settings,
                           tdDataBuffers& buffers) 
  : sig_generator_(sig_generator),
    corrector_({sig_generator_}),
    dc_({settings, corrector_}),
    buffers_(buffers)
{
  generateNextHalfbuffer();
  generateNextHalfbuffer();
}

uint16_t PwmGenerator::getDutyCycle() {
  float sig_value = sig_generator_();
  uint16_t dc = 
    (uint16_t)(dc_.min() + dc_.range() * dc_.amp_corr() * sig_value);
  zeroCrossingCheck(sig_value);
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

void PwmGenerator::SetSignal(uint8_t signal, uint8_t param, uint16_t value) {
  reset();
  sig_generator_.SetSignal(signal, param, value);
}

void PwmGenerator::zeroCrossingCheck(float value) {
  value > 0 ? is_negative_halfwave = 0 : is_negative_halfwave = 1;
}

void PwmGenerator::reset() {
  is_negative_halfwave = 0;
  buffer_index = 0;
  for(uint16_t i = 0; i < DATA_BUFFER_SIZE; ++i) {
    buffers_.buffer[is_negative_halfwave][i] = 0;
    buffers_.buffer[!is_negative_halfwave][i] = 0;
  }
}

#include "pwm_generator.hpp"

PwmGenerator::PwmGenerator(SignalGenerator& sig_generator,
                           const tdDutyCycleSettings settings,
                           tdDataBuffers& buffers) 
  : sig_generator_(sig_generator),
    corrector_({sig_generator_}),
    dc_({settings, corrector_}),
    buffers_(buffers)
{
  GenerateNextHalfbuffer();
  GenerateNextHalfbuffer();
}

float PwmGenerator::GetDutyCycle() {
  float sig_value = sig_generator_();
  float dc = 
    (uint16_t)(dc_.min() + dc_.range() * dc_.amp_corr() * sig_value);
  zeroCrossingCheck(sig_value);
  return dc;
}

void PwmGenerator::UpdateBuffer() {
  buffers_.ptr[is_negative_halfwave][buffer_index] = (uint16_t)std::abs(GetDutyCycle());
  buffers_.ptr[!is_negative_halfwave][buffer_index] = 0;
  ++buffer_index;
  if(buffer_index >= DATA_BUFFER_SIZE) {
    buffer_index = 0;
  }
}

void PwmGenerator::GenerateNextHalfbuffer() {
  for(uint16_t i = 0; i < DATA_BUFFER_SIZE / 2; ++i) {
    UpdateBuffer();
  }
}

void PwmGenerator::SetSignal(uint8_t signal, uint8_t param, uint16_t value) {
  reset();
  sig_generator_.SetSignal(signal, param, value);
}

inline void PwmGenerator::zeroCrossingCheck(float value) {
  value > 0 ? is_negative_halfwave = 0 : is_negative_halfwave = 1;
}

void PwmGenerator::reset() {
  is_negative_halfwave = 0;
  buffer_index = 0;
  for(uint16_t i = 0; i < DATA_BUFFER_SIZE; ++i) {
    buffers_.ptr[is_negative_halfwave][i] = 0;
    buffers_.ptr[!is_negative_halfwave][i] = 0;
  }
}

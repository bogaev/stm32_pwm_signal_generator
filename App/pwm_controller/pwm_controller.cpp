#include "pwm_controller.hpp"
#include "tim.h"

PwmController::PwmController(TIM_HandleTypeDef* timer,
                             tdPwmChannels channels,
                             PwmGenerator& generator)
  : timer_(timer),
    generator_(generator)
{
  channels_.positiveHalfwaveChannel = channels.positiveHalfwaveChannel;
  channels_.negativeHalfwaveChannel = channels.negativeHalfwaveChannel;
  HAL_TIM_PWM_Start_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Start_IT(timer_, channels_.negativeHalfwaveChannel);
}

PwmController::~PwmController() {
  Stop();
}

void PwmController::Stop() {
  HAL_TIM_PWM_Stop(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Stop_DMA(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_DMA(timer_, channels_.negativeHalfwaveChannel);
  is_started = false;
}

void PwmController::SetPWM(uint8_t signal, uint8_t param, uint16_t value) {
  generator_.SetSignal(signal, param, value);
}

//class DMA_PwmController ----------------------------------------------------

DMA_PwmController::DMA_PwmController(TIM_HandleTypeDef* timer,
                 tdPwmChannels channels,
                 PwmGenerator& generator)
: PwmController(timer, channels, generator)
{}

void DMA_PwmController::Start() {
  Stop();
  HAL_TIM_PWM_Start_DMA(timer_, channels_.positiveHalfwaveChannel, (uint32_t*)generator_.GetBufferPtr(0), DATA_BUFFER_SIZE);
  HAL_TIM_PWM_Start_DMA(timer_, channels_.negativeHalfwaveChannel, (uint32_t*)generator_.GetBufferPtr(1), DATA_BUFFER_SIZE);
  is_started = true;
}

void DMA_PwmController::Run() {
  if(!is_started) return;
  generator_.GenerateNextHalfbuffer();
}

//class IT_PwmController ----------------------------------------------------

IT_PwmController::IT_PwmController(TIM_HandleTypeDef* timer,
                 tdPwmChannels channels,
                 PwmGenerator& generator)
: PwmController(timer, channels, generator)
{}

void IT_PwmController::Start() {
  Stop();
  HAL_TIM_PWM_Start_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Start_IT(timer_, channels_.negativeHalfwaveChannel);
  buffer_index = 0;
  is_started = true;
}

void IT_PwmController::Run() {
  if(!is_started) return;
  if(is_buffer_used_) {
    generator_.GenerateNextHalfbuffer();
    GetValueFromBuffer();
  }
  else {
    GetValue();
  }
}

void IT_PwmController::SetUseBuffer(bool is_buffer_used) {
  is_buffer_used_ = is_buffer_used;
}

void IT_PwmController::GetValueFromBuffer() {
  __HAL_TIM_SET_COMPARE(timer_,
                        channels_.positiveHalfwaveChannel,
                        generator_.GetBufferPtr(0)[buffer_index]);
  __HAL_TIM_SET_COMPARE(timer_,
                        channels_.negativeHalfwaveChannel,
                        generator_.GetBufferPtr(1)[buffer_index]);
  ++buffer_index;
  if(buffer_index >= DATA_BUFFER_SIZE) {
    buffer_index = 0;
  }
}

void IT_PwmController::GetValue() {
  float dc = generator_.GetDutyCycle();
  if(dc > 0) {
    __HAL_TIM_SET_COMPARE(timer_,
                          channels_.positiveHalfwaveChannel,
                          (uint16_t)std::abs(dc));
  }
  else {
    __HAL_TIM_SET_COMPARE(timer_,
                          channels_.negativeHalfwaveChannel,
                          (uint16_t)std::abs(dc));
  }
}
#include "pwm_controller.hpp"
#include "tim.h"

PwmController* pwm[PWM_NUM];
PwmGenerator* pwmGenerator;

PwmController::PwmController(TIM_HandleTypeDef* timer, tdPwmChannels channels,
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
  HAL_TIM_PWM_Stop(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Stop_DMA(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_DMA(timer_, channels_.negativeHalfwaveChannel);
}

void PwmController::start_IT_buffer(const tdDataBuffers& buffers) {
  b_ = &buffers;
  IT_buffer_index = 0;
  is_started = 1U;
}

void PwmController::start_DMA(const tdDataBuffers& dma) {
  HAL_TIM_PWM_Stop_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Start_DMA(timer_, channels_.positiveHalfwaveChannel, (uint32_t*)dma.buffer[0], DATA_BUFFER_SIZE);
  HAL_TIM_PWM_Start_DMA(timer_, channels_.negativeHalfwaveChannel, (uint32_t*)dma.buffer[1], DATA_BUFFER_SIZE);
  is_started = 1U;
}

uint8_t PwmController::isStarted() {
  return is_started;
}

void PwmController::setPWM(uint8_t signal, uint8_t param, uint16_t value) {
  generator_.setSignal(signal, param, value);
}

void PwmController::nextValueFromBuffer() {
  __HAL_TIM_SET_COMPARE(timer_, channels_.positiveHalfwaveChannel, b_->buffer[0][IT_buffer_index]);
  __HAL_TIM_SET_COMPARE(timer_, channels_.negativeHalfwaveChannel, b_->buffer[1][IT_buffer_index]);
  ++IT_buffer_index;
  if(IT_buffer_index >= DATA_BUFFER_SIZE) {
    IT_buffer_index = 0;
  }
}

void InitPwmControllers() {
//  const int DUTY_CYCLE_MIN = 50;
//  const int DUTY_CYCLE_MAX = 65;

  const int DUTY_CYCLE_MIN = 60;
  const int DUTY_CYCLE_MAX = 75;
  const float TIMER_PERIOD = 656.f;

  static uint16_t dma_positive[DATA_BUFFER_SIZE] = {0};
  static uint16_t dma_negative[DATA_BUFFER_SIZE] = {0};
  static tdDataBuffers dma_buffers = { {dma_positive, dma_negative} };
  
  static SignalGenerator carrier({SIGNAL_TYPE_SINUS, 1});
  static SignalGenerator amp_mod({SIGNAL_TYPE_NONE});
//  static SignalGenerator amp_mod({SIGNAL_TYPE_SINUS});
//  static SignalGenerator freq_mod({SIGNAL_TYPE_SINUS, 1.0f, 1.0f});
  static SignalGenerator freq_mod({SIGNAL_TYPE_NONE});
  SignalGenerator* params[] = {&carrier, &amp_mod, &freq_mod};
  
  static PwmGenerator pwm_signal(params, 
                                 {DUTY_CYCLE_MIN, DUTY_CYCLE_MAX, TIMER_PERIOD},
                                 dma_buffers);
  pwmGenerator = &pwm_signal;

  static PwmController pwm0(&htim1, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_signal);
  pwm[TIM1_CH12_DMA] = &pwm0;
  static PwmController pwm1(&htim2, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_signal);
  pwm[TIM2_CH12_DMA] = &pwm1;
  static PwmController pwm2(&htim2, {TIM_CHANNEL_3, TIM_CHANNEL_4}, pwm_signal);
  pwm[TIM2_CH34_IT] = &pwm2;
  static PwmController pwm3(&htim3, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_signal);
  pwm[TIM3_CH12_IT] = &pwm3;
  static PwmController pwm4(&htim3, {TIM_CHANNEL_3, TIM_CHANNEL_4}, pwm_signal);
  pwm[TIM3_CH34_DMA] = &pwm4;
  static PwmController pwm5(&htim4, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_signal);
  pwm[TIM4_CH12_DMA] = &pwm5;
//  static PwmController pwm6(&htim4, {TIM_CHANNEL_3, TIM_CHANNEL_4}, pwm_signal);
//  pwm[TIM4_CH34_IT] = &pwm6;
  static PwmController pwm7(&htim8, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_signal);
  pwm[TIM8_CH12_IT] = &pwm7;
  static PwmController pwm8(&htim12, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_signal);
  pwm[TIM12_CH12_IT] = &pwm8;
  
  pwm[TIM1_CH12_DMA]->start_DMA(dma_buffers);
//  pwm[TIM2_CH12_DMA]->start_DMA(dma_buffers);
//  pwm[TIM2_CH34_IT]->start_IT_buffer(dma_buffers);
//  pwm[TIM3_CH12_IT]->start_IT_buffer(dma_buffers);
//  pwm[TIM3_CH34_DMA]->start_DMA(dma_buffers);
//  pwm[TIM4_CH12_DMA]->start_DMA(dma_buffers);
//////  pwm[TIM4_CH34_IT]->start_IT_buffer(dma_buffers);
//  pwm[TIM8_CH12_IT]->start_IT_buffer(dma_buffers);
//  pwm[TIM12_CH12_IT]->start_IT_buffer(dma_buffers);
}
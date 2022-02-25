#include "cmsis_os.h"
#include "signal_generator.hpp"
#include "pwm_controller.hpp"

extern osSemaphoreId_t GenerateNextHalfWaveSemaphoreHandle;
extern osSemaphoreId_t GeneratePwmSemaphoreHandle;
PwmController* pwm[PWM_NUM];
PwmGenerator* pwmGenerator;
volatile size_t counterPwmSemaphore;
volatile size_t counterNextHalfWaveSemaphore;
volatile size_t counterGeneratePwmSemaphore;
static void GiveSemaphore();
SignalGenerator* carrierPtr;

PwmController::PwmController(TIM_HandleTypeDef* timer, tdPwmChannels channels, PwmGenerator& signal)
  : timer_(timer),
    signal_(signal)
{
  channels_.positiveHalfwaveChannel = channels.positiveHalfwaveChannel;
  channels_.negativeHalfwaveChannel = channels.negativeHalfwaveChannel;
  HAL_TIM_PWM_Start_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Start_IT(timer_, channels_.negativeHalfwaveChannel);
}

PwmController::~PwmController() {
  HAL_TIM_PWM_Stop_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Stop_DMA(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_DMA(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Stop(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop(timer_, channels_.negativeHalfwaveChannel);
}

void PwmController::start_IT() {
  is_started = 1U;
}

void PwmController::start_DMA(const tdDMABuffers& dma) {
  HAL_TIM_PWM_Stop(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.positiveHalfwaveChannel);
  HAL_TIM_PWM_Stop_IT(timer_, channels_.negativeHalfwaveChannel);
  HAL_TIM_PWM_Start_DMA(timer_, channels_.positiveHalfwaveChannel, (uint32_t*)dma.buffer[0], DMA_BUFFER_SIZE);
  HAL_TIM_PWM_Start_DMA(timer_, channels_.negativeHalfwaveChannel, (uint32_t*)dma.buffer[1], DMA_BUFFER_SIZE);
  is_started = 1U;
}

uint8_t PwmController::isStarted()
{
  return is_started;
}

void InitPwmControllers() {
  const int DUTY_CYCLE_MIN = 70;
  const int DUTY_CYCLE_MAX = 85;

  static uint16_t dma_positive[DMA_BUFFER_SIZE] = {0};
  static uint16_t dma_negative[DMA_BUFFER_SIZE] = {0};
  static tdDMABuffers dma_buffers = { {dma_positive, dma_negative} };
  
  static SignalGenerator carrier({SIGNAL_TYPE_SINUS});
  static SignalGenerator amp_mod({SIGNAL_TYPE_CONST, 1.f});
//  static SignalGenerator amp_mod({SIGNAL_TYPE_SINUS, 1.f});
//  static SignalGenerator freq_mod({SIGNAL_TYPE_CONST, 1.f});
  static SignalGenerator freq_mod({SIGNAL_TYPE_SINUS, 1.f});
  SignalGenerator* params[] = {&carrier, &amp_mod, &freq_mod};
  carrierPtr = &carrier;
  
  static PwmGenerator signal(params, {DUTY_CYCLE_MIN, DUTY_CYCLE_MAX}, dma_buffers);
  pwmGenerator = &signal;

  static PwmController pwm0(&htim1, {TIM_CHANNEL_1, TIM_CHANNEL_2}, signal);
  pwm[0] = &pwm0;
  static PwmController pwm1(&htim2, {TIM_CHANNEL_1, TIM_CHANNEL_2}, signal);
  pwm[1] = &pwm1;
  static PwmController pwm2(&htim2, {TIM_CHANNEL_3, TIM_CHANNEL_4}, signal);
  pwm[2] = &pwm2;
  static PwmController pwm3(&htim3, {TIM_CHANNEL_1, TIM_CHANNEL_2}, signal);
  pwm[3] = &pwm3;
  static PwmController pwm4(&htim3, {TIM_CHANNEL_3, TIM_CHANNEL_4}, signal);
  pwm[4] = &pwm4;
  static PwmController pwm5(&htim4, {TIM_CHANNEL_1, TIM_CHANNEL_2}, signal);
  pwm[5] = &pwm5;
  static PwmController pwm6(&htim4, {TIM_CHANNEL_3, TIM_CHANNEL_4}, signal);
  pwm[6] = &pwm6;
  static PwmController pwm7(&htim8, {TIM_CHANNEL_1, TIM_CHANNEL_2}, signal);
  pwm[7] = &pwm7;
  static PwmController pwm8(&htim12, {TIM_CHANNEL_1, TIM_CHANNEL_2}, signal);
  pwm[8] = &pwm8;
  
  pwm[0]->start_DMA(dma_buffers);
}

void GiveSemaphore() {
  osSemaphoreRelease(GenerateNextHalfWaveSemaphoreHandle);
  counterNextHalfWaveSemaphore = osSemaphoreGetCount(GenerateNextHalfWaveSemaphoreHandle);
  if(counterNextHalfWaveSemaphore >= 20) {
    HAL_GPIO_WritePin(LED3_RED_GPIO_Port, LED3_RED_Pin, GPIO_PIN_SET);
  }
  else {
    HAL_GPIO_WritePin(LED3_RED_GPIO_Port, LED3_RED_Pin, GPIO_PIN_RESET);
  }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  GiveSemaphore();
}

void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim) {
//  GiveSemaphore();
}

void GenNextHalfwaveTask(void *argument) {
  /* Infinite loop */
  for(;;) {
    osSemaphoreAcquire(GenerateNextHalfWaveSemaphoreHandle, portMAX_DELAY);
    pwm[0]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
  }
}

void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(LED2_GREEN_GPIO_Port, LED2_GREEN_Pin);
  }
  /* USER CODE END StartDefaultTask */
}

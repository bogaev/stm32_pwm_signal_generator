#include "pwm_os_tasks.hpp"
#include "App/pwm_controller/pwm_controller.hpp"
#include "App/com_interface/com_interface.hpp"
#include "tim.h"

#include <memory>

extern osSemaphoreId_t GeneratePWMSemaphoreHandle;
extern osMutexId_t MutexChangeParamsHandle;
extern osMessageQueueId_t SignalGeneratorQueueHandle;
extern UART_HandleTypeDef huart5;
extern Uart uart;

volatile size_t counterNextHalfWaveSemaphore;
static void GiveSemaphore();
static PwmController* pwms[PWM_NUM];

void GiveSemaphore() {
  osSemaphoreRelease(GeneratePWMSemaphoreHandle);
  counterNextHalfWaveSemaphore = osSemaphoreGetCount(GeneratePWMSemaphoreHandle);
  if(counterNextHalfWaveSemaphore >= 20) {
    HAL_GPIO_WritePin(LED3_RED_GPIO_Port, LED3_RED_Pin, GPIO_PIN_SET);
  }
  else {
    HAL_GPIO_WritePin(LED3_RED_GPIO_Port, LED3_RED_Pin, GPIO_PIN_RESET);
  }
}

void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim) {
  if(htim == &htim1) {
    GiveSemaphore();
  }
}

void GeneratePWM(void *argument) {
  /* Infinite loop */
  for(;;) {
    osSemaphoreAcquire(GeneratePWMSemaphoreHandle, portMAX_DELAY);
    osMutexAcquire(MutexChangeParamsHandle, osWaitForever);
    for(auto& pwm : pwms) {
      pwm->Run();
    }
    osMutexRelease(MutexChangeParamsHandle);
  }
}

void StartDefaultTask(void *argument) {
  /* Infinite loop */
  for(;;) {
    HAL_GPIO_TogglePin(LED2_GREEN_GPIO_Port, LED2_GREEN_Pin);
    if(!__HAL_UART_GET_IT_SOURCE(&huart5, UART_IT_RXNE)) {
      uart.init();
    }
  }
}

/**
* @brief Function implementing the ChngSignalParam thread.
* @param argument: Not used
* @retval None
*/
void ChangeSignalParamsTask(void *argument)
{
  tdPwmData msg;
  osStatus_t status;
  
  for(;;) {
    // wait for message
    status = osMessageQueueGet(SignalGeneratorQueueHandle, &msg, NULL, osWaitForever);
    if (status == osOK) {
      osMutexAcquire(MutexChangeParamsHandle, osWaitForever);
      pwms[msg.emitter]->SetPWM(msg.signal, msg.param, msg.value);
      osMutexRelease(MutexChangeParamsHandle);
    }
    else {
      Error_Handler();
    }
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
    
  static SignalGenerator signal_generator;
  static PwmGenerator pwm_generator(signal_generator, 
                                 {DUTY_CYCLE_MIN, DUTY_CYCLE_MAX, TIMER_PERIOD},
                                 dma_buffers);

  pwms[TIM1_CH_1_2_DMA] = new DMA_PwmController(&htim1, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_generator, dma_buffers);
  pwms[TIM2_CH_1_2_DMA] = new DMA_PwmController(&htim2, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_generator, dma_buffers);
  pwms[TIM2_CH_3_4_IT] = new IT_PwmController(&htim2, {TIM_CHANNEL_3, TIM_CHANNEL_4}, pwm_generator);
  pwms[TIM3_CH_1_2_IT] = new IT_PwmController(&htim3, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_generator);
  pwms[TIM3_CH_3_4_DMA] = new DMA_PwmController(&htim3, {TIM_CHANNEL_3, TIM_CHANNEL_4}, pwm_generator, dma_buffers);
  pwms[TIM4_CH_1_2_DMA] = new DMA_PwmController(&htim4, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_generator, dma_buffers);
  pwms[TIM8_CH_1_2_IT] = new IT_PwmController(&htim8, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_generator);
  pwms[TIM12_CH_1_2_IT] = new IT_PwmController(&htim12, {TIM_CHANNEL_1, TIM_CHANNEL_2}, pwm_generator);
  
  for(auto& pwm : pwms) {
    pwm->Start();
  }
}
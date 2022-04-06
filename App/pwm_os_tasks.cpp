#include "pwm_os_tasks.hpp"
#include "pwm_controller.hpp"
#include "com_interface.hpp"
#include "tim.h"

extern osSemaphoreId_t GenerateHalfWaveSemaphoreHandle;
extern osMutexId_t MutexChangeParamsHandle;
extern osMessageQueueId_t SignalGeneratorQueueHandle;
extern UART_HandleTypeDef huart5;
extern Uart uart;

volatile size_t counterNextHalfWaveSemaphore;
static void GiveSemaphore();
extern PwmController* pwm[PWM_NUM];
extern PwmGenerator* pwmGenerator;

void GiveSemaphore() {
  osSemaphoreRelease(GenerateHalfWaveSemaphoreHandle);
  counterNextHalfWaveSemaphore = osSemaphoreGetCount(GenerateHalfWaveSemaphoreHandle);
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

void GenerateHalfwaveTask(void *argument) {
  /* Infinite loop */
  for(;;) {
    osSemaphoreAcquire(GenerateHalfWaveSemaphoreHandle, portMAX_DELAY);
    osMutexAcquire(MutexChangeParamsHandle, osWaitForever);
    pwm[TIM1_CH12_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
//    pwm[TIM2_CH12_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
//    pwm[TIM2_CH34_IT]->isStarted() ? pwm[TIM2_CH34_IT]->nextValueFromBuffer() : (void)0U;
//    pwm[TIM3_CH12_IT]->isStarted() ? pwm[TIM3_CH12_IT]->nextValueFromBuffer() : (void)0U;
//    pwm[TIM3_CH34_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
//    pwm[TIM4_CH12_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
////    pwm[TIM4_CH34_IT]->isStarted() ?  pwm[TIM4_CH34_IT]->nextValueFromBuffer() : (void)0U;
//    pwm[TIM8_CH12_IT]->isStarted() ?  pwm[TIM8_CH12_IT]->nextValueFromBuffer() : (void)0U;
//    pwm[TIM12_CH12_IT]->isStarted() ? pwm[TIM12_CH12_IT]->nextValueFromBuffer() : (void)0U;
    osMutexRelease(MutexChangeParamsHandle);
  }
}

void Generate_IT_wave(void *argument) {
  for(;;) {
////    pwm[TIM1_CH12_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
////    pwm[TIM2_CH12_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
//    pwm[TIM2_CH34_IT]->isStarted() ? pwm[TIM2_CH34_IT]->nextValueFromBuffer() : (void)0U;
//    pwm[TIM3_CH12_IT]->isStarted() ? pwm[TIM3_CH12_IT]->nextValueFromBuffer() : (void)0U;
////    pwm[TIM3_CH34_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
////    pwm[TIM4_CH12_DMA]->isStarted() ? pwmGenerator->generateNextHalfbuffer() : (void)0U;
//////    pwm[TIM4_CH34_IT]->isStarted() ?  pwm[TIM4_CH34_IT]->nextValueFromBuffer() : (void)0U;
//    pwm[TIM8_CH12_IT]->isStarted() ?  pwm[TIM8_CH12_IT]->nextValueFromBuffer() : (void)0U;
//    pwm[TIM12_CH12_IT]->isStarted() ? pwm[TIM12_CH12_IT]->nextValueFromBuffer() : (void)0U;
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
      pwm[msg.emitter]->setPWM(msg.signal, msg.param, msg.value);
      osMutexRelease(MutexChangeParamsHandle);
    }
    else {
      Error_Handler();
    }
  }
}
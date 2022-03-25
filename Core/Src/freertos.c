/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "App/pwm_types.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for GenHalfwave */
osThreadId_t GenHalfwaveHandle;
const osThreadAttr_t GenHalfwave_attributes = {
  .name = "GenHalfwave",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ChngSignalParam */
osThreadId_t ChngSignalParamHandle;
const osThreadAttr_t ChngSignalParam_attributes = {
  .name = "ChngSignalParam",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for Gen_IT_wave */
osThreadId_t Gen_IT_waveHandle;
const osThreadAttr_t Gen_IT_wave_attributes = {
  .name = "Gen_IT_wave",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for SignalGeneratorQueue */
osMessageQueueId_t SignalGeneratorQueueHandle;
const osMessageQueueAttr_t SignalGeneratorQueue_attributes = {
  .name = "SignalGeneratorQueue"
};
/* Definitions for GenerateHalfWaveSemaphore */
osSemaphoreId_t GenerateHalfWaveSemaphoreHandle;
const osSemaphoreAttr_t GenerateHalfWaveSemaphore_attributes = {
  .name = "GenerateHalfWaveSemaphore"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void GenerateHalfwaveTask(void *argument);
void ChangeSignalParamsTask(void *argument);
void Generate_IT_wave(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* creation of GenerateHalfWaveSemaphore */
  GenerateHalfWaveSemaphoreHandle = osSemaphoreNew(30, 30, &GenerateHalfWaveSemaphore_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of SignalGeneratorQueue */
  SignalGeneratorQueueHandle = osMessageQueueNew (8, sizeof(tdPwmData), &SignalGeneratorQueue_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of GenHalfwave */
  GenHalfwaveHandle = osThreadNew(GenerateHalfwaveTask, NULL, &GenHalfwave_attributes);

  /* creation of ChngSignalParam */
  ChngSignalParamHandle = osThreadNew(ChangeSignalParamsTask, NULL, &ChngSignalParam_attributes);

  /* creation of Gen_IT_wave */
  Gen_IT_waveHandle = osThreadNew(Generate_IT_wave, NULL, &Gen_IT_wave_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
__weak void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
  }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_GenerateHalfwaveTask */
/**
* @brief Function implementing the GenHalfwave thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_GenerateHalfwaveTask */
__weak void GenerateHalfwaveTask(void *argument)
{
  /* USER CODE BEGIN GenerateHalfwaveTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END GenerateHalfwaveTask */
}

/* USER CODE BEGIN Header_ChangeSignalParamsTask */
/**
* @brief Function implementing the ChngSignalParam thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_ChangeSignalParamsTask */
__weak void ChangeSignalParamsTask(void *argument)
{
  /* USER CODE BEGIN ChangeSignalParamsTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END ChangeSignalParamsTask */
}

/* USER CODE BEGIN Header_Generate_IT_wave */
/**
* @brief Function implementing the Gen_IT_wave thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Generate_IT_wave */
__weak void Generate_IT_wave(void *argument)
{
  /* USER CODE BEGIN Generate_IT_wave */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END Generate_IT_wave */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */


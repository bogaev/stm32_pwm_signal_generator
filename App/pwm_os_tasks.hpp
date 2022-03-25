#ifndef _PWM_OS_TASKS_H_
#define _PWM_OS_TASKS_H_

#include "tim.h"

extern "C" void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim);
extern "C" void StartDefaultTask(void *argument);
extern "C" void GenerateHalfwaveTask(void *argument);
extern "C" void ChangeSignalParamsTask(void *argument);
extern "C" void Generate_IT_wave(void *argument);

#endif // #ifndef _PWM_OS_TASKS_H_
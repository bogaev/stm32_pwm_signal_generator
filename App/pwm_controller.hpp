#ifndef _PWM_CONTROLLER_H_
#define _PWM_CONTROLLER_H_

#include "tim.h"
#include "pwm_generator.hpp"

#define PWM_NUM 9

typedef enum enPWM_Timers_ {
  TIM1_CH12_DMA = 0,
  TIM2_CH12_DMA,
  TIM2_CH34_IT,
  TIM3_CH12_IT,
  TIM3_CH34_DMA,
  TIM4_CH12_DMA,
  TIM4_CH34_IT,
  TIM8_CH12_IT,
  TIM12_CH12_IT
} enPWM_Timers;

struct tdPwmChannels
{
  uint32_t positiveHalfwaveChannel;
  uint32_t negativeHalfwaveChannel;
};

class PwmController {
public:
    PwmController(TIM_HandleTypeDef* timer, tdPwmChannels channels, PwmGenerator& signal);
    ~PwmController();
    uint8_t isStarted();
    void start_IT();
    void start_DMA(const tdDataBuffers& dma);
    
private:
    TIM_HandleTypeDef* timer_ = nullptr;
    PwmGenerator& signal_;
    tdPwmChannels channels_;
    
    uint16_t TOTAL_POINTS_NUM;
    uint16_t index = 0;
    uint8_t is_started = 0;
};

extern "C" void StartDefaultTask(void *argument);
extern "C" void InitPwmControllers();
extern "C" void GenNextHalfwaveTask(void *argument);
extern "C" void HAL_TIM_PWM_PulseFinishedHalfCpltCallback(TIM_HandleTypeDef *htim);

#endif // #ifndef _SIN_H_

#ifndef _PWM_CONTROLLER_H_
#define _PWM_CONTROLLER_H_

#include "tim.h"
#include "pwm_generator.hpp"

#define PWM_NUM 9

struct tdPwmChannels
{
  uint32_t positiveHalfwaveChannel;
  uint32_t negativeHalfwaveChannel;
};

class PwmController
{
public:
    PwmController(TIM_HandleTypeDef* timer, tdPwmChannels channels, PwmGenerator& signal);
    ~PwmController();
    uint8_t isStarted();
    void start_IT();
    void start_DMA(const tdDMABuffers& dma);
    
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
extern "C" void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);

#endif // #ifndef _SIN_H_

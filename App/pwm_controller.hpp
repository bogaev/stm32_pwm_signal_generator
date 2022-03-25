#ifndef _PWM_CONTROLLER_H_
#define _PWM_CONTROLLER_H_

#include "pwm_generator.hpp"

typedef enum enPWM_Timers_ {
  TIM1_CH12_DMA = 0,
  TIM2_CH12_DMA,
  TIM2_CH34_IT,
  TIM3_CH12_IT,
  TIM3_CH34_DMA,
  TIM4_CH12_DMA,
  TIM4_CH34_IT,
  TIM8_CH12_IT,
  TIM12_CH12_IT,
  PWM_NUM
} enPWM_Timers;

struct tdPwmChannels {
  uint32_t positiveHalfwaveChannel;
  uint32_t negativeHalfwaveChannel;
};

class PwmController {
    uint16_t TOTAL_POINTS_NUM;
    uint32_t IT_buffer_index = 0;
    uint8_t is_started = 0;
    
    TIM_HandleTypeDef* timer_ = nullptr;
    PwmGenerator& generator_;
    tdPwmChannels channels_;
    const tdDataBuffers* b_;
    
public:
    PwmController(TIM_HandleTypeDef* timer, tdPwmChannels channels, PwmGenerator& generator);
    ~PwmController();
    uint8_t isStarted();
    void nextValueFromBuffer();
    void setPWM(uint8_t signal, uint8_t param, uint16_t value);
    void start_IT_buffer(const tdDataBuffers& b);
    void start_DMA(const tdDataBuffers& dma);
};

extern "C" void InitPwmControllers();

#endif // #ifndef _SIN_H_
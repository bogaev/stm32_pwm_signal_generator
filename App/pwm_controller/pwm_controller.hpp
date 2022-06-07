#ifndef _PWM_CONTROLLER_H_
#define _PWM_CONTROLLER_H_

#include "App/pwm_generator/pwm_generator.hpp"
#include "App/signal_generator/signals.h"
#include <memory>

typedef enum enPWM_Timers_ {
  TIM1_CH_1_2_DMA = 0,
  TIM2_CH_1_2_DMA,
  TIM2_CH_3_4_IT,
  TIM3_CH_1_2_IT,
  TIM3_CH_3_4_DMA,
  TIM4_CH_1_2_DMA,
  TIM4_CH_34_IT_RESERVE,
  TIM8_CH_1_2_IT,
  TIM12_CH_1_2_IT,
  PWM_NUM
} enPWM_Timers;

struct tdPwmChannels {
  uint32_t positiveHalfwaveChannel;
  uint32_t negativeHalfwaveChannel;
};

class PwmController {
public:
    PwmController(TIM_HandleTypeDef* timer,
                  tdPwmChannels channels,
                  PwmGenerator& generator);
    virtual ~PwmController();
    
    virtual void Start() = 0;
    virtual void Run() = 0;
    void Stop();

    void SetPWM(uint8_t signal, uint8_t param, uint16_t value);
    
protected:
    uint16_t TOTAL_POINTS_NUM;
    bool is_started = false;
    
    TIM_HandleTypeDef* timer_ = nullptr;
    tdPwmChannels channels_;
    PwmGenerator& generator_;
};

class DMA_PwmController : public PwmController {
public:
    DMA_PwmController(TIM_HandleTypeDef* timer,
                     tdPwmChannels channels,
                     PwmGenerator& generator,
                     const tdDataBuffers& buffers);
    void Start() override;
    void Run() override;

private:
    const tdDataBuffers& buffers_;
};

class IT_PwmController : public PwmController {    
public:
    IT_PwmController(TIM_HandleTypeDef* timer,
                     tdPwmChannels channels,
                     PwmGenerator& generator);
    void Start() override;
    void Run() override;
    void SetBuffer(const tdDataBuffers* buffers);
    void ResetBuffer();
    
private:
    void GetValueFromBuffer();
    void GetValue();
    
    const tdDataBuffers* buffers_ = nullptr;
    uint32_t index = 0;
};

#endif // #ifndef _SIN_H_
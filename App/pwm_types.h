#ifndef _PWM_TYPES_H_
#define _PWM_TYPES_H_

#include <stdint.h>

const int EMITTERS_NUM = 8;

typedef enum enPwmSignals_ {
  UART_SIGNAL_CARRIER = 0,
  UART_SIGNAL_AMP_MOD,
  UART_SIGNAL_FREQ_MOD,
  UART_SIGNAL_NUM
} enPwmSignals;

typedef enum tdSignalTypes_ {
  SIGNAL_TYPE_SINUS = 0,
  SIGNAL_TYPE_MEANDR,
  SIGNAL_TYPE_TRIANGLE,
  SIGNAL_TYPE_SAW,
  SIGNAL_TYPE_NONE,
  SIGNAL_TYPE_NUM
} tdSignalTypes;

typedef enum enSignalParams_ {
  UART_PARAM_SIGNAL_TYPE = 0,
  UART_PARAM_AMP,
  UART_PARAM_FREQ,
  UART_PARAM_NUM
} enSignalParams;

typedef struct tdPwmData_ {
  uint16_t value;
  uint16_t reserved1;
  uint8_t emitter;
  uint8_t signal;
  uint8_t param;
  uint8_t reserved2;
} tdPwmData;

typedef struct tdUartMessage_ {
  tdPwmData data;
  uint32_t crc;
} tdUartMessage;

#endif // #ifndef _PWM_TYPES_H_
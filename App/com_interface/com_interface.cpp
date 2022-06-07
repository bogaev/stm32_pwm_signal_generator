#include "App/com_interface/com_interface.hpp"
#include "boost/crc.hpp"
#include <iostream>

using namespace std;

HAL_UART_StateTypeDef debug_uart;
Uart uart;

void UartStart() {
  uart.init();
}

/**
  * @brief  Rx Transfer completed callbacks.
  * @param  huart  Pointer to a UART_HandleTypeDef structure that contains
  *                the configuration information for the specified UART module.
  * @retval None
  */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
  if(uart.ReceiveMessage() == HAL_OK) {
    uart.ReadMessage();
  }
  else {
    Error_Handler();
  }
}

void Uart::init() {
  HAL_UART_Receive_IT(&huart5, (uint8_t*)&message, sizeof(tdUartMessage));
}
  
HAL_StatusTypeDef Uart::ReceiveMessage() {
  return HAL_UART_Receive_IT(&huart5, (uint8_t*)&message, sizeof(tdUartMessage));
}
  
void Uart::ReadMessage() {
  boost::crc_32_type crc32;
  crc32.process_bytes( &message.data, sizeof(message.data) );
  uint32_t crc = crc32.checksum();
  if(crc == message.crc) {
    osStatus_t status = osMessageQueuePut(SignalGeneratorQueueHandle, &message.data, 0U, 0U);
    if(status == osOK) {
//      cout << status << endl;
//      cout << data.emitter << endl;
//      cout << data.signal << endl;
//      cout << data.param << endl;
//      cout << data.value << endl;
//      cout << endl;
    } else {
//      cout << status << endl;
    }
  }
  else {
    Error_Handler();
  }
}
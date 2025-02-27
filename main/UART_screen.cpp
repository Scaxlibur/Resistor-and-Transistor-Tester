#include "UART_screen.hpp"

UARTscreen_class::UARTscreen_class()
{
    init_uart();
}

UARTscreen_class::~UARTscreen_class()
{
}

void UARTscreen_class::init_uart(void){
    if(uart_is_driver_installed(UART_NUM_SCREEN) == false){
        ESP_ERROR_CHECK(uart_param_config(UART_NUM_SCREEN, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(UART_NUM_SCREEN, UART_NUM_SCREEN_TX, UART_NUM_SCREEN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_SCREEN, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    };
}

int UARTscreen_class::command_loop(void)
{
  int length = 0;
  uart_get_buffered_data_len(UART_NUM_SCREEN, (size_t*)&length);
  if (length == 0) return 0;// 如果串口是空的直接返回
  char received_chars[10];
  vTaskDelay(1 / portTICK_PERIOD_MS);
  // 从串口读取返回的数据，读取20个字符
  uart_read_bytes(UART_NUM_SCREEN, received_chars, length, 100 / portTICK_PERIOD_MS);

  // 根据指令做不同动作
  if (received_chars[0] == 'R') // A指令，设置为电阻测量模式
  {

  }else if(received_chars[0] == 'T') // T指令，设置为三极管测量模式
  //  最后清空串口
  while (length >= 0)
    uart_flush(UART_NUM_SCREEN);
  return 0;
}
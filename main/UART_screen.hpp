#ifndef __UART_SCREEN_HPP__
#define __UART_SCREEN_HPP__

#include "driver/uart.h" 
#include <string.h>
#include "soc\gpio_num.h"
#include "FreeRTOS/portmacro.h"

#define UART_NUM_SCREEN UART_NUM_1
#define UART_NUM_SCREEN_TX UART_PIN_NO_CHANGE
#define UART_NUM_SCREEN_RX UART_PIN_NO_CHANGE

class UARTscreen_class
{
private:
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT
    };
    static const int RX_BUF_SIZE = 1024;  //串口接收缓冲区大小
    void init_uart(void);
    int command_loop(void);

public:
    UARTscreen_class();
    ~UARTscreen_class();
    
};

#endif
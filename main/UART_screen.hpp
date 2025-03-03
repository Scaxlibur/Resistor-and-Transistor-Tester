#ifndef __UART_SCREEN_HPP__
#define __UART_SCREEN_HPP__

#include "driver/uart.h" 
#include <string.h>
#include "soc\gpio_num.h"
#include "FreeRTOS/portmacro.h"
#include "modeSwitch.hpp"
#include <string.h>
#include <stdlib.h>

#define UART_NUM_SCREEN UART_NUM_1
#define UART_NUM_SCREEN_TX GPIO_NUM_17
#define UART_NUM_SCREEN_RX GPIO_NUM_18

typedef struct{
    measuringMode_t measuringMode;  //测量模式
    uint16_t measuringValue_int;    //测量结果的整数部分
    uint16_t measuringValue_float;  //测量结果的小数部分
    float measuringValue;           //测量真值
}data4Tasks;

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
    void UARTWriteBytes(uint8_t data);
    void UARTWriteBytes(uint16_t data);
    void UARTWriteBytes(uint32_t data);
    void UARTWriteBytes(const char* data);
    void UARTcommandEnd(void);

public:
    UARTscreen_class();
    ~UARTscreen_class();
    void command_send(data4Tasks com);
    void command_receive(data4Tasks com);
};

#endif
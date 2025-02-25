#include <stdio.h>
#include "UART_screen.hpp"


typedef struct{
    bool isRunning = false;
    int msg = 0;
}data4Tasks;

QueueHandle_t screen_resistor_com_handle;  //串口屏和电阻测量模块的通信队列
QueueHandle_t screen_transistor_com_handle;  //串口屏和三极管测量模块的通信队列

TaskHandle_t screen_taskhHandle;
TaskHandle_t resistor_taskhHandle;
TaskHandle_t transistor_taskhHandle;

void UARTscreen_task()
{
    UART_screen_class UARTscreen;
}

extern "C" void app_main(void)
{
    while (true)
    {
        vTaskDelay(45 / portTICK_PERIOD_MS);
    }
}
#include <stdio.h>
#include "i2c_screen.hpp"
#include "resistor.hpp"
#include "transistor.hpp"
#include "UART_screen.hpp"
#include "modeSwitch.hpp"

typedef struct{
    measuringMode_t measuringMode;  //测量模式
    uint16_t measuringValue_int;    //测量结果的整数部分
    uint16_t measuringValue_float;  //测量结果的小数部分
    double measuringValue;
}data4Tasks;

//QueueHandle_t UARTscreen_modeSwitch_measuring_com_handle;
QueueHandle_t I2Cscreen_modeSwitch_measuring_com_handle;       //OLED屏、电阻测量、三极管测量的三方通信队列

TaskHandle_t UARTscreen_taskHandle;
TaskHandle_t I2Cscreen_taskHandle;
TaskHandle_t resistor_taskHandle;
TaskHandle_t transistor_taskHandle;
TaskHandle_t switch_taskhandle;

/*
void UARTscreen_task(void *arg)
{
    UARTscreen_class UARTscreen;
}
*/

void switch_task(void *arg)
{
    modeSwitch_class modeSwitch;
    data4Tasks I2Cscreen_modeSwitch_measuring_com;
    BaseType_t I2Cscreen_modeSwitch_measuring_com_status;
    measuringMode_t last_measuringMode = ohm2K;//存储之前挡位，避免反复发送
    measuringMode_t now_measuringMode = modeSwitch.whichMode();
    while(true)
    {
        now_measuringMode = modeSwitch.whichMode();
        if(last_measuringMode != now_measuringMode){
            last_measuringMode = now_measuringMode;
            switch (last_measuringMode)
            {
            case ohm200:
                I2Cscreen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case ohm2K:
                I2Cscreen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case ohm20K:
                I2Cscreen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case ohm200K:
                I2Cscreen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            case transistor:
                I2Cscreen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case NPN:
                I2Cscreen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case PNP:
                I2Cscreen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            }
            I2Cscreen_modeSwitch_measuring_com_status = xQueueSendToFront(I2Cscreen_modeSwitch_measuring_com_handle, &I2Cscreen_modeSwitch_measuring_com.measuringMode, 50/portTICK_PERIOD_MS);//把状态发送给屏幕
        }
    }
}

void resistor_task(void *arg)
{
    resistorMeasuring_class resistor;
    BaseType_t I2Cscreen_modeSwitch_measuring_com_status;
    data4Tasks I2Cscreen_modeSwitch_measuring_com;
    while (true)
    {
        I2Cscreen_modeSwitch_measuring_com_status = xQueueReceive(I2Cscreen_modeSwitch_measuring_com_handle, &I2Cscreen_modeSwitch_measuring_com.measuringMode, 500/portTICK_PERIOD_MS);  //从队列I2Cscreen_modeSwitch_measuring_com中取一条数据
        if(I2Cscreen_modeSwitch_measuring_com_status == pdPASS )
        {
            resistor.R_compute(I2Cscreen_modeSwitch_measuring_com.measuringMode);
        }
    }
    
}

void transistor_task(void *arg)
{
    transistorMeasuring_class transistorMeasuring;
    BaseType_t I2Cscreen_modeSwitch_measuring_com_status;
    data4Tasks I2Cscreen_modeSwitch_measuring_com;
    while (true)
    {
        I2Cscreen_modeSwitch_measuring_com_status = xQueueReceive(I2Cscreen_modeSwitch_measuring_com_handle, &I2Cscreen_modeSwitch_measuring_com.measuringMode, 50/portTICK_PERIOD_MS);
        if(I2Cscreen_modeSwitch_measuring_com_status == pdTRUE)
        {
            if((I2Cscreen_modeSwitch_measuring_com.measuringMode == transistor)||
               (I2Cscreen_modeSwitch_measuring_com.measuringMode == NPN)||
               (I2Cscreen_modeSwitch_measuring_com.measuringMode == PNP))
               {
                    I2Cscreen_modeSwitch_measuring_com.measuringValue_int = transistorMeasuring.beta_compute(NPN);//这里到底怎么写还需要思考
               }
        }
        I2Cscreen_modeSwitch_measuring_com_status = xQueueSendToFront(I2Cscreen_modeSwitch_measuring_com_handle, &I2Cscreen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);//把状态发送给屏幕
    }
}

void I2Cscreen_task(void *arg)
{
    I2Cscreen_class I2Cscreen;
    data4Tasks I2Cscreen_modeSwitch_measuring_com;
    BaseType_t I2Cscreen_modeSwitch_measuring_com_status;
    while(true)
    {
        I2Cscreen_modeSwitch_measuring_com_status = xQueueReceive(I2Cscreen_modeSwitch_measuring_com_handle, &I2Cscreen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    printf("power n\n");
    I2Cscreen_modeSwitch_measuring_com_handle = xQueueCreate(10, sizeof(data4Tasks));

    xTaskCreate(I2Cscreen_task, "I2Cscreen_task", 12*1024, NULL, 5, &I2Cscreen_taskHandle);
    xTaskCreate(resistor_task, "resistor_task", 12*1024, NULL, 5, &resistor_taskHandle);
    xTaskCreate(transistor_task, "transistor_task", 12*1024, NULL, 5, &transistor_taskHandle);
    xTaskCreate(switch_task, "switch_task", 12*1024, NULL, 5, &switch_taskhandle);
    //xTaskCreate(UARTscreen_task, "UARTscreen_task", 12*1024, NULL, 5, &UARTscreen_task_taskHandle);
}
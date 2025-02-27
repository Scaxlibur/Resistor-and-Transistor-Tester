#include <stdio.h>
#include "i2c_screen.hpp"
#include "resistor.hpp"
#include "transistor.hpp"
#include "UART_screen.hpp"
#include "modeSwitch.hpp"

typedef enum{
    resistor,
    transistor
}whichRunning_t;

typedef struct{
    whichRunning_t whichRunning;
    measuringMode_t measuringMode;
    double measuringValue;
    int msg = 0;
}data4Tasks;

QueueHandle_t UARTscreen_resistor_com_handle;       //串口屏和电阻测量模块的通信队列
QueueHandle_t UARTscreen_transistor_com_handle;     //串口屏和三极管测量模块的通信队列
QueueHandle_t I2Cscreen_resistor_com_handle;        //OLED屏和电阻测量模块的通信队列
QueueHandle_t I2Cscreen_transistor_com_handle;      //OLED屏和三极管测量模块的通信队列
QueueHandle_t modeSwitch_resistor_com_handle;       //挡位切换和电阻测量模块的通信队列
QueueHandle_t modeSwitch_I2Cscreen_com_handle;      //挡位切换和OLED屏的通信队列
QueueHandle_t modeSwitch_transistor_com_handle;     //挡位切换和三极管测量的通信队列

TaskHandle_t UARTscreen_taskHandle;
TaskHandle_t I2Cscreen_taskHandle;
TaskHandle_t resistor_taskHandle;
TaskHandle_t transistor_taskHandle;
TaskHandle_t switch_taskhandle;

void UARTscreen_task(void *arg)
{
    UARTscreen_class UARTscreen;
}

void switch_task(void *arg)
{
    modeSwitch_class modeSwitch;
    data4Tasks modeSwitch_I2Cscreen_com;
    data4Tasks modeSwitch_resistor_com;
    data4Tasks modeSwitch_transistor_com;
    BaseType_t modeSwitch_I2Cscreen_com_status;
    BaseType_t modeSwitch_resistor_com_status;
    BaseType_t modeSwitch_transistor_com_status;
    measuringMode_t last_measuringMode = ohm2K;//存储之前挡位，避免反复发送
    while(true)
    {
        if(last_measuringMode != modeSwitch.whichMode()){
            last_measuringMode = modeSwitch.whichMode();
            switch (last_measuringMode)
            {
            case ohm200:
                modeSwitch_resistor_com.measuringMode = last_measuringMode;
                modeSwitch_I2Cscreen_com.measuringMode = last_measuringMode;
            break;
            case ohm2K:
                modeSwitch_resistor_com.measuringMode = last_measuringMode;
                modeSwitch_I2Cscreen_com.measuringMode = last_measuringMode;
            break;
            case ohm20K:
                modeSwitch_resistor_com.measuringMode = last_measuringMode;
                modeSwitch_I2Cscreen_com.measuringMode = last_measuringMode;
            break;
            case ohm200K:
                modeSwitch_resistor_com.measuringMode = last_measuringMode;
                modeSwitch_I2Cscreen_com.measuringMode = last_measuringMode;
            break;
            case tansistor:
                modeSwitch_transistor_com.measuringMode = last_measuringMode;
                modeSwitch_I2Cscreen_com.measuringMode = last_measuringMode;
            break;
            case NPN:
                modeSwitch_transistor_com.measuringMode = last_measuringMode;
                modeSwitch_I2Cscreen_com.measuringMode = last_measuringMode;
            break;
            case PNP:
                modeSwitch_transistor_com.measuringMode = last_measuringMode;
                modeSwitch_I2Cscreen_com.measuringMode = last_measuringMode;
            break;
            }
            modeSwitch_resistor_com_status = xQueueSendToFront(modeSwitch_resistor_com_handle, &modeSwitch_resistor_com.measuringMode, 50/portTICK_PERIOD_MS);//把状态发送给电阻测量
            modeSwitch_I2Cscreen_com_status = xQueueSendToFront(modeSwitch_I2Cscreen_com_handle, &modeSwitch_I2Cscreen_com.measuringMode, 50/portTICK_PERIOD_MS);//把状态发送给屏幕
            modeSwitch_transistor_com_status = xQueueSendToFront(modeSwitch_transistor_com_handle, &modeSwitch_transistor_com.measuringMode, 50/portTICK_PERIOD_MS);//把状态发送给三极管测量
        }
    }
}

void resistor_task(void *arg)
{
    resistorMeasuring_class resistor;
    BaseType_t I2Cscreen_resistor_com_status;
    data4Tasks I2Cscreen_resistor_com;
    while (true)
    {
        I2Cscreen_resistor_com_status = xQueueReceive(I2Cscreen_resistor_com_handle, &I2Cscreen_resistor_com.measuringMode, 500/portTICK_PERIOD_MS);  //从队列I2Cscreen_resistor_com中取一条数据
        if(I2Cscreen_resistor_com_status == pdPASS )
        {
            resistor.R_compute(I2Cscreen_resistor_com.measuringMode);
        }
    }
    
}

void transistor_task(void *arg)
{
    transistorMeasuring_class transistorMeasuring;
    BaseType_t modeSwitch_transistor_com_status;
    BaseType_t I2Cscreen_transistor_com_status;
    data4Tasks modeSwitch_transistor_com;
    data4Tasks I2Cscreen_transistor_com;
    while (true)
    {
        modeSwitch_transistor_com_status = xQueueReceive(modeSwitch_transistor_com_handle, &modeSwitch_transistor_com.measuringMode, 50/portTICK_PERIOD_MS);
        if(modeSwitch_transistor_com_status == pdTRUE)
        {
            if((modeSwitch_transistor_com.measuringMode == transistor)||
               (modeSwitch_transistor_com.measuringMode == NPN)||
               (modeSwitch_transistor_com.measuringMode == PNP))
               {
                    I2Cscreen_transistor_com.measuringValue = transistorMeasuring.beta_compute(NPN);//这里到底怎么写还需要思考
               }
        }
        I2Cscreen_transistor_com_status = xQueueSendToFront(I2Cscreen_transistor_com_handle, &I2Cscreen_transistor_com, 50/portTICK_PERIOD_MS);//把状态发送给屏幕
    }
}

void I2Cscreen_task(void *arg)
{
    I2Cscreen_class I2Cscreen;
    data4Tasks I2Cscreen_resistor_com;
    data4Tasks I2Cscreen_transistor_com;
    BaseType_t I2Cscreen_resistor_com_status;
    BaseType_t I2Cscreen_transistor_com_status;
    while(true)
    {
        I2Cscreen_resistor_com_status = xQueueReceive(I2Cscreen_resistor_com_handle, &I2Cscreen_resistor_com, 50/portTICK_PERIOD_MS);
        I2Cscreen_transistor_com_status = xQueueReceive(I2Cscreen_transistor_com_handle, &I2Cscreen_transistor_com, 50/portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    printf("power n\n");
    I2Cscreen_resistor_com_handle = xQueueCreate(10, sizeof(data4Tasks));
    I2Cscreen_transistor_com_handle = xQueueCreate(10, sizeof(data4Tasks));

    xTaskCreate(I2Cscreen_task, "I2Cscreen_task", 12*1024, NULL, 5, &I2Cscreen_taskHandle);
    xTaskCreate(resistor_task, "resistor_task", 12*1024, NULL, 5, &resistor_taskHandle);
    xTaskCreate(transistor_task, "transistor_task", 12*1024, NULL, 5, &transistor_taskHandle);
    xTaskCreate(switch_task, "switch_task", 12*1024, NULL, 5, &switch_taskhandle);
    //xTaskCreate(UARTscreen_task, "UARTscreen_task", 12*1024, NULL, 5, &UARTscreen_task_taskHandle);
}
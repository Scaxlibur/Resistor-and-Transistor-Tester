#include <stdio.h>
#include "i2c_screen.hpp"
#include "resistor.hpp"
#include "transistor.hpp"
#include "UART_screen.hpp"
#include "modeSwitch.hpp"

QueueHandle_t screen_modeSwitch_measuring_com_handle;       //屏幕、电阻测量、三极管测量的三方通信队列
TaskHandle_t UARTscreen_taskHandle;
TaskHandle_t I2Cscreen_taskHandle;
TaskHandle_t resistor_taskHandle;
TaskHandle_t transistor_taskHandle;
TaskHandle_t switch_taskhandle;

void UARTscreen_task(void *arg)
{
    UARTscreen_class UARTscreen;
    BaseType_t screen_modeSwitch_measuring_com_status;
    data4Tasks screen_modeSwitch_measuring_com;
    while(true)
    {
        for(int i=0;i<7;i++){
            screen_modeSwitch_measuring_com.measuringMode = (measuringMode_t)i;
            UARTscreen.command_send(screen_modeSwitch_measuring_com);
            vTaskDelay(1000 / portTICK_PERIOD_MS);

        }
        /*
        screen_modeSwitch_measuring_com_status = xQueueReceive(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 500/portTICK_PERIOD_MS);  //从队列screen_modeSwitch_measuring_com中取一条数据
        if(screen_modeSwitch_measuring_com_status == pdPASS )
        {

        }*/
        vTaskDelay(100 / portTICK_PERIOD_MS);
        
    }
}

void switch_task(void *arg)
{
    modeSwitch_class modeSwitch;
    data4Tasks screen_modeSwitch_measuring_com;
    BaseType_t screen_modeSwitch_measuring_com_status;
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
                screen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case ohm2K:
                screen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case ohm20K:
                screen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case ohm200K:
                screen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            case transistor:
                screen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case NPN:
                screen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            case PNP:
                screen_modeSwitch_measuring_com.measuringMode = last_measuringMode;
            break;
            }
            screen_modeSwitch_measuring_com_status = xQueueSendToFront(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com.measuringMode, 50/portTICK_PERIOD_MS);//把状态发送给屏幕
        }
    }
}

void resistor_task(void *arg)
{
    resistorMeasuring_class resistor;
    BaseType_t screen_modeSwitch_measuring_com_status;
    data4Tasks screen_modeSwitch_measuring_com;
    while (true)
    {
        screen_modeSwitch_measuring_com_status = xQueueReceive(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com.measuringMode, 500/portTICK_PERIOD_MS);  //从队列screen_modeSwitch_measuring_com中取一条数据
        if(screen_modeSwitch_measuring_com_status == pdPASS )
        {
            resistor.R_compute(screen_modeSwitch_measuring_com.measuringMode);
        }
    }
    
}

void transistor_task(void *arg)
{
    transistorMeasuring_class transistorMeasuring;
    BaseType_t screen_modeSwitch_measuring_com_status;
    data4Tasks screen_modeSwitch_measuring_com;
    while (true)
    {
        screen_modeSwitch_measuring_com_status = xQueueReceive(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com.measuringMode, 50/portTICK_PERIOD_MS);
        if(screen_modeSwitch_measuring_com_status == pdTRUE)
        {
            if((screen_modeSwitch_measuring_com.measuringMode == transistor)||
               (screen_modeSwitch_measuring_com.measuringMode == NPN)||
               (screen_modeSwitch_measuring_com.measuringMode == PNP))
               {
                    screen_modeSwitch_measuring_com.measuringValue_int = transistorMeasuring.beta_compute(NPN);//这里到底怎么写还需要思考
               }
        }
        screen_modeSwitch_measuring_com_status = xQueueSendToFront(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);//把状态发送给屏幕
    }
}

void I2Cscreen_task(void *arg)
{
    I2Cscreen_class I2Cscreen;
    data4Tasks screen_modeSwitch_measuring_com;
    BaseType_t screen_modeSwitch_measuring_com_status;
    while(true)
    {
        screen_modeSwitch_measuring_com_status = xQueueReceive(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);
    }
}

extern "C" void app_main(void)
{
    printf("power on\n");
    screen_modeSwitch_measuring_com_handle = xQueueCreate(10, sizeof(data4Tasks));
/*
    xTaskCreate(I2Cscreen_task, "I2Cscreen_task", 12*1024, NULL, 5, &I2Cscreen_taskHandle);
    xTaskCreate(resistor_task, "resistor_task", 12*1024, NULL, 5, &resistor_taskHandle);
    xTaskCreate(transistor_task, "transistor_task", 12*1024, NULL, 5, &transistor_taskHandle);
    xTaskCreate(switch_task, "switch_task", 12*1024, NULL, 5, &switch_taskhandle);
    */
    xTaskCreate(UARTscreen_task, "UARTscreen_task", 12*1024, NULL, 5, &UARTscreen_taskHandle);
}
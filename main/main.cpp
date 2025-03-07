#include <stdio.h>
#include "resistor.hpp"
#include "transistor.hpp"
#include "UART_screen.hpp"
#include "modeSwitch.hpp"
#include "ADS1115.hpp"

QueueHandle_t screen_modeSwitch_measuring_com_handle;       //屏幕、电阻测量、三极管测量的三方通信队列
TaskHandle_t UARTscreen_taskHandle;
TaskHandle_t resistor_taskHandle;
TaskHandle_t transistor_taskHandle;
TaskHandle_t switch_taskhandle;
TaskHandle_t ads1115_taskhandle;

/* i2c setup ----------------------------------o------- */
// Config profile for espressif I2C lib
i2c_config_t i2c_cfg = {                     
    .mode = I2C_MODE_MASTER, 
    .sda_io_num = SDA_IO,
    .scl_io_num = SCL_IO,
    .sda_pullup_en = GPIO_PULLUP_DISABLE,
    .scl_pullup_en = GPIO_PULLUP_DISABLE,
    .master = {
       .clk_speed = FREQ_HZ,
    },
  };
  
  /* ADS1115 setup ------------------------------------- */
  // Below uses the default values speficied by the datasheet
  ads1115_t ads1115_cfg = {
    .reg_cfg =  ADS1115_CFG_LS_COMP_MODE_TRAD | // Comparator is traditional
                ADS1115_CFG_LS_COMP_LAT_NON |   // Comparator is non-latching
                ADS1115_CFG_LS_COMP_POL_LOW |   // Alert is active low
                ADS1115_CFG_LS_COMP_QUE_DIS |   // Compator is disabled
                ADS1115_CFG_LS_DR_1600SPS |     // No. of samples to take
                ADS1115_CFG_MS_MODE_SS |         // Mode is set to single-shot
                ADS1115_CFG_MS_PGA_FSR_6_144V,
    .dev_addr = 0x49,
  };

void UARTscreen_task(void *arg)
{
    UARTscreen_class UARTscreen;
    BaseType_t screen_modeSwitch_measuring_com_status;
    data4Tasks screen_modeSwitch_measuring_com;
    while(true)
    {
        screen_modeSwitch_measuring_com.measuringMode = UARTscreen.command_receive();
        xQueueSendToFront(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);
        xQueueReceive(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 500/portTICK_PERIOD_MS);
        UARTscreen.command_send(screen_modeSwitch_measuring_com);
        
        
        printf("%d",(int)screen_modeSwitch_measuring_com.measuringMode);
        
        
        
        
        /*for(int i=0;i<7;i++){
            screen_modeSwitch_measuring_com.measuringMode = (measuringMode_t)i;
            screen_modeSwitch_measuring_com.measuringValue = i;
            UARTscreen.command_send(screen_modeSwitch_measuring_com);
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }*/
        //screen_modeSwitch_measuring_com_status = xQueueReceive(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 500/portTICK_PERIOD_MS);  //从队列screen_modeSwitch_measuring_com中取一条数据
        //if(screen_modeSwitch_measuring_com_status == pdPASS )
        {

        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
        
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
        if(screen_modeSwitch_measuring_com_status == pdTRUE) //先看通信是否正常
        {
            if((screen_modeSwitch_measuring_com.measuringMode == transistor)||
               (screen_modeSwitch_measuring_com.measuringMode == NPN))
               {
                    screen_modeSwitch_measuring_com.measuringValue = transistorMeasuring.beta_compute(NPN);
               }else if(screen_modeSwitch_measuring_com.measuringMode == PNP)
               {
                    screen_modeSwitch_measuring_com.measuringValue = transistorMeasuring.beta_compute(PNP);
               }
        }
        screen_modeSwitch_measuring_com_status = xQueueSendToFront(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);//把状态发送给屏幕
    }
}

void ads1115(void *arg)
{
    // Buffer for result
    uint16_t result = 0;
    BaseType_t screen_modeSwitch_measuring_com_status;
    data4Tasks screen_modeSwitch_measuring_com;
    for(;;) 
    {
        xQueueReceive(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);
        //if (screen_modeSwitch_measuring_com.measuringMode == ohm200)
        {
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
                vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            float votage = result * 0.0001875;
            float resistor = votage * 200;
            printf("%f", resistor);
            screen_modeSwitch_measuring_com.measuringValue = resistor;
        }/*
        else if(screen_modeSwitch_measuring_com.measuringMode == ohm2K){
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
            vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            float votage = result * 0.0001875;
            float resistor = votage * 200 *220 /(220-200*votage);
            screen_modeSwitch_measuring_com.measuringValue = resistor;
        }else if(screen_modeSwitch_measuring_com.measuringMode == ohm20K){
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
            vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            float votage = result * 0.0001875;
            float resistor = votage * 200 *220 /(220-200*votage);
            screen_modeSwitch_measuring_com.measuringValue = resistor;
        }else if(screen_modeSwitch_measuring_com.measuringMode == ohm200K){ 
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
            vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            float votage = result * 0.0001875;
            float resistor = votage * 200 *200 /(200-200*votage);
            screen_modeSwitch_measuring_com.measuringValue = resistor;
        }else if(screen_modeSwitch_measuring_com.measuringMode == PNP){
            ADS1115_request_single_ended_AIN0();
            while(!ADS1115_get_conversion_state()) 
            vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            float votage = result * 0.0001875;
            float beta = votage *25 * 100;//公式：beta = Vadc * 25 * 100
            screen_modeSwitch_measuring_com.measuringValue = beta;
        }else if(screen_modeSwitch_measuring_com.measuringMode == NPN){
            ADS1115_request_single_ended_AIN2();
            while(!ADS1115_get_conversion_state()) 
            vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            float votage = result * 0.0001875;
            float beta = (5 - votage) * 2500;//公式：beta = (5 - 4* Vadc) * 25 * 100
            screen_modeSwitch_measuring_com.measuringValue = beta;
        }       
    
        xQueueSendToFront(screen_modeSwitch_measuring_com_handle, &screen_modeSwitch_measuring_com, 50/portTICK_PERIOD_MS);
        */
        /*
        // Request single ended on pin AIN0  
        ADS1115_request_single_ended_AIN1();      // all functions except for get_conversion_X return 'esp_err_t' for logging

        // Check conversion state - returns true if conversion is complete 
        while(!ADS1115_get_conversion_state()) 
            vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
        
        // Return latest conversion value
        result = ADS1115_get_conversion() ;
        float votage = result * 0.0001875;
        printf("Conversion Value: %f V\n", votage);
        //float beta = (5 - 4 * votage)* 25*100; //公式：beta = (5 - 4* Vadc) * 25 * 100    
        //printf("beta: %f \n", beta);
        */  
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }

    printf("Should not reach here!");
    vTaskDelete(NULL);  
}

extern "C" void app_main(void)
{
    printf("power on\n");
    printf("Starting ADS1115 example..\n");
    screen_modeSwitch_measuring_com_handle = xQueueCreate(10, sizeof(data4Tasks));
        // Setup I2C
        i2c_param_config(I2C_NUM, &i2c_cfg);
        i2c_driver_install(I2C_NUM, I2C_MODE, I2C_RX_BUF_STATE, I2C_TX_BUF_STATE, I2C_INTR_ALOC_FLAG);
      
        // Setup ADS1115
        ADS1115_initiate(&ads1115_cfg);
/*
    xTaskCreate(resistor_task, "resistor_task", 12*1024, NULL, 5, &resistor_taskHandle);
    xTaskCreate(transistor_task, "transistor_task", 12*1024, NULL, 5, &transistor_taskHandle);
    */
    xTaskCreate(UARTscreen_task, "UARTscreen_task", 12*1024, NULL, 5, &UARTscreen_taskHandle);
    xTaskCreate(ads1115, "ads1115" ,12*1024 , NULL , 3, &ads1115_taskhandle);
}
#include <stdio.h>
#include "UART_screen.hpp"
#include "modeSwitch.hpp"
#include "ADS1115.hpp"

QueueHandle_t screen_measuring_com_handle;       //屏幕到测量仪的三方通信队列
QueueHandle_t measuring_screen_com_handle;       //测量仪到屏幕的三方通信队列

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
                ADS1115_CFG_MS_PGA_FSR_4_096V,
    .dev_addr = 0x49,
  };


extern "C" void app_main(void)
{
    printf("power on\n");
    printf("Starting ADS1115 example..\n");

    // Setup I2C
    i2c_param_config(I2C_NUM, &i2c_cfg);
    i2c_driver_install(I2C_NUM, I2C_MODE, I2C_RX_BUF_STATE, I2C_TX_BUF_STATE, I2C_INTR_ALOC_FLAG);
    
    // Setup ADS1115
    ADS1115_initiate(&ads1115_cfg);

    UARTscreen_class UARTscreen;
    data4Tasks com;
    int16_t result;
    while (true)
    {
        com.measuringMode = UARTscreen.command_receive(com.measuringMode);//从屏幕中读取挡位
        double votage;
        double resistor;
        double beta;
        switch (com.measuringMode)
        {
        case error:
            com.measuringMode = UARTscreen.command_receive(com.measuringMode);//从屏幕中读取挡位
            break;
        case ohm200://4v以上结果为error
            ADS1115_initiate(&ads1115_cfg);
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
                vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
             votage = result * 0.000125;
             resistor = votage * 200;
            printf("%f\n", resistor);
            printf("%fV\n", votage);
            com.measuringValue = resistor;
            
            com.measuringMode = ohm200;
            if (votage > 4)  com.measuringMode = error;
            break;
        case ohm2K:
            ADS1115_initiate(&ads1115_cfg);
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
                vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
             votage = result * 0.000125 - 0.0065;//125;
             resistor = votage * 200 *220 /(220-200*votage);
            printf("%f\n", resistor);
            printf("%fV\n", votage);
            com.measuringValue = resistor;
            com.measuringMode = ohm2K;
            if (resistor <0 || resistor > 2000)  com.measuringMode = error;
            break;        
        case ohm20K:
            ADS1115_initiate(&ads1115_cfg);
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
                vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
             votage = result * 0.000125 + 0.000625;
             resistor = votage * 200 *220 /(220-200*votage);
            printf("%f\n", resistor);
            printf("%fV\n", votage);
            com.measuringValue = resistor;
            com.measuringMode = ohm20K;
            break;        
        case ohm200K:
            ADS1115_initiate(&ads1115_cfg);
            ADS1115_request_single_ended_AIN1();
            while(!ADS1115_get_conversion_state()) 
                vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
             votage = result * 0.000125;
             resistor = votage * 200 *200 /(200-200*votage);
            printf("%f\n", resistor);
            printf("%fV\n", votage);
            com.measuringValue = resistor;
            com.measuringMode = ohm200K;
            break;
        case PNP:
            ADS1115_initiate(&ads1115_cfg);
            ADS1115_request_single_ended_AIN0();
            while(!ADS1115_get_conversion_state()) 
                vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            votage = result * 0.000125;
            beta = votage *25 * 100;//公式：beta = Vadc * 25 * 100
            printf("%f\n", beta);
            printf("%f\n", votage);
            com.measuringValue = beta;
            com.measuringMode = PNP;
            break;
        case NPN:
            ADS1115_request_single_ended_AIN2();
            while(!ADS1115_get_conversion_state()) 
                vTaskDelay(5 / portTICK_PERIOD_MS);          // wait 5ms before check again
            result = ADS1115_get_conversion() ;
            votage = result * 0.000125;
            beta = (5 - 4* votage) * 2500 -70;//公式：beta = (5 - 4* Vadc) * 25 * 100
            printf("%f\n", beta);
            printf("%f\n", votage);
            com.measuringValue = beta;
            com.measuringMode = NPN;
            break;
        case transistor:
            break;
        default:
            break;
        }

        UARTscreen.command_send(com);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}
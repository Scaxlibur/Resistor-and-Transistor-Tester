#ifndef __MODESWITCH_HPP__
#define __MODESWITCH_HPP__

#include "driver/gpio.h"

#define IS_OHM200_PIN           GPIO_NUM_10
#define IS_OHM2K_PIN            GPIO_NUM_11
#define IS_OHM20K_PIN           GPIO_NUM_12
#define IS_OHM200K_PIN          GPIO_NUM_13
#define OR_TRI_R_MEASURING_PIN  GPIO_NUM_14 //选择测量三极管还是测量电阻

typedef enum{
    ohm200,
    ohm2K,
    ohm20K,
    ohm200K,
    transistor,
    NPN,
    PNP,
    error,
}measuringMode_t; //测量挡位

class modeSwitch_class
{
public:
    modeSwitch_class();
    ~modeSwitch_class();
    measuringMode_t whichMode();

private:
    gpio_config_t gpioConfig ={
        .pin_bit_mask = (1ULL << IS_OHM200_PIN)||
                        (1ULL << IS_OHM2K_PIN)||
                        (1ULL << IS_OHM20K_PIN)||
                        (1ULL << IS_OHM200K_PIN)||
                        (1ULL << OR_TRI_R_MEASURING_PIN),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
        #if SOC_GPIO_SUPPORT_PIN_HYS_FILTER
        .hys_ctrl_mode = GPIO_HYS_SOFT_ENABLE,
        #endif
    };

};
#endif
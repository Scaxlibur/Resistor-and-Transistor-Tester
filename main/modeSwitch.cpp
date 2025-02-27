#include "modeSwitch.hpp"

modeSwitch_class::modeSwitch_class()
{
    gpio_config(&gpioConfig);
}

modeSwitch_class::~modeSwitch_class()
{

}

measuringMode_t modeSwitch_class::whichMode()
{
    bool is_ohm200 = gpio_get_level(IS_OHM200_PIN);
    bool is_ohm2K = gpio_get_level(IS_OHM2K_PIN);
    bool is_ohm20K = gpio_get_level(IS_OHM20K_PIN);
    bool is_ohm200K = gpio_get_level(IS_OHM200K_PIN);
    bool transistor_r_or = gpio_get_level(OR_TRI_R_MEASURING_PIN); //暂且认为true（为1）时测量电阻
    if(transistor_r_or){
        if(is_ohm200){
            return ohm200;
        }else if(is_ohm2K){
            return ohm2K;
        }else if(is_ohm20K){
            return ohm20K;
        }else{
            return ohm200K;
        }
    }else{
        return tansistor;
    }
}
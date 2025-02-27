#include "modeSwitch.hpp"

modeSwitch_class::modeSwitch_class()
{
    gpio_config(&gpioConfig);
}

modeSwitch_class::~modeSwitch_class()
{

}

resistorMeasuringType_t modeSwitch_class::whichMode()
{
    bool is_ohm200 = gpio_get_level(IS_OHM200_PIN);
    bool is_ohm2K = gpio_get_level(IS_OHM2K_PIN);
    bool is_ohm20K = gpio_get_level(IS_OHM20K_PIN);
    bool is_ohm200K = gpio_get_level(IS_OHM200K_PIN);
    if(is_ohm200){
        return ohm200;
    }else if(is_ohm2K){
        return ohm2K;
    }else if(is_ohm20K){
        return ohm20K;
    }else{
        return ohm200K;
    }
}
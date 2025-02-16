#ifndef __RESISTOR_HPP__
#define __RESISTOR_HPP__

#include "adc.hpp"

typedef enum{
    ohm200,
    ohm2K,
    ohm20K,
    ohm200K
}resistorMeasuringType_t; //电阻测量挡位

class resistorMeasuring_class
{
private:
    int R_compute(resistorMeasuringType_t Rtype);
    ADC_class ADC; 

public:
    resistorMeasuring_class(/* args */);
    ~resistorMeasuring_class();
};

#endif
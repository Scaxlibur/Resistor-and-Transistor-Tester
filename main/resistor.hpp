#ifndef __RESISTOR_HPP__
#define __RESISTOR_HPP__

#include "adc.hpp"
#include "modeSwitch.hpp"



class resistorMeasuring_class
{
private:
    ADC_class ADC; 

public:
    int R_compute(measuringMode_t Rtype);
    resistorMeasuring_class(/* args */);
    ~resistorMeasuring_class();
};

#endif
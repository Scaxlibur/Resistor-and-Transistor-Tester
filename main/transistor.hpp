#ifndef __transistor_HPP__
#define __transistor_HPP__

#include "adc.hpp"
#include "modeSwitch.hpp"

class transistorMeasuring_class
{
private:
    
    ADC_class ADC; 

public:
    transistorMeasuring_class(/* args */);
    ~transistorMeasuring_class();
    int beta_compute(measuringMode_t transistorType);
};




#endif
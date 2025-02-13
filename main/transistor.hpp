#ifndef __transistor_HPP__
#define __transistor_HPP__

#include "adc.hpp"

typedef enum {
    PNP,
    NPN
}transistorType_t;

class transistorMeasuring_class
{
private:
    int beta_compute(transistorType_t transistorType);
    ADC_class ADC; 

public:
    transistorMeasuring_class(/* args */);
    ~transistorMeasuring_class();
};




#endif
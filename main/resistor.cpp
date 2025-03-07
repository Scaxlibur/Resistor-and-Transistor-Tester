#include "resistor.hpp"

resistorMeasuring_class::resistorMeasuring_class()
{

}

resistorMeasuring_class::~resistorMeasuring_class(){
}

int resistorMeasuring_class::R_compute(measuringMode_t Rtype)
{
    //int voltage_x10 = ADC.voltage_read_x10();
    switch (Rtype)
    {
    case ohm200:
        /* code */
        break;
    case ohm2K:
        /* code */
        break;
    case ohm20K:
        /* code */
        break;
    case ohm200K:
        /* code */
        break;
    default:
        break;
    }
    return 0;
}

#include "resistor.hpp"

resistorMeasuring_class::resistorMeasuring_class()
{

}

resistorMeasuring_class::~resistorMeasuring_class(){
    delete &ADC;    //ADC类的构造在这个类的私有成员里面，没有在构造函数单独构造
}

int resistorMeasuring_class::R_compute(resistorMeasuringType_t Rtype)
{
    int voltage_x10 = ADC.voltage_read_x10();
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

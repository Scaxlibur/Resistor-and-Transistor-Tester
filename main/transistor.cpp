#include "transistor.hpp"

transistorMeasuring_class::transistorMeasuring_class()
{
}

transistorMeasuring_class::~transistorMeasuring_class()
{
    delete &ADC;
}

int transistorMeasuring_class::beta_compute(transistorType_t transistorType){ //这个函数的返回值到底是什么类型还需要思考
    if(transistorType == PNP){
        int voltage_x10 = ADC.voltage_read_x10();
        int beta = voltage_x10 / 10;//公式：Iec = Vadc/10, beta = Iec / 0.1 
        return beta;
    }
    else if (transistorType == NPN)
    {
        int voltage_x10 = ADC.voltage_read_x10();
        int beta = voltage_x10 * 200;//公式：Iec = 2*Vadc*100, beta = Iec / 0.1 
        return beta;
    }
    return 0;
}
#include "transistor.hpp"

transistorMeasuring_class::transistorMeasuring_class()
{

}

transistorMeasuring_class::~transistorMeasuring_class()
{
    delete &ADC;    //ADC类的构造在这个类的私有成员里面，没有在构造函数单独构造
}

float transistorMeasuring_class::beta_compute(measuringMode_t transistorType){ //这个函数的返回值到底是什么类型还需要思考
    if(transistorType == PNP){
        int voltage_x10 = ADC.voltage_read_x10();
        float beta = voltage_x10 *25 * 100;//公式：beta = Vadc * 25 * 1000
        return beta;
    }
    else if (transistorType == NPN)
    {
        int voltage_x10 = ADC.voltage_read_x10();
        float beta = (5 - voltage_x10) * 2500;//公式：beta = (5 - Vadc) * 25 * 1000 
        return beta;
    }
    return 0;
}
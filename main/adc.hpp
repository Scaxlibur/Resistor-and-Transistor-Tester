#ifndef __ADC_HPP__
#define __ADC_HPP__

#include "esp_adc/adc_oneshot.h"
#define MAX_VOTAGE_x10 33  //ADC最大读取电压的10倍，乘10是为了浮点计算优化

class ADC_class
{
    public:
    ADC_class();
    int voltage_read_x10();

    private:
    
    adc_oneshot_unit_handle_t ADC_handle;
    adc_oneshot_unit_init_cfg_t ADC_init_config = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    adc_oneshot_chan_cfg_t ADC_chan_config = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
};

#endif
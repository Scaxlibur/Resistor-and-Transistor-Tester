#include "adc.hpp"

ADC_class::ADC_class()
{
    ESP_ERROR_CHECK(adc_oneshot_config_channel(ADC_handle, ADC_CHANNEL_0, &ADC_chan_config));
}

ADC_class::~ADC_class()
{
    ESP_ERROR_CHECK(adc_oneshot_del_unit(ADC_handle));
}

int ADC_class::voltage_read_x10()
{
    int *Dout = NULL;
    ESP_ERROR_CHECK(adc_oneshot_read(ADC_handle, ADC_CHANNEL_0, Dout));
    int output_10x = (*Dout * MAX_VOTAGE_x10) >> ADC_BITWIDTH_DEFAULT;  //转换公式Vout = Dout * Vmax / Dmax
    return output_10x;
}
 
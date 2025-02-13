#include "adc.hpp"

ADC::ADC()
{
    ESP_ERROR_CHECK(adc_oneshot_config_channel(ADC_handle, ADC_CHANNEL_0, &ADC_chan_config));
}

int ADC::votage_read_x10()
{
    int *Dout;
    ESP_ERROR_CHECK(adc_oneshot_read(ADC_handle, ADC_CHANNEL_0, Dout));
    int output_10x = (*Dout * MAX_VOTAGE_x10) >> ADC_BITWIDTH_DEFAULT;  //转换公式Vout = Dout * Vmax / Dmax
    return output_10x;
}
 
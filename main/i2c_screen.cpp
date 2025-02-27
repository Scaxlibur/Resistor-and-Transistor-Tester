#include "i2c_screen.hpp"

I2Cscreen_class::I2Cscreen_class(/* args */)
{
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config, &i2c_master_bus_handle));
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_master_bus_handle, &dev_cfg, &i2c_master_dev_handle));
    
}

I2Cscreen_class::~I2Cscreen_class()
{
    i2c_master_bus_rm_device(i2c_master_dev_handle);
}

int I2Cscreen_class::updateScreen(uint8_t *data_wr)
{
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_master_dev_handle, data_wr, sizeof(data_wr), -1));
    return 0;
}
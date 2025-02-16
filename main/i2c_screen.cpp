#include "i2c_screen.hpp"

i2c_screen::i2c_screen(/* args */)
{
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_config, &i2c_master_bus_handle));
    ESP_ERROR_CHECK(i2c_master_bus_add_device(i2c_master_bus_handle, &dev_cfg, &i2c_master_dev_handle));
    
}

i2c_screen::~i2c_screen()
{
    i2c_master_bus_rm_device(i2c_master_dev_handle);
}

int i2c_screen::updateScreen(uint8_t *data_wr)
{
    ESP_ERROR_CHECK(i2c_master_transmit(i2c_master_dev_handle, data_wr, sizeof(data_wr), -1));
}
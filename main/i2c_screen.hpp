#ifndef __I2C_SCREEN_HPP__
#define __I2C_SCREEN_HPP__

#include "driver/i2c_master.h"

#define SDA_IO_NUM GPIO_NUM_0
#define SCL_IO_NUM GPIO_NUM_1

class I2Cscreen_class
{
private:
    i2c_master_bus_config_t i2c_config ={
        .i2c_port = -1,
        .sda_io_num = SDA_IO_NUM,
        .scl_io_num = SCL_IO_NUM,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7, //设置主机总线的毛刺周期。如果线上的毛刺周期小于设置的值（通常设为 7），则可以被滤除。
        .flags = {
            .enable_internal_pullup = 1,
            .allow_pd = 1,
        },
    };
    i2c_device_config_t dev_cfg = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x58,
        .scl_speed_hz = 100000,
    };
    i2c_master_bus_handle_t i2c_master_bus_handle;
    i2c_master_dev_handle_t i2c_master_dev_handle;
    

public:
    I2Cscreen_class(/* args */);
    ~I2Cscreen_class();
    int updateScreen(uint8_t *data_wr);
};


#endif
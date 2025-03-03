#include "UART_screen.hpp"

void UARTscreen_class::UARTWriteBytes(uint8_t data)
{
    uart_write_bytes(UART_NUM_SCREEN, &data, 1);
}


void UARTscreen_class::UARTWriteBytes(uint16_t data)
{
    uint8_t data1 = data >> 8;
    uart_write_bytes(UART_NUM_SCREEN, &data1, 1);
    uart_write_bytes(UART_NUM_SCREEN, &data, 1);
}

void UARTscreen_class::UARTWriteBytes(uint32_t data)
{
    uint8_t data1 = data >> 24;
    uint8_t data2 = data >> 16;
    uint8_t data3 = data >> 8;
    uart_write_bytes(UART_NUM_SCREEN, &data1, 1);
    uart_write_bytes(UART_NUM_SCREEN, &data2, 1);
    uart_write_bytes(UART_NUM_SCREEN, &data3, 1);
    uart_write_bytes(UART_NUM_SCREEN, &data, 1);
}

void UARTscreen_class::UARTWriteBytes(const char* data)
{
    for(size_t i = 0; i < strlen(data); i++)
    {
        uart_write_bytes(UART_NUM_SCREEN, (data + i), sizeof(char));
    }
}

void UARTscreen_class::UARTcommandEnd(void)
{
    uart_write_bytes(UART_NUM_SCREEN, "\xff\xff\xff", 3);
}

UARTscreen_class::UARTscreen_class()
{
    init_uart();
}

UARTscreen_class::~UARTscreen_class()
{
}

void UARTscreen_class::init_uart(void){
    if(uart_is_driver_installed(UART_NUM_SCREEN) == false){
        ESP_ERROR_CHECK(uart_param_config(UART_NUM_SCREEN, &uart_config));
        ESP_ERROR_CHECK(uart_set_pin(UART_NUM_SCREEN, UART_NUM_SCREEN_TX, UART_NUM_SCREEN_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
        ESP_ERROR_CHECK(uart_driver_install(UART_NUM_SCREEN, RX_BUF_SIZE * 2, 0, 0, NULL, 0));
    };
}

void UARTscreen_class::command_receive(data4Tasks com)
{
    int length = 0;
    uart_get_buffered_data_len(UART_NUM_SCREEN, (size_t*)&length);
    if (length == 0) return;// 如果串口是空的直接返回
    char received_chars[10];
    // 从串口读取返回的数据，读取20个字符
    uart_read_bytes(UART_NUM_SCREEN, received_chars, length, 100 / portTICK_PERIOD_MS);

    // 根据指令做不同动作
    if (received_chars[0] == 'R') // A指令，设置为电阻测量模式
    {

    }else if(received_chars[0] == 'T') // T指令，设置为三极管测量模式
    //  最后清空串口
    while (length >= 0)
        uart_flush(UART_NUM_SCREEN); 
}

void UARTscreen_class::command_send(data4Tasks com)
{
    //printf("page0.t2.txt=\"");
    UARTWriteBytes("page0.t2.txt=\"");
    switch (com.measuringMode)
    {
        case ohm200:
            //printf("200");
            UARTWriteBytes("200");
            break;
        case ohm2K:
            //printf("2K");
            UARTWriteBytes("2K");
            break;
        case ohm20K:
            //printf("20K");
            UARTWriteBytes("20K");
            break;
        case ohm200K:
            //printf("200K");
            UARTWriteBytes("200K");
            break;
        case transistor:
        case NPN:
            //printf("NPN");
            UARTWriteBytes("NPN");
            break;
        case PNP:
            //printf("PNP");
            UARTWriteBytes("PNP");
            break;
    }
    //printf("\"\xff\xff\xff");
    UARTWriteBytes("\"");
    UARTcommandEnd();
    
    com.measuringValue_int = com.measuringValue * 1000; //把测量值转换为伪浮点数（后三位是小数）的字符串
    UARTWriteBytes("page0.x0.val=");
    char change[32];
    itoa(com.measuringValue_int, change, 10);
    UARTWriteBytes(change);
    UARTcommandEnd();
}




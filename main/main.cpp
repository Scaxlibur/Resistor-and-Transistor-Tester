#include <stdio.h>
#include "freertos/FreeRTOS.h"

extern "C" void app_main(void)
{
  while (true)
  {
    vTaskDelay(45 / portTICK_PERIOD_MS);
  }
}
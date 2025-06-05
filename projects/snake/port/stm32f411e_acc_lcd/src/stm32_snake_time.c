#include "snake_time.h"
#include "stm32_init.h"
#include "stm32f4xx_hal.h"


int snake_time_init(__attribute__((unused)) snake_game_t* p_game)
{
    return stm32_init();
}

void snake_time_close(__attribute__((unused)) snake_game_t* p_game)  {
}

void snake_time_delay_ms (uint32_t ms) {
    HAL_Delay(ms);
}





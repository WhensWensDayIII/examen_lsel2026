#include "snake_input.h"
#include "snake.h"
#include "stm32_bsp.h"
#include "accelero.h"
#include "stm32_init.h"
#include <stdlib.h>  

int snake_input_init(snake_game_t* p_game)
{
    if (stm32_init()) {
        ACCELERO_DrvTypeDef* acc = bsp_get_accelero();
        uint16_t acc_config = bsp_get_accelero_config();
        acc->Init(acc_config);
    
        return 1;      
    }
    return 0;
}

void
snake_input_close(snake_game_t* p_game) {
    // No hacer nada
}

void
snake_input_update_new_heading (snake_game_t* p_game) {
    int16_t xyz[3] = {0};

    ACCELERO_DrvTypeDef* acc = bsp_get_accelero();
    acc->GetXYZ(xyz);
    
     int16_t abs_x = abs(xyz[0]);
    int16_t abs_y = abs(xyz[1]);
    int16_t abs_z = abs(xyz[2]);
    
    if (abs_x > abs_y && abs_x > abs_z){
        if(xyz[0] > 0){
            p_game->new_heading = UP;
        } else {
            p_game->new_heading = DOWN;
        }
    } else if (abs_y > abs_z){
        if(xyz[1] > 0){
            p_game->new_heading = LEFT;
        } else {
            p_game->new_heading = RIGHT;
        }
    } else {
        p_game->new_heading = NONE;
    }
}
    





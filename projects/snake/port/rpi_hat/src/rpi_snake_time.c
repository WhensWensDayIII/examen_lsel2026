#include "snake_time.h"

#include <unistd.h>

int 
snake_time_init (__attribute__((unused)) snake_game_t* p_game) 
{
  /* No hace nada */
  return 1;
}

void 
snake_time_close (__attribute__((unused)) snake_game_t* p_game) 
{
  /* No hace nada */
}

void 
snake_time_delay_ms (uint32_t ms) 
{
  /* Get from original code */
  /* Delay argument in ms */
  usleep(ms*1000);
}

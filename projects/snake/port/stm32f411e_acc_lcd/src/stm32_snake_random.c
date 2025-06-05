#include "snake_random.h"
#include "stm32_init.h"
#include <stdlib.h>
#include <time.h>


int snake_random_init(snake_game_t* p_game)
{
  if (stm32_init()) {
    srand (time(NULL));
    return 1;
  }
  return 0;
}

void
snake_random_close(snake_game_t* p_game)
{
}

int
snake_random_get(int limit)
{
  return rand() % limit;
}




#include "snake_time.h"                                                         
#include <stdio.h>

#ifndef SAMPLE_TIME_DURATION
#define SAMPLE_TIME_DURATION	1000
#endif

#ifndef SAMPLE_TIME_COUNT
#define SAMPLE_TIME_COUNT	10
#endif

void
sample_time_1()
{ 
  int i;
  snake_game_t game;

  snake_time_init(&game);
  printf("Begin");
  fflush(stdout);
  for (i = 0; i < SAMPLE_TIME_COUNT; i++) {
    printf(".");
    fflush(stdout);
    snake_time_delay_ms(SAMPLE_TIME_DURATION);
  }
  snake_time_close(&game);
  printf("End\r\n");
  fflush(stdout);
} 

int main() {
  sample_time_1();
}

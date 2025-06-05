#include "snake_random.h"                                                         
#include <stdio.h>

#ifndef SAMPLE_RANDOM_COUNT
#define SAMPLE_RANDOM_COUNT	10
#endif

#ifndef SAMPLE_RANDOM_LIMIT
#define SAMPLE_RANDOM_LIMIT	16
#endif

void
sample_random_1()
{ 
  int i;
  int r;
  snake_game_t game;

  snake_random_init(&game);
  printf("Begin\r\n");
  fflush(stdout);
  for (i = 0; i < SAMPLE_RANDOM_COUNT; i++) {
    r = snake_random_get(SAMPLE_RANDOM_LIMIT);
    printf("R(%d): %d\r\n", i, r);
    fflush(stdout);
  }
  snake_random_close(&game);
  printf("End\r\n");
  fflush(stdout);
} 

int main() {
  sample_random_1();
  return 0;
}

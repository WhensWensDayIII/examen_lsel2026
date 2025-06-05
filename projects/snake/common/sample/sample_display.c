#include "snake_display.h"
#include "snake_time.h"
#include <stdio.h>
#include <stdlib.h>

#ifndef SAMPLE_DISPLAY_DURATION
#define SAMPLE_DISPLAY_DURATION 5000
#endif

void
sample_display_1()
{
  snake_game_t game;
  struct segment_t * new_tail;
  struct segment_t * last_tail;

  if (!snake_time_init(&game)) {
    printf("Error: time_init\r\n");
    return;
  }
  if (!snake_display_init(&game)) {
    printf("Error: display_init\r\n");
    return;
  }

  printf("Game field with %d rows and %d columns\r\n", game.limits.x, game.limits.y);
  printf("Head in top left corner\r\n");
  printf("Body continues vertically down the column until mid-column\r\n");
  printf("Body continues horizontally to the right towards the center of the game field\r\n");
  printf("Apple in bottom right corner\r\n");

  game.snake.head.x = 0;
  game.snake.head.y = 0;
  game.snake.head.next = NULL;
  game.snake.tail = &(game.snake.head);
  for (int i = 1; i < game.limits.x/2; i++) {
    new_tail = malloc(sizeof(struct segment_t));
    new_tail->x = game.snake.tail->x + 1;
    new_tail->y = game.snake.tail->y;
    new_tail->next = game.snake.tail;
    game.snake.tail = new_tail;
  }

  for (int i = 1; i < game.limits.y/2; i++) {
    new_tail = malloc(sizeof(struct segment_t));
    new_tail->x = game.snake.tail->x;
    new_tail->y = game.snake.tail->y + 1;
    new_tail->next = game.snake.tail;
    game.snake.tail = new_tail;
  }

  game.apple.x = game.limits.x - 1;
  game.apple.y = game.limits.y - 1;

  snake_display_render(&game);
  snake_time_delay_ms(SAMPLE_DISPLAY_DURATION);

  for(last_tail = game.snake.tail; ((last_tail != NULL) && (last_tail != &(game.snake.head))) ; ) {
    new_tail = last_tail->next;
    free(last_tail);
    last_tail = new_tail;
  }

  snake_display_close(&game);
  snake_time_close(&game);
}

int main()
{
  sample_display_1();
  return 0;
}

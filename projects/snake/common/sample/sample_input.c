#include "snake_input.h"                                                         
#include "snake_time.h"                                                         
#include <stdio.h>
      
#ifndef SAMPLE_INPUT_CHANGES
#define SAMPLE_INPUT_CHANGES 10
#endif

#ifndef SAMPLE_INPUT_TIME
#define SAMPLE_INPUT_TIME 200
#endif

static char *heading_str[] = {                                                     
  "LEFT",
  "DOWN",
  "RIGHT",
  "UP",
  "NONE"
};
  
void
sample_input_1()
{ 
  int i;
  snake_game_t game;                                                               
  
  if (!snake_time_init(&game)) {
    printf("Error: time_init\r\n");
    return;
  }
  if (!snake_input_init(&game)) {
    printf("Error: input_init\r\n");
    return;
  }

  game.new_heading = NONE;
  game.snake.heading = NONE;
  
  printf("Perform %d heading changes\r\n", SAMPLE_INPUT_CHANGES);                       
  fflush(stdout);
  for (i = 0; i < SAMPLE_INPUT_CHANGES ; i++) {                                         
    printf("Use the platform to change the heading of the snake\r\n");              
    printf("Current(%d): %d %s\r\n", i, game.snake.heading, heading_str[game.snake.heading]);
    fflush(stdout);
         
    while (game.snake.heading == game.new_heading) {                               
      snake_time_delay_ms(SAMPLE_INPUT_TIME);                                                    
      printf(".");                                                                 
      fflush(stdout);
      snake_input_update_new_heading(&game);                                       
    }                                                                              
    game.snake.heading = game.new_heading;
    printf("Heading changed\r\n");                                                 
    fflush(stdout);
  }

  snake_input_close(&game);
  snake_time_close(&game);
} 

int main() {
  sample_input_1();
  return 0;
}

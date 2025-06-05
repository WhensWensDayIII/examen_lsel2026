#include "snake_display.h"

#define _GNU_SOURCE

#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_X 16
#define MAX_Y 16

#define CHAR_APPLE '*'
#define CHAR_HEAD '@'
#define CHAR_BODY 'o'
#define CHAR_EMPTY ' '

struct fb_t {
  char pixel[MAX_X][MAX_Y];
};
static struct fb_t fb_s;
static struct fb_t* fb = &fb_s;

static void
clear_screen()
{
  printf("\e[1;1H\e[2J");
  fsync(1);
}

static void
print_hlimit(snake_game_t* p_game)
{
  // TODO: imprime + , - por cada columna , +
  putchar('+');
  for (int i = 0; i < p_game->limits.y; ++i) {
    putchar('-');
  }
  putchar('+');
  putchar('\n');
}


static void
print_row(snake_game_t* p_game, int x)
{
  putchar('|');
  for (int i = 0; i < p_game->limits.y; ++i) {
    putchar(fb->pixel[x][i]);
  }
  putchar('|');
  putchar('\n');  
} 

void 
snake_display_render(snake_game_t* p_game)
{
  /* Change: Each element is a char */
  /* - Apple is CHAR_APPLE */
  /* - Snake head is CHAR_HEAD */
  /* - Snake body CHAR_BODY */
  /* - Empty pixels are CHAR_EMPTY */
  //struct segment_t *seg_i;

  /* Set Blank */
  memset(fb, CHAR_EMPTY, sizeof(struct fb_t));

  // Apple
  fb->pixel[p_game->apple.x][p_game->apple.y] = CHAR_APPLE;

  // Snake body (except head)
  for (struct segment_t* seg = p_game->snake.tail; seg && seg->next; seg = seg->next) {
    fb->pixel[seg->x][seg->y] = CHAR_BODY;
  }

  // Snake head
  fb->pixel[p_game->snake.head.x][p_game->snake.head.y] = CHAR_HEAD;

  clear_screen();

  print_hlimit(p_game);
  for (int x = 0; x < p_game->limits.x; x++) {
    print_row(p_game, x);
  }
  print_hlimit(p_game);

  fflush(NULL); 
}

int
snake_display_init(snake_game_t* p_game)
{
  p_game->limits = (struct apple_t){ .x = MAX_X, .y = MAX_Y };

  return 1;
}

void
snake_display_close(__attribute__((unused)) snake_game_t* p_game)
{
}

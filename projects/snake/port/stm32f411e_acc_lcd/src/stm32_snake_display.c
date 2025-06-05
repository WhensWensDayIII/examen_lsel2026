#include "snake.h"
#include "stm32_init.h"
#include "stm32_bsp.h"
#include "accelero.h"
#include <stdlib.h>
#include <string.h>

#include "snake_display.h"
#include "ssd1306.h"
#include "fonts.h"

#define MAX_X 8
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
  
int 
snake_display_init(snake_game_t* p_game)
{
    p_game->limits.x = MAX_X;
    p_game->limits.y = MAX_Y; 

    if (stm32_init()) {
        if (lcd_init()) {
            return 1;
        }
    }

    return 0;
}

void snake_display_close(snake_game_t* p_game)
{
    lcd_reset();    
}

void snake_display_render(snake_game_t* p_game)
{
    /* Set Blank */
    memset(fb, CHAR_EMPTY, sizeof(struct fb_t));

    fb->pixel[p_game->apple.x][p_game->apple.y] = CHAR_APPLE;

    // Snake body (except head)
    for (struct segment_t* seg = p_game->snake.tail; seg && seg->next; seg = seg->next) {
        fb->pixel[seg->x][seg->y] = CHAR_BODY;
    }

    // Snake head
    fb->pixel[p_game->snake.head.x][p_game->snake.head.y] = CHAR_HEAD;

    for (int x = 0; x < p_game->limits.x; x++) {
        lcd_set_cursor(x, 0);
        for (int y = 0; y < p_game->limits.y; y++) {
            lcd_write_char(fb->pixel[x][y]);
        }
    }

    lcd_update_screen();
}
    





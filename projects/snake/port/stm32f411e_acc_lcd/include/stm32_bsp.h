#ifndef _STM32_BSP_H
#define _STM32_BSP_H

#include <stdint.h>
#include "accelero.h"

int stm32_i2c_init (void);
ACCELERO_DrvTypeDef* bsp_get_accelero(void);
uint16_t bsp_get_accelero_config(void);

int lcd_init(void);
void lcd_reset(void);
void lcd_update_screen(void);
void lcd_set_cursor(uint8_t x, uint8_t y);
void lcd_write_char(char ch);

#endif //_STM32_BSP_H

#ifndef LCD_UI_H
#define LCD_UI_H

#include <stdint.h>

void draw_player(uint8_t row, uint8_t *dice, uint8_t *sel); //rysuje kostki i zaznaczenia do przerzutu
void draw_cursor(uint8_t cursor, //ale w znaczeniu - indeks kostki 
                 uint8_t player,
                 uint8_t cursor_active,
                 uint8_t phase_result); // do rysowania "^"

#endif

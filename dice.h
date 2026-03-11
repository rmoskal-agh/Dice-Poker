#ifndef DICE_H
#define DICE_H

#include <stdint.h>

#define DICE_COUNT 5

void roll_all(uint8_t *dice); //losowanie kostek
void reroll_selected(uint8_t *dice, uint8_t *sel); //przerzut, tablica sel to kostki do przerzutu 
void sort_desc(uint8_t *dice); //sortowanie kostek malej¹co

#endif

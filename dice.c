#include "dice.h"
#include <stdlib.h>

void roll_all(uint8_t *dice)
{
    for(int i = 0; i < DICE_COUNT; i++)
        dice[i] = (rand() % 6) + 1;
} //losowanie liczb od 1 do 6

void reroll_selected(uint8_t *dice, uint8_t *sel)
{
    for(int i = 0; i < DICE_COUNT; i++)
    {
        if(sel[i]) //przerzuca te kostki, oznaczone 1, czyli "^"
        {
            dice[i] = (rand() % 6) + 1;
            sel[i] = 0;
        }
    }
}

void sort_desc(uint8_t *dice) //b¹belkowy algorytm sortuj¹cy malej¹co
{
    for(int i = 0; i < DICE_COUNT - 1; i++)
    {
        for(int j = 0; j < DICE_COUNT - 1 - i; j++)
        {
            if(dice[j] < dice[j+1]) 
            {
                uint8_t t = dice[j];
                dice[j] = dice[j+1];
                dice[j+1] = t;
            }
        }
    }
}

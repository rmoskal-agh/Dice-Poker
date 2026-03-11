#include "lcd_ui.h"
#include "lcd1602.h"

#define DICE_COUNT 5

void draw_player(uint8_t row, uint8_t *dice, uint8_t *sel)
{
    char c[2];
    c[1] = 0;

    LCD1602_SetCursor(0, row);  //gracz pierwszy row == 0, gracz drugi row == 1 

    for(int i = 0; i < DICE_COUNT; i++)
    {
        if(sel[i])
            c[0] = 'x';
        else
            c[0] = '0' + dice[i]; // zapis dla znaku ASCI, nie mo¿na daæ samego dice[i] bo dziej¹ siê rzeczy niestworzone

        LCD1602_Print(c);
        LCD1602_Print(" ");
    }
}

void draw_cursor(uint8_t cursor, uint8_t player, uint8_t cursor_active, uint8_t phase_result)
{
    if(!cursor_active || phase_result)
        return;

    LCD1602_SetCursor(cursor * 2, player); //poniewa¿ cyfra + spacja sprawia ¿e ka¿da kostka zawiera 2 znaki
    LCD1602_Print("^");
}

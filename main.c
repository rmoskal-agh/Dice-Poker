/*-------------------------------------------------------------------------
    Projekt: Kościany Poker – MAIN
---------------------------------------------------------------------------*/
#include "MKL05Z4.h"
#include "frdm_bsp.h"
#include "lcd1602.h"
#include "klaw.h"
#include "dice.h"
#include "lcd_ui.h"
#include "wyniki.h"
#include <stdlib.h>

#define DICE_COUNT   5
#define MAX_REROLLS  2 //liczba przerzutów

uint8_t dice_p1[DICE_COUNT];
uint8_t dice_p2[DICE_COUNT];
uint8_t sel_p1[DICE_COUNT] = {0};
uint8_t sel_p2[DICE_COUNT] = {0}; //kostki zaznaczone do przerzutu

uint8_t cursor = 0;
uint8_t player = 0;
uint8_t cursor_active = 0;
uint8_t reroll_count = 0;
uint8_t phase_result = 0;
uint8_t redraw = 1;

// losowość z szumu
static void ADC_Init_Noise(void)
{
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    PORTB->PCR[11] &= ~PORT_PCR_MUX_MASK;

    ADC0->CFG1 = ADC_CFG1_MODE(1) |
                 ADC_CFG1_ADIV(1) |
                 ADC_CFG1_ADICLK(0);
    ADC0->SC2 = ADC_SC2_REFSEL(0);
}

static uint16_t ADC_Read(uint8_t ch)
{
    ADC0->SC1[0] = ADC_SC1_ADCH(ch);
    while(!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
    return ADC0->R[0];
}
void show_summary(HandResult h1, HandResult h2) //wynik słownie; wykonywany na końcu 
{
    LCD1602_ClearAll();

    if(h1.rank == h2.rank && h1.value == h2.value)
    {
        LCD1602_SetCursor(0,0);
        LCD1602_Print("REMIS ");
        LCD1602_Print(rank_name(h1.rank));
        return;
    }

    if(compare_hands(h1, h2) > 0)
    {
        LCD1602_SetCursor(0,0);
        LCD1602_Print(rank_name(h1.rank));
        LCD1602_Print(" BIJE");

        LCD1602_SetCursor(0,1);
        LCD1602_Print(rank_name(h2.rank));
    }
    else
    {
        LCD1602_SetCursor(0,0);
        LCD1602_Print(rank_name(h2.rank));
        LCD1602_Print(" BIJE");

        LCD1602_SetCursor(0,1);
        LCD1602_Print(rank_name(h1.rank));
    }
		
}

int main(void)
{
    LCD1602_Init();
    LCD1602_Backlight(TRUE);
    LCD1602_ClearAll();

    Klaw_Init();
    Klaw_S1_4_Int(); //włączenie wszystkich 4 przycisków 

    ADC_Init_Noise();
    srand(ADC_Read(11)); //tak jakby ustawia seed 

    roll_all(dice_p1);
    roll_all(dice_p2);

    while(1)
    {
        // ruchy kursora
        if(S2_press && !phase_result)
        {
            if(!cursor_active)
            {
                cursor_active = 1;
                cursor = 0;
                player = 0;
            }
            else
            {
                if(cursor < DICE_COUNT-1) //zmiana gracza gdy kursor na 5 kostce
                    cursor++;
                else
                {
                    cursor = 0;
                    player ^= 1;
                }
            }
            S2_press = 0;
            redraw = 1;
        }

        if(!(PTA->PDIR & S1_MASK) && cursor_active && !phase_result)
        {
            DELAY(200);
            if(cursor > 0) cursor--;
            redraw = 1;
        } //cofa do kostki nr 1 danego gracza

        if(S3_press && cursor_active && !phase_result)
        {
            if(player == 0) sel_p1[cursor] ^= 1; //zaznaczanie do przerzutu 
            else            sel_p2[cursor] ^= 1;
            S3_press = 0;
            redraw = 1;
        }

        if(S4_press && cursor_active && !phase_result)
        {
            reroll_selected(dice_p1, sel_p1);
            reroll_selected(dice_p2, sel_p2);
            reroll_count++;

            cursor_active = 0;
            cursor = 0;
            player = 0;

            if(reroll_count >= MAX_REROLLS) //drugi przerzut
            {
                phase_result = 1;

                LCD1602_ClearAll();
                draw_player(0, dice_p1, sel_p1);
                draw_player(1, dice_p2, sel_p2);
                DELAY(8000);

                sort_desc(dice_p1);
                sort_desc(dice_p2);
            }

            S4_press = 0;
            redraw = 1;
        }

        if(redraw)
        {
            LCD1602_ClearAll();
            draw_player(0, dice_p1, sel_p1);
            draw_player(1, dice_p2, sel_p2);

            if(!phase_result)
            {
                draw_cursor(cursor, player, cursor_active, phase_result);
            }
            else
            {
                HandResult h1 = evaluate_hand(dice_p1);
                HandResult h2 = evaluate_hand(dice_p2);
                int r = compare_hands(h1, h2);

                if(r > 0)
                {
                    LCD1602_SetCursor(12,0);
                    LCD1602_Print("WIN");
                }
                else if(r < 0)
                {
                    LCD1602_SetCursor(12,1);
                    LCD1602_Print("WIN");
                }
                else
                {
                    LCD1602_SetCursor(12,0);
                    LCD1602_Print("DRAW");
                    LCD1602_SetCursor(12,1);
                    LCD1602_Print("DRAW");
                }
							DELAY(40000);          
							show_summary(h1, h2);
							DELAY(12000);         

            }
            redraw = 0;
        }
    }
}




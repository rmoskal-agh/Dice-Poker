#ifndef WYNIKI_H
#define WYNIKI_H

#include <stdint.h>

#define DICE_COUNT 5 //liczba koœci, zmienienie jej generalnie nie ma sensu dla tej gry, ale dajê tak¹ opcjê jako mo¿liwoœæ rozbudowy 

typedef enum { //uk³ady od najs³abszego do najsilniejszego
    RANK_NONE = 0,
    RANK_PAIR,
    RANK_TWO_PAIRS,
    RANK_TRIPLE,
    RANK_SMALL_STRAIGHT,
    RANK_BIG_STRAIGHT,
    RANK_FULL,
    RANK_FOUR,
    RANK_POKER
} HandRank; //wszystkie uk³ady pokerowe dla tej gry

typedef struct {
    HandRank rank;
    uint8_t value; //wartoœæ kostki punktuj¹cej do rozgrzygania remisów np 3 w uk³adzie trójka 3-3-3-2-1, w uk³adzie dwie pary - ta wy¿sza kostka 
} HandResult;

HandResult evaluate_hand(uint8_t *dice); //zwraca rangê analizuj¹c rzuty kostkami
int compare_hands(HandResult a, HandResult b); //porównanie r¹k

const char* rank_name(HandRank r); //zmiana nazwy rangi na polsk¹ 

#endif



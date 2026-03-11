#include "wyniki.h"

static void count_values(uint8_t *dice, uint8_t *cnt) //zlicza wyst¹pienia ka¿dej kostki 
{
    for(int i=0;i<7;i++) cnt[i]=0;
    for(int i=0;i<DICE_COUNT;i++)
        cnt[dice[i]]++;
}
const char* rank_name(HandRank r)
{
    switch(r)
    {
        case RANK_POKER:          return "POKER";
        case RANK_FOUR:           return "KARETA";
        case RANK_FULL:           return "FUL";
        case RANK_BIG_STRAIGHT:   return "DUZY STRIT"; //z bólem serca nie mogê u¿ywaæ tutaj polskich znaków
        case RANK_SMALL_STRAIGHT: return "MALY STRIT";
        case RANK_TRIPLE:         return "TROJKA";
        case RANK_TWO_PAIRS:      return "2 PARY";
        case RANK_PAIR:           return "PARA";
        default:                  return "NIC"; 
    }
}


HandResult evaluate_hand(uint8_t *dice) //szuka najlepszej mo¿liwej kombinacji
{
    HandResult res = {RANK_NONE, 0};
    uint8_t cnt[7];
    count_values(dice, cnt);

    uint8_t pairs = 0;
    uint8_t triple = 0;
    uint8_t four = 0;
    uint8_t five = 0;

    for(int i=1;i<=6;i++)
    {
        if(cnt[i] == 5) five = i;
        if(cnt[i] == 4) four = i;
        if(cnt[i] == 3) triple = i; //trójka, czwórka i pi¹tka mo¿e byæ tylko jedna; zwróci odrazu wartoœæ punktuj¹c¹
        if(cnt[i] == 2) pairs++; //par mo¿e byæ maks 2
    }

    // POKER (ale czasem zwany te¿ pi¹tka)
    if(five)
        return (HandResult){RANK_POKER, five};

    //KARETA
    if(four)
        return (HandResult){RANK_FOUR, four};

    //FULL
    if(triple && pairs)
        return (HandResult){RANK_FULL, triple};

    //DU¯Y STRIT
    if(cnt[6]&&cnt[5]&&cnt[4]&&cnt[3]&&cnt[2])
        return (HandResult){RANK_BIG_STRAIGHT, 6};

    // MA£Y STRIT
    if(cnt[5]&&cnt[4]&&cnt[3]&&cnt[2]&&cnt[1])
        return (HandResult){RANK_SMALL_STRAIGHT, 5}; //drugi argument w stritach jest czystym ozdobnikiem i nic nie robi

    //TRÓJKA
    if(triple)
        return (HandResult){RANK_TRIPLE, triple};

    //DWIE PARY
    if(pairs == 2)
        for(int i=6;i>=1;i--)
            if(cnt[i]==2)
                return (HandResult){RANK_TWO_PAIRS, i}; //ma zwróciæ tê wy¿sz¹ parê

    //PARA
    if(pairs == 1)
        for(int i=6;i>=1;i--)
            if(cnt[i]==2)
                return (HandResult){RANK_PAIR, i};

    //NIC
    return (HandResult){RANK_NONE, dice[0]};
}

int compare_hands(HandResult a, HandResult b)
{
    if(a.rank > b.rank) return 1;
    if(a.rank < b.rank) return -1;
//gdy kombinacja taka sama
    if(a.value > b.value) return 1;
    if(a.value < b.value) return -1;
//gdy remis
    return 0;
}




/**
 *  Papryka
 *
 *  \date   2008-05-13
 *  \author STF
 */

#include <stdio.h>
#include <string.h>

// max wykladnik (uwzglednia dodawanie papryk)
#define MaxN (25)

// max liczba papryczek o danej masie
#define MaxP (1000)

const long  pow2[28] = { 0x1,       0x2,       0x4,       0x8,
                         0x10,      0x20,      0x40,      0x80,
                         0x100,     0x200,     0x400,     0x800,
                         0x1000,    0x2000,    0x4000,    0x8000,
                         0x10000,   0x20000,   0x40000,   0x80000,
                         0x100000,  0x200000,  0x400000,  0x800000,
                         0x1000000, 0x2000000, 0x4000000, 0x8000000 };

int papryka[MaxN + 2];
int maxWyk = 0;

void init() {
    memset(papryka, 0, sizeof(papryka));
}

void czytaj() {
    int i = 0;

    scanf("%d", &maxWyk);
    for (i = 0; i <= maxWyk; i++) {
        scanf("%d", papryka + i);
    }
}

long oblicz() {
     int    i = 0;
     int    r = 0;  // przeniesienie
    long suma = 0;  // masa calej papryki do tej pory

    for (i = 0; ; i++) {
        suma += ((long) pow2[i]) * ((long) papryka[i]);
        r = papryka[i] + r;     // ile papryk jest na miejscu [i]

        if (0 == r)
            return (suma + 1);  // nie ma juz nic --- ale wszystkie liczby az do suma mozna uzyskac

        r = (r - 1) / 2;        // zostawiamy troche papryki, reszte przenosimy dalej
    }
}

int main() {
    init();
    czytaj();

    long wynik = oblicz();
    printf("%ld\n", wynik);

    return 0;
}

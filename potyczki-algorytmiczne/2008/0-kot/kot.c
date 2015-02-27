#include <stdio.h>
#include <string.h>

const int MaxN = 500000;
const int MaxKlaw = 256;

int main() {
    char klawiatura[MaxKlaw + 2];
    char t[MaxN + 2];
    char u[MaxN + 2];
    int  i = 0;
    char c = 0;

    memset(klawiatura, 0, sizeof(klawiatura));
    memset(t, 0, sizeof(t));
    memset(u, 0, sizeof(u));

    klawiatura['`'] = '1';
    klawiatura['1'] = '1';
    klawiatura['2'] = '1';
    klawiatura['3'] = '1';
    klawiatura['4'] = '1';
    klawiatura['5'] = '1';
    klawiatura['6'] = '1';
    klawiatura['7'] = '1';
    klawiatura['8'] = '1';
    klawiatura['9'] = '1';
    klawiatura['0'] = '1';
    klawiatura['-'] = '1';
    klawiatura['='] = '1';

    klawiatura['Q'] = '2';
    klawiatura['W'] = '2';
    klawiatura['E'] = '2';
    klawiatura['R'] = '2';
    klawiatura['T'] = '2';
    klawiatura['Y'] = '2';
    klawiatura['U'] = '2';
    klawiatura['I'] = '2';
    klawiatura['O'] = '2';
    klawiatura['P'] = '2';
    klawiatura['['] = '2';
    klawiatura[']'] = '2';
    klawiatura['\\'] = '2';

    klawiatura['A'] = '3';
    klawiatura['S'] = '3';
    klawiatura['D'] = '3';
    klawiatura['F'] = '3';
    klawiatura['G'] = '3';
    klawiatura['H'] = '3';
    klawiatura['J'] = '3';
    klawiatura['K'] = '3';
    klawiatura['L'] = '3';
    klawiatura[';'] = '3';
    klawiatura['\''] = '3';

    klawiatura['Z'] = '4';
    klawiatura['X'] = '4';
    klawiatura['C'] = '4';
    klawiatura['V'] = '4';
    klawiatura['B'] = '4';
    klawiatura['N'] = '4';
    klawiatura['M'] = '4';
    klawiatura[','] = '4';
    klawiatura['.'] = '4';
    klawiatura['/'] = '4';

    klawiatura[' '] = '5';

    if (fgets(t, sizeof(t), stdin)) {
        while ((c = t[i])) {
            u[i++] = klawiatura[(int) c];
        }
        puts(u);
    }

    return 0;
}

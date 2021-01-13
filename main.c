#include <stdint.h>
#include <stdio.h>

extern unsigned int int2half(int x, int decimalExp);
extern int half2int(unsigned half, int decimalExp);

int main() {
    int values[5][3] = {
        {6000, -4, 0x38cd},     // val: 0.6,        half: 0x38cd/0.60009765625.
        {138452, -5, 0x3d8a},   // val: 1.38452,    half: 0x3d8a/1.384765625.
        {6000, 0, 0x6ddc},      // val: 6000.0,     half: 0x6ddc/6000.0.
        {60000, 0, 0x7b53},     // val: 60000.0,    half: 0x7b53/60000.0.
        {6, -4, 0x10ea},        // val: 0.0006,     half: 0x10ea/0.0005998611450195312.
    };

    for (int idx = 0; idx < sizeof(values)/sizeof(*values); ++idx) {
        int value = values[idx][0];
        int decimalExp = values[idx][1];
        int should = values[idx][2];

        unsigned half = int2half(value, decimalExp);
        int toValue = half2int(half, decimalExp);

        printf("\nvalue:\t%6x/%8d/%d, \thalf: %04x(should: %04x), \ttoValue: %8d / %d.", value, value, decimalExp, half, should, toValue, toValue - value);

        // assert(value == toValue);
    }

    printf("\ncomplete\n");
}
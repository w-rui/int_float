#include <stdint.h>
#include <stdio.h>

extern unsigned int int2half(int x, int decimalExp);
extern int half2int(unsigned half, int decimalExp);


void doTest(char * test, int values[][3], int size) {
    for (int idx = 0; idx < size; ++idx) {
        int value = values[idx][0];
        int decimalExp = values[idx][1];
        int should = values[idx][2];

        int half = int2half(value, decimalExp);
        int toValue = half2int(half, decimalExp);

        if (should != half)
            printf("\n[FAIL] [%s] value:\t%6x/%8d/%d, \thalf: %04x(should: %04x), \ttoValue: %8d / %d.", test, value, value, decimalExp, half, should, toValue, toValue - value);
    }
}

void test_simple() {
    int values[][3] = {
        {1,     0,      0x3c00},
        {-1,    0,      0xbc00},
        {2,     0,      0x4000},
        {-2,    0,      0xc000},

        {99975,     -6,      0x2e66},
        {333251,    -6,      0x3555},

        {65504,     0,      0x7bff},
        {-65504,    0,      0xfbff},

        {6103,     -8,      0x4000},
        {-6103,     -8,      0x8400},

        {59604,     -13,      0x0001},
        {-59604,     -13,      0x8001},


        {0,     0,      0},
    };

    doTest("simple values", values, sizeof(values)/sizeof(*values));
}

void test_rounding() {
    int values[][3] = {
        { 2983, -11, 0x0001, },
        { 2980, -11, 0, }
    };
    
    doTest("rounding", values, sizeof(values)/sizeof(*values));
}

void test_samples() {
    int values[][3] = {
        {6000, -4, 0x38cd},     // val: 0.6,        half: 0x38cd/0.60009765625.
        {138452, -5, 0x3d8a},   // val: 1.38452,    half: 0x3d8a/1.384765625.
        {6000, 0, 0x6ddc},      // val: 6000.0,     half: 0x6ddc/6000.0.
        {60000, 0, 0x7b53},     // val: 60000.0,    half: 0x7b53/60000.0.
        {6, -4, 0x10ea},        // val: 0.0006,     half: 0x10ea/0.0005998611450195312.
    };
    
    doTest("sample", values, sizeof(values)/sizeof(*values));
}

int main() {
    test_simple();
    test_rounding();
    test_samples();
}
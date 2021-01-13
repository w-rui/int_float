
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define bits(size)      ((1 << (size)) - 1)

int max(int a, int b) { return a > b ? a : b; }

int manbitsOf(int tmp) {
    int manbits = 0;
    while (tmp)
    {
        tmp >>= 1;
        manbits++;
    }

    return manbits;
}

int decimalOfExp(int exp) {
    int by = 1;
    exp = abs(exp);

    while (exp-- > 0) {
        by *= 10;
    }

    return by;
}

int firstBitOffs(int val) {
    int offs = 0;
    int maxOffs = sizeof(val) * 8;
    while ((val & 0x1) == 0) {
        val >>= 1;
        offs++;

        if (offs > maxOffs)
            return -1;
    }

    return offs;
}

int int2float(int d, unsigned fracSize, unsigned expSize, int decimalExp) {
    if (d == 0)
        return 0;

    int sign = d < 0;
    unsigned int frac = abs(d);

    // zero exp.
    int zeroExp = (0x1 << (expSize - 1)) - 1;
    int exp = zeroExp + fracSize;

    if (decimalExp != 0) {
        int by = decimalOfExp(decimalExp);
        unsigned isMulti = decimalExp > 0;

        if (isMulti) {
            int adj = firstBitOffs(frac);
            if (adj > 0) {
                frac >>= adj;
                exp += adj;
            }

            frac *= by;
        } else {
            int adj = max(sizeof(d)*8, fracSize + expSize + 1) - manbitsOf(frac);
            if (adj > 0) {
                frac <<= adj;
                exp -= adj;
            }

            frac /= by;
        }
    }

    unsigned manbits = manbitsOf(frac);
    if (!manbits)
        return sign << (fracSize + expSize);

    int shift = manbits - (fracSize + 1);
    if (shift > 0) {
        frac >>= shift;
    } else if (shift < 0) {
        frac <<= -shift;
    }
    exp += shift;

    int infiExp = (0x1 << expSize) - 1;
    if (exp > infiExp)
    {
        // absx was too big, force it to +/- infinity
        exp = infiExp; // special infinity value
        frac = 0;
    } else if (exp < 0) {
        frac >>= -exp;
        exp = 0;
    }

    return (sign << (fracSize + expSize)) | (exp << fracSize) | (frac & bits(fracSize));
}

int int2halfTunc(int d, int decimalExp) {
    return int2float(d, 10, 5, decimalExp);
}

int float2int(unsigned int f, unsigned fracSize, unsigned expSize, int decimalExp) {
    unsigned int sign = (f >> (fracSize + expSize)) & 0x1;
    unsigned int exp = (f >> fracSize) & bits(expSize);
    unsigned int frac = f & bits(fracSize);

    if (exp == 0) {
        if (frac == 0)   // +/- zero
            return 0;
        // else denormal
    } else if (exp < (0x1 << expSize) - 1) {    // normal float
        frac |= 0x1 << fracSize;
    } else {
        if (frac == 0)   // +/- infinite
            return (sign ? -1 : 1) * bits(fracSize + expSize);
        else    // NaN
            return (sign ? -1 : 1) * bits(fracSize + expSize + 1);
    }

    if (decimalExp != 0) {
        int by = decimalOfExp(decimalExp);
        unsigned isMulti = decimalExp < 0;

        if (isMulti) {
            int adj = firstBitOffs(frac);
            if (adj > 0) {
                frac >>= adj;
                exp += adj;
            }

            frac *= by;
        } else {
            int adj = max(sizeof(frac)*8, fracSize + expSize + 1) - manbitsOf(frac);
            if (adj > 0) {
                frac <<= adj;
                exp -= adj;
            }

            frac /= by;
        }
    }

    int val = frac;
    int zeroExp = (0x1 << (expSize - 1)) - 1;
    int shift = (zeroExp - exp) + fracSize;
    if (shift > 0) {
        val /= 0x1 << shift;
    } else if (shift < 0)
        val *= 0x1 << -shift;

    if (sign) val = -val;

    return val;
}

int half2int(unsigned f, int decimalExp) {
    return float2int(f, 10, 5, decimalExp);
}

int float32int(unsigned f, int decimalExp) {
    return float2int(f, 23, 8, decimalExp);
}


// the real value:  x * 10^decimalExp
unsigned int int2half(int x, int decimalExp)
{
    unsigned truncHalf = int2halfTunc(x, decimalExp);

    if (truncHalf == 0xFFFF)
        return truncHalf;

    unsigned nextHalf = truncHalf + 1;
    if (nextHalf == 0xFFFF)
        return truncHalf;

    int trunc = half2int(truncHalf, decimalExp);
    int next = half2int(nextHalf, decimalExp);

    unsigned truncDelta = abs(trunc - x);
    unsigned nextDelta = abs(next - x);

    if (truncDelta != nextDelta)
        return truncDelta > nextDelta ? nextHalf : truncHalf;

    return truncHalf & 0x1 ? nextHalf : truncHalf;
}

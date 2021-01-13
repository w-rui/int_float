
# 整数 <=> 浮点数转换

文档: https://zh.wikipedia.org/wiki/IEEE_754

用于在不支持浮点运算的设备上（主要是单片机），进行整数（带小数）与浮点数的二进制转换。

## 简介

简单运行: `gcc -o build/half main.c float2int.c && build/half`

测试: `gcc -o build/test test.c float2int.c && build/test`

具体使用请参见 `test.c` 文件.

## half2int / float16 to int

`extern int half2int(unsigned half, int decimalExp);`

参数:

- half: float16 的二进制表示
- decimalExp: 运算结果的十进制指数
- return: 基于 decimalExp 指数的整数结果，即 `return = fromFloat16(half) / power(10, decimalExp)`

示例:

1. half = 0.6 的 float16 二进制表示, decimalExp = -4, 则运算结果 = 600
2. half = 100 的 float16 二进制表示, decimalExp = 2, 则运算结果 = 1

## int2half / int to float16

`extern unsigned int int2half(int x, int decimalExp);`

参数:

- x: 基于 10 进制指数 decimalExp 的整数值
- decimalExp: x 的 10 进制指数。
- return: float16 二进制表示结果，即 `return = toFloat16Binary(x * power(10, decimalExp))`

eg:

1. x = 6000, decimalExp = -3, 运算结果为 6.0 的 float16 二进制表示。
2. x = 1, decimal = 2, 运算结果为 0.01 的 float16 二进制表示




# int <=> float

document: https://zh.wikipedia.org/wiki/IEEE_754

Used for devices not support float/float16.

## introduction

simple run: `gcc -o build/half main.c float2int.c && build/half`

test: `gcc -o build/test test.c float2int.c && build/test`

More usage please see `test.c`.

## half2int / float16 to int

`extern int half2int(unsigned half, int decimalExp);`

args:

- half: binary of half.
- decimalExp: exp of the out decimal number, in 10 radix.
- return: the integer value of half base on decimalExp. `return = fromFloat16(half) / power(10, decimalExp)`

eg:

1. half = binary of 0.6, decimalExp = -4, result = 600
2. half = bianry of 100, decimalExp = 2, result = 1


## int2half / int to float16

`extern unsigned int int2half(int x, int decimalExp);`

args:

- x: the integer value base on decimalExp.
- decimalExp: exp of x, in 10 radix.
- return: the binary format of half. `return = toFloat16(x * power(10, decimalExp))`

eg:

1. x = 6000, decimalExp = -3, result = bianry of 6.0
2. x = 1, decimal = 2, result = binary of 0.01

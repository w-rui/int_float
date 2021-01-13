
# 整数 <=> 浮点数转换

文档: https://zh.wikipedia.org/wiki/IEEE_754

用于在不支持浮点运算的设备上（主要是单片机），进行整数（带小数）与浮点数的二进制转换。

## 简介

简单运行: `gcc -o build/half main.c float2int.c && build/half`

测试: `gcc -o build/test test.c float2int.c && build/test`

具体使用请参见 `test.c` 文件.

## 使用场景

当我们需要在不支持浮点数的设备上，通过网络传输浮点数的时候，比如，当一个电压的范围从 0.003V - 3000V 的时候，我们需要使用一个 float16 作为传输格式。这个时候，0.003 我们可以使用 `3 * power(10, -3)` 来表示，3000 可以使用 `3 * power(10, 3)` 来表示。

则:

- 0.003 的 float16 表示结果为：`half_0_003 = int2half(3, -3)`
- 3000 的 float16 表示结果为：`half_3000 = int2half(3000, 0)` 或 `half = int2half(3, 3)`

同理，当从网络收到一个 0.003 或者 3000 的 float16 二进制表示的时候，我们可以解析结果为：

- 以 0.001 为基数的 0.003 的整数表示结果为：`3 = half2int(half_0_003, -3)`
- 3000 的证书表示结果为：
  - 以 1 为基数：`3000 = half2int(half_3000, 0)`
  - 以 1000 为基数：`3 = half2int(half_3000, 3)`

## half2int / float16 转换 int

`extern int half2int(unsigned half, int decimalExp);`

参数:

- half: float16 的二进制表示
- decimalExp: 运算结果的十进制指数
- return: 基于 decimalExp 指数的整数结果，即 `return = fromFloat16(half) / power(10, decimalExp)`

示例:

1. half = 0.6 的 float16 二进制表示, decimalExp = -4, 则运算结果 = 600
2. half = 100 的 float16 二进制表示, decimalExp = 2, 则运算结果 = 1

## int2half / int 转换 float16

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

## Introduction

simple run: `gcc -o build/half main.c float2int.c && build/half`

test: `gcc -o build/test test.c float2int.c && build/test`

More usage please see `test.c`.

## Usage

When we need to transfer float data via network. For example, a Voltage scope is 0.003V - 3000V, and we need a float16 as representation format. So 0.003 could be result of `3 * power(10, -3)`, and 3000 could be result of `3 * power(10, 3)`。

So:

- the float16 bianry of 0.003: `half_0_003 = int2half(3, -3)`
- the float16 bianry of 3000: `half_3000 = int2half(3000, 0)` or `half = int2half(3, 3)`

When we received a float16 binary of 0.003 or 3000, we could parse them like:

- the integer of 0.003 base on `power(10, -3)`: `3 = half2int(half_0_003, -3)`
- the integer of 3000:
  - base on `power(10, 0)`: `3000 = half2int(half_3000, 0)`
  - base on `power(10, 3)`: `3 = half2int(half_3000, 3)`

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

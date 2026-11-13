# src/foundation/math/log_tables.c

This file holds the constants of the logarithm. It has no function.

## The constants

| Constant | Content |
| --- | --- |
| `LOG_LN2` | The logarithm of 2 as 2 numbers: a head whose low bits are 0, and a tail. |
| `LOG_FAR` | The 5 coefficients of the series far from 1. |
| `LOG_NEAR` | The 11 coefficients of the series near 1. |
| `LOG_INVERSE` | 128 rows: 1 divided by the center of the row, and the logarithm of the center. |
| `LOG_CENTER` | 128 rows: the center of the row as 2 numbers. |

## The origin

The values are those of `log_data.c` of musl 1.2.5, which Arm Limited wrote.
A program read them from that file and wrote them here, and no person typed them.
musl writes each value as a hexadecimal float. Here each value is the shortest decimal text that gives the same bits.
The style check reads the exponent of a hexadecimal float as an operator, so the decimal text is used.

## Functions

| Function | Result |
| --- | --- |
| None | The file holds only constants. |

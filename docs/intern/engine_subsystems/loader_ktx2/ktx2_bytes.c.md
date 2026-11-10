# src/engine_subsystems/loader_ktx2/ktx2_bytes.c

This file reads the numbers of a file, and compares a text with a word.

## The numbers

Each number of KTX 2.0 is little-endian. `ktx2_u32` builds a number of 32 bits from 4 bytes, and `ktx2_u64` builds a number of 64 bits from 2 numbers of 32 bits.
The caller makes sure that the bytes are in the file.

## Functions

| Function | Result |
| --- | --- |
| `ktx2_u32` | The number of 32 bits at an offset. |
| `ktx2_u64` | The number of 64 bits at an offset. |
| `ktx2_same` | `true` when the text has the bytes of the word, and no other byte. |

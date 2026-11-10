# src/engine_subsystems/loader_ktx2/ktx2_block.c

This file reads the size of a block and its bytes.

## The basic block

The data format descriptor must be in the file, and must hold at least `KTX2_DFD_MIN` bytes.
The word after `dfdTotalSize` holds the vendor and the type of the descriptor block. Both are 0 for the basic block of Khronos.
`texelBlockDimension0` to `texelBlockDimension2` are at the bytes 16 to 18, minus 1. `bytesPlane0` is at the byte 20.

A format of 1 byte for each texel has a block of 1 by 1 by 1 and 1 byte. BC7 has a block of 4 by 4 by 1 and 16 bytes.

A `bytesPlane0` of 0 tells a supercompressed file, which the module refuses.

## Functions

| Function | Result |
| --- | --- |
| `ktx2_read_block` | Writes the size of a block and its bytes, or gives the reason of a refusal. |

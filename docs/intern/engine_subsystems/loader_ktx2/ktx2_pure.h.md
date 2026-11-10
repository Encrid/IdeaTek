# src/engine_subsystems/loader_ktx2/ktx2_pure.h

This header holds the types and the prototypes of the pure part of the module. That part reads a file and the settings, with no call of Bridge.

## The types

| Type | Content |
| --- | --- |
| `ktx2_bytes_t` | A view of bytes: where they start and how many there are. |
| `ktx2_range_t` | The offset and the length of 1 level in the file. |
| `ktx2_file_t` | The fields of the header that the module reads, and where the data format descriptor is. Also the size of a block, the sum of the bytes of the levels, and the range of each level. |
| `ktx2_fault_t` | 1 fault of the settings: its kind, and the offset and the length of its key in the text. |
| `ktx2_sampler_read_t` | The sampler that the settings give, and their first `KTX2_MAX_FAULTS` faults. |
| `ktx2_pair_t` | The key and the value of 1 line of the settings. |

## The constants

| Constant | Value |
| --- | --- |
| `KTX2_HEAD` | 80: the bytes of the header and the index. |
| `KTX2_LEVEL` | 24: the bytes of 1 entry of the level index. |
| `KTX2_MAX_LEVELS` | 32: the most levels of a side of 32 bits. |
| `KTX2_DFD_MIN` | 28: the bytes of `dfdTotalSize` and of a basic block with no sample. |
| `KTX2_MAX_FAULTS` | 8: the most faults of the settings that the module writes. |
| `KTX2_UNKNOWN_KEY`, `KTX2_BAD_VALUE`, `KTX2_APPLIED` | The 3 results of 1 line of the settings. |

`ktx2_inside.h` includes this header, so the module and its pure part share the types.

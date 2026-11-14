# src/engine_subsystems/rendering_vulkan/render_memory.c

This file finds the types of memory of the GPU, and makes the read buffers of the captures.

`find_memory` gives the first type of memory that the needs of an object allow and that has each asked property.
It walks the types from the last to the first, and keeps the last match, so it skips each allowed type that lacks a property.
Vulkan gives each buffer a type that is visible to the host and coherent. It gives each image a type of the device. So a match is always there.

`make_reads` makes 1 read buffer for each slot, of the size of the half of `capture_bytes`. With a half of 0, it makes none.
The memory of a read buffer is visible to the host and coherent, and stays mapped. So the CPU reads the pixels of a capture with no other copy.
Each step of `make_read` stops at the first refusal of Vulkan, so no call runs on an object that does not exist.

## Functions

| Function | Result |
| --- | --- |
| `find_memory` | The index of a type of memory with the asked properties. |
| `make_read` | Makes, binds and maps 1 read buffer. `false` when Vulkan refuses. |
| `make_reads` | Makes the read buffer of each slot. `false` when Vulkan refuses. |

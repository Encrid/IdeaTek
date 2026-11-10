# src/engine_subsystems/loader_gltf/gltf_buffers.c

This file gives cgltf the bytes of each buffer before a build.

A buffer of the GLB container takes the binary chunk. A `data:` URI is decoded with `cgltf_load_buffer_base64`. Each other buffer takes the copy of its file.
A buffer shorter than its `byteLength` refuses the file.

## Functions

| Function | Result |
| --- | --- |
| `decode` | Decodes the base64 data of a URI, or writes its line. |
| `bytes_of` | The bytes of a buffer and their size. |
| `point` | Points the buffer at its bytes, or writes the line of a short buffer. |
| `wire_buffers` | Points each buffer of the file at its bytes. |

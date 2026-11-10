# src/engine_subsystems/loader_gltf/gltf_text.c

This file hashes the texts, and joins the folder of a file with the URI of a dependency.

A `%` and 2 hexadecimal digits in a URI give 1 byte. A `%` that 2 digits do not follow stays. A path stops at `PATH_ROOM` minus 1 bytes.

## Functions

| Function | Result |
| --- | --- |
| `hash_text` | A hash of a text by FNV-1a in 64 bits, from a seed or from the offset basis. |
| `hex_of` | The value of a hexadecimal digit, or -1. |
| `copy_uri` | Copies a URI after a folder, with its escapes decoded. |
| `join_path` | The path of a dependency. |

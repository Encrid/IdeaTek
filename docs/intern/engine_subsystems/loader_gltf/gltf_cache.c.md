# src/engine_subsystems/loader_gltf/gltf_cache.c

This file finds the copies of the buffer files, and frees the copies that no file uses.

## Functions

| Function | Result |
| --- | --- |
| `find_cache` | The copy of a resource, or nothing. `find_cache(0)` gives a free record. |
| `is_used` | `true` when a dependency names the resource. |
| `free_caches` | Frees each copy that no dependency names. |
| `wake_users` | Tries to build, or refuses, each file whose buffer is the resource. |

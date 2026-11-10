# src/engine_subsystems/loader_gltf/gltf_hear.c

The observers of the bytes of a file and of its output are in this file.

`bytes` starts the module, parses the file, plans its dependencies, and waits for them: `docs/intern/engine_subsystems/loader_gltf/gltf_wait.c.md`.
`swap` frees the old output block of a file after a reload is applied. `gone` forgets a file whose `ideatek:Model` leaves, then frees the copies of buffers that no file uses.

## Functions

| Function | Result |
| --- | --- |
| `read_file` | Parses 1 file, and plans and waits, or refuses it. |
| `hear_bytes` | Starts the module, or refuses the file of a broken module. |
| `bytes` | The observer of the bytes of the loader. |
| `swap` | The observer of a new model of a file. |
| `gone` | The observer of a model that leaves. |

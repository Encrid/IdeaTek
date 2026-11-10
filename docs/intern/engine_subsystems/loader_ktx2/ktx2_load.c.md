# src/engine_subsystems/loader_ktx2/ktx2_load.c

This file loads the bytes of 1 resource.

## The load

1. The first load starts the module. A broken module refuses each load.
2. `ktx2_read` reads the file. A reason writes its line, and the outcome fails.
3. `give_texture` copies the texels and attaches `ideatek:Texture`.
4. When the texture is there, `give_sampler` reads the settings and attaches `ideatek:Sampler`.
5. The module attaches `ideatek:ResourceOutcome`.

## Functions

| Function | Result |
| --- | --- |
| `refuse` | Writes the line, and attaches a failed outcome. |
| `is_ready` | Starts the module, and refuses the load of a broken module. |
| `load_one` | Loads 1 resource. |

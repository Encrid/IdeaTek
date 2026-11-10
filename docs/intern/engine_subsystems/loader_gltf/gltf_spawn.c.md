# src/engine_subsystems/loader_gltf/gltf_spawn.c

This file spawns the loads of the new dependencies, and destroys the uses of the dropped ones.

The texts of the new loads go into 1 block of the output arena, which frees the block of the last texts. The resources module copies the texts of a load in the round after it.

## Functions

| Function | Result |
| --- | --- |
| `drop_deps` | Destroys the use of each dependency that the file no longer names. |
| `total_of` | The bytes of the texts of the new loads. |
| `spawn_one` | Spawns the use entity of 1 load with `reload_owner` at 1. |
| `spawn_deps` | Takes the block of the texts, and spawns each new load. |

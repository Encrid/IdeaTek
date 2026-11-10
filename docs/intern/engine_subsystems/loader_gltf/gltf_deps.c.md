# src/engine_subsystems/loader_gltf/gltf_deps.c

This file plans the dependencies of a file at each parse.

A buffer file is a dependency, and an image that the KTX2 loader can read is a dependency too.
The key of a dependency is the hash of its kind and its path.
A dependency that a reload keeps keeps its use entity. A new dependency gets a record, and a dependency that the file no longer names is dropped.
1 allocation of the parse arena holds the room of the paths and of the settings of all the dependencies.

## Functions

| Function | Result |
| --- | --- |
| `match_dep` | The dependency of a file with a hash, or a free record. |
| `plan_one` | Keeps or adds 1 dependency. `false` when the table is full. |
| `find_folder` | The folder of the path of the file. |
| `plan_images` | Plans the images with the settings of their sampler. |
| `plan_deps` | Plans each dependency, drops the old ones, and spawns the new ones. |

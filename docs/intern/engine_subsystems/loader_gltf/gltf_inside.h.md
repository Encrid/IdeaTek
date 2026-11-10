# src/engine_subsystems/loader_gltf/gltf_inside.h

The header of the glTF loader holds its settings, slots, records, build context and state, with the prototypes of its sources.

## The records

| Record | Content |
| --- | --- |
| `gltf_file_t` | 1 file: its entity, the keys of its output now and before a reload, and the key of the texts of its loads. Also its revision, and whether it waits. |
| `gltf_dep_t` | 1 dependency: its file, its use entity and the resource of its handle. Also the hash of its kind and path, its kind and its index. |
| `gltf_cache_t` | 1 copy of the bytes of a buffer file: its resource, its key, where it is and its size. |
| `gltf_part_t` | 1 sub-resource: its file, its entity, the hash of its label, its revision and its kind. |
| `gltf_plan_t` | The plan of 1 mesh: its streams with their offsets, and its counts of vertices, indices, targets and sub-meshes. |
| `gltf_build_t` | The context of 1 build: the parsed data, the writer, and the arrays of the spans, the labels, the parts, the plans and the ranks. |

A key of 0 names no block. A record whose entity or file is 0 is free.

## The state

`gltf_module` keeps the tables from 1 call to the next. `.toolchain/project.toml` excepts it from G4.
The slots of the types follow the order of `docs/intern/engine_subsystems/loader_gltf/gltf_declare.c.md`.
`NAME_ROOM` is the room of a name or a label. `PATH_ROOM` is the room of a path: a folder of 4096 bytes and its URI.

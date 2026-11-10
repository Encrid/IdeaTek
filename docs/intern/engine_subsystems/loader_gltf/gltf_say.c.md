# src/engine_subsystems/loader_gltf/gltf_say.c

The lines of the glTF loader that start with the path of a file come from this file, with the outcome.

`path_of` reads the path of `ideatek:Resource`. The resources module attaches it before the bytes of a file, so its empty path is a guard that `.toolchain/project.toml` excepts.

## Functions

| Function | Result |
| --- | --- |
| `path_of` | The path of a resource and its length. |
| `say_path` | Writes 1 line: the path, then the rest. |
| `say_index` | Writes 1 line with an index in its format. |
| `refuse_id` | Says the line, then attaches a failed outcome. |
| `give_outcome` | Attaches `ideatek:ResourceOutcome`. |

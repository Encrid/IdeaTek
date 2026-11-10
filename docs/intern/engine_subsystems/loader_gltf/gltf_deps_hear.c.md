# src/engine_subsystems/loader_gltf/gltf_deps_hear.c

The observers of the handles and of the failures of the dependencies are in this file.

When the handle of a buffer comes, the module looks at the state of its resource. A loaded resource with no copy gets `ideatek:ResourceReload`, because `ideatek:RawBytes` comes only at the end of a load. A failed resource refuses the file.
`raw` copies the bytes of a buffer file: `docs/intern/engine_subsystems/loader_gltf/gltf_keep.c.md`. `failed` refuses each file whose buffer failed.

## Functions

| Function | Result |
| --- | --- |
| `take_handle` | Keeps the resource of a handle, and asks a reload or refuses as the state tells. |
| `handles` | The observer of the handles. |
| `raw` | The observer of the raw bytes. |
| `refuse_users` | Refuses each file whose buffer is the failed resource. |
| `failed` | The observer of the failures. |

# include/kernel/ideatek_resources.h

`include/kernel/ideatek_resources.h` gives the types of the resources part: the loads, the handles, the loaders, the sources and the states of the resources.
`readme.md`, *The contracts*, tells the rules of each contract.
The fields name entities with `ideatek_entity_t` of `include/platform_abstraction/ideatek_world.h`. The resources module reads the files with the files part, `include/platform_abstraction/ideatek_files.h`.

## The roles

- The game loads resources with `ideatek:ResourceLoad`, and reads their handles and their states. The game is a module.
- A loader module tells its loader with `ideatek:ResourceLoader`. It reads `ideatek:ResourceBytes` and attaches its own component of the resource, `ideatek:ResourceOutcome` and `ideatek:ResourceLabel`.
- A source module serves the files of a source: *The source modules*.
- The resources module gives the handles, reads the files, picks the loaders and writes the states. A deployment has one resources module.

The component of a resource is a type of the loader module, for example `game:Image`. IdeaTek gives no format of a file.
The files of the mode `processed` come from a tool. No tool is part of IdeaTek.
A source on the web, a source in an archive and a source in a program are source modules.

## The handles

A use entity is an entity with `ideatek:ResourceLoad` or `ideatek:ResourceAdd`. Its `ideatek:ResourceHandle` is a strong handle: it keeps the resource.
The entity of the resource is a weak handle: an id that keeps nothing.
The count of a resource is the number of use entities whose handle names the resource or one of its sub-resources.
When the count becomes 0, the resources module destroys the resource, its sub-resources and each use entity whose `owner` is the resource.
A loader module hears the free with an observer of `BRIDGE_ON_EXIT` of its component.

When the `owner` of a use entity is destroyed, the resources module destroys the use entity. An `owner` of 0 names no entity.

When the count becomes 0 during a load, the resources module also destroys the entity of each read of the resource.
The files part or the source module then cancels the read.

## The loads

A module spawns a use entity and attaches `ideatek:ResourceLoad`. In a later round, the resources module attaches `ideatek:ResourceHandle`.
Two loads with the same `path` and the same `type` get the same resource.
The `settings` of the first load stay. The resources module ignores the `settings` of a later load of the same resource.

1. The resources module reads the meta file: *The meta files*.
2. It picks the loader: *The loaders*.
3. It reads the file, and copies the bytes: *The bytes of the loads*.
4. It attaches `ideatek:ResourceBytes` to the resource.
5. The loader attaches its component and `ideatek:ResourceOutcome` to the resource.
6. The resources module writes `state`, and detaches `ideatek:ResourceBytes` and `ideatek:ResourceOutcome`.

Each step takes at least 1 round. A result of the files part gives an error of the resource:

| `ideatek_file_status_t` | `ideatek_resource_error_t` |
| --- | --- |
| `IDEATEK_FILE_STATUS_NOT_FOUND` | `IDEATEK_RESOURCE_ERROR_NOT_FOUND` |
| `IDEATEK_FILE_STATUS_TOO_BIG` | `IDEATEK_RESOURCE_ERROR_TOO_BIG` |
| Another status that is not `IDEATEK_FILE_STATUS_OK` | `IDEATEK_RESOURCE_ERROR_READ_FAILED` |

## The priority

The priority of a resource is the highest `priority` of its loads that wait.
The resources module gives that priority to each read of the resource. It starts the waiting reads with the highest priority first.
A streamed world loads the near resources with a high priority, and the far resources with a low priority.

## The resources of a module

A module can make a resource in its code, for example a mesh that it computes.
It spawns the entity of the resource and attaches its own component to it. Then it spawns a use entity and attaches `ideatek:ResourceAdd`.
The resources module attaches `ideatek:Resource` to the entity, with an empty path and the state `IDEATEK_LOAD_STATE_LOADED`.
The count and the free of the resource follow *The handles*. A resource of a module has no reload.

## The paths

A path of a resource is `source://path#label`. The part `source://` and the part `#label` are optional in the text.
The part `path` follows the path rule of the files part.
A path with no source names the default source. The default source is the content storage, or `processed_folder` in the mode `processed`.

- A named source is an item `name=folder` of `sources`. The folder is a folder of the content storage.
- The source `user` is the folder of `IDEATEK_STORAGE_USER`. The resources module only reads it. Mods and the maps of the players are there.
- A source module gives each other name: *The source modules*.

A name of a source holds only `a` to `z`, `0` to `9` and `_`. A path with an unknown source gets `IDEATEK_RESOURCE_ERROR_NOT_FOUND`.
When 2 sources have the same name, the resources module keeps the first, and the log gets 1 line.
A path can hold `{name}` of an `ideatek:ResourceVariable`. The resources module puts `value` in its place, or `fallback` when that file is missing. An empty `fallback` means none.

A path with a name that no variable has gets `IDEATEK_RESOURCE_ERROR_NOT_FOUND`. When 2 variables have the same name, the resources module keeps the first, and the log gets 1 line.
When a variable changes, the resources module reloads each resource whose path holds its name.

## The source modules

A source module serves the files of 1 source, for example the files in an archive of another game.
At `init`, it spawns 1 entity with `ideatek:ResourceSource`.
For each read, the resources module spawns an entity with `ideatek:ResourceSourceRead`.
The source module attaches `ideatek:ResourceSourceResult` to the same entity, in a later round.
The bytes of the result stay good until the resources module destroys the entity.

A source module has no watch. A module that knows of a change attaches `ideatek:ResourceReload` to the resource.

## The loaders

At `init`, a loader module spawns 1 entity for each of its loaders, with `ideatek:ResourceLoader`.
A line of `extensions` gives 1 extension with no dot, and ends with `\n`. The extension of a path is the text after the last dot of its last name.

- When the meta file names a loader, the resources module takes the loader with that `name`.
- Else, when 1 loader has the `type` of the load, the resources module takes it.
- Else, it takes the loader with the extension of the path, among the loaders of the `type`. An empty `type` allows each loader.
- When 2 loaders have the extension, it reads the file first. Then it takes the loader whose `magic` is the start of the bytes.

No loader gets `IDEATEK_RESOURCE_ERROR_NO_LOADER`. A name, or an extension with no loader of the right `magic`, that 2 loaders hold gets `IDEATEK_RESOURCE_ERROR_TWO_LOADERS`.
A loader of the meta file with another `type` than the load gets `IDEATEK_RESOURCE_ERROR_WRONG_TYPE`.
The formats of 2 games can share an extension, as `.bsp` or `.mdl`, so `magic` tells them apart.

## The meta files

The meta file of a path is the file `path.meta` in the same source, in TOML.
With `meta_check` at `always`, the resources module reads the meta file of each load. With `never`, it reads none.
With `paths`, it reads the meta files of the paths in `meta_paths` only. A meta file that is not found gives the defaults.

With `always`, each load makes 1 more read. `never` or `paths` removes that read where a missing file costs a request.

| Key | Default | Value |
| --- | --- | --- |
| `action` | `load` | `load`, `process` or `ignore`. |
| `loader` | No loader | The `name` of a loader. |
| `settings` | An empty table | A table that the resources module gives to the loader as text. |

The resources module loads a path with `process` as a path with `load`. The tool reads `process`.
A path with `ignore` gets `IDEATEK_RESOURCE_ERROR_IGNORED`.
A load with an empty `settings` takes the `settings` of the meta file.

## The bytes of the loads

At `init`, the resources module takes `load_bytes` bytes of its persistent arena. It copies the bytes of each read into them.
The bytes of `ideatek:ResourceBytes` stay good until the resources module detaches it.
A read waits for a later round while its bytes do not fit in the free bytes.
A file with more bytes than `load_bytes` gets `IDEATEK_RESOURCE_ERROR_TOO_BIG`, unless its loader is `ranged`.
The `persistent` of its manifest includes `load_bytes`.

## The ranges

A loader with `ranged` at `true` reads a file in parts, for example the mips of a texture or a long music.
Its first `ideatek:ResourceBytes` holds the first `head_bytes` bytes of the file, and `size` tells the size of the whole file.
To read another range, the loader attaches `ideatek:ResourceRead` to the resource. The resources module reads it, and attaches `ideatek:ResourceBytes` again.
A loader can read a range also after the state is `IDEATEK_LOAD_STATE_LOADED`. The state does not change.

A range past the end of the file gives `IDEATEK_RESOURCE_ERROR_READ_FAILED` in `error`.

## The raw bytes

The resources module has 1 loader of its own, with the name `raw` and the type `ideatek:RawBytes`. It has no extension.
A loader loads a side file with the type `ideatek:RawBytes`, as a dependency. Examples are the `.bin` of a `.gltf`, or the include of a shader.
The resources module attaches `ideatek:RawBytes` for 1 round when the load ends, and for 1 round after each reload. The owner copies the bytes in that round.

## The dependencies

A loader loads a dependency with a use entity whose `owner` is the resource. Thus the dependency lives while the resource lives.
`dependencies` is `IDEATEK_LOAD_STATE_LOADED` when each dependency is `IDEATEK_LOAD_STATE_LOADED`. It is `IDEATEK_LOAD_STATE_FAILED` when 1 dependency failed.
`recursive` tells the same of the resource and of the dependencies at each depth.
`error` keeps the first error.

With `reload_owner` at 1, a reload of the dependency reloads the owner too.

## The labels

A sub-resource is a part of a file, for example a mesh of a scene. The loader spawns 1 entity for each sub-resource.
The entity gets `ideatek:ResourceLabel` and the component of the loader. The resources module then attaches `ideatek:Resource` with the path `path#label`.
A load of `path#label` loads `path`. When `path` ends, the resources module attaches the handle of the label.

A label that the file does not hold gets an entity with `IDEATEK_LOAD_STATE_FAILED` and `IDEATEK_RESOURCE_ERROR_NO_LABEL`.
When `path` fails, the handle names `path`.

## The reloads

With `hot_reload` at `true`, the resources module watches the default source and the named sources with `IDEATEK_FILE_ACTION_WATCH`.
It does not watch `user`, because the files part watches only the content storage.
A change of a file or of its meta file reloads the resource, `reload_delay_ms` after the last change.
The wait joins the events of 1 save into 1 reload, and it lets the writer end the file before the read.
`ideatek:ResourceReload` reloads the resource at once.

A reload keeps the entity. `state` becomes `IDEATEK_LOAD_STATE_LOADING`, and the loader attaches its component again.
The loader puts the new values on the labels that stay, and attaches `ideatek:ResourceLabel` to each of them again. It spawns an entity for each new label. A label that is gone gets `IDEATEK_RESOURCE_ERROR_NO_LABEL`.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `hot_reload` | `false` | `true` or `false` | The resources module watches the sources and reloads the changed resources. |
| `reload_delay_ms` | 300 | 0 to 60000 | The milliseconds after the last change of a file, before its reload. |
| `meta_check` | `always` | `always`, `never` or `paths` | The meta files that the resources module reads. |
| `meta_paths` | `""` | A text of 0 to 4095 bytes: paths that the path rule allows, with a comma between 2 paths. | With `paths`: the paths whose meta file the resources module reads. |
| `mode` | `unprocessed` | `unprocessed` or `processed` | The files of the default source. |
| `processed_folder` | `processed` | A path that the path rule allows. | The default source in the mode `processed`. |
| `sources` | `""` | A text of 0 to 4095 bytes: items `name=folder`, with a comma between 2 items. | The named sources. |
| `load_bytes` | 4194304 | 1 to 1073741824 | The bytes for the bytes of the loads. |
| `head_bytes` | 65536 | 1 to `load_bytes` | The bytes of the first read of a file of a ranged loader. |

## ideatek:ideatek_load_state_t

`ideatek_load_state_t` tells the state of a load.
`ideatek_load_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LOAD_STATE_NOT_LOADED` | No load started. |
| 1 | `IDEATEK_LOAD_STATE_LOADING` | The load runs. |
| 2 | `IDEATEK_LOAD_STATE_LOADED` | The load is done. |
| 3 | `IDEATEK_LOAD_STATE_FAILED` | The load failed: `error` tells why. |

## ideatek:ideatek_resource_error_t

`ideatek_resource_error_t` tells why a load failed.
`ideatek_resource_error_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_RESOURCE_ERROR_NONE` | No error. |
| 1 | `IDEATEK_RESOURCE_ERROR_NO_LOADER` | No loader fits the load. |
| 2 | `IDEATEK_RESOURCE_ERROR_TWO_LOADERS` | 2 loaders fit the load. |
| 3 | `IDEATEK_RESOURCE_ERROR_NOT_FOUND` | The path names no file. |
| 4 | `IDEATEK_RESOURCE_ERROR_READ_FAILED` | The read of the file failed. |
| 5 | `IDEATEK_RESOURCE_ERROR_LOADER_FAILED` | The loader did not read the bytes. |
| 6 | `IDEATEK_RESOURCE_ERROR_NO_LABEL` | The file holds no such label. |
| 7 | `IDEATEK_RESOURCE_ERROR_WRONG_TYPE` | The loader gives another type. |
| 8 | `IDEATEK_RESOURCE_ERROR_IGNORED` | The meta file tells `ignore`. |
| 9 | `IDEATEK_RESOURCE_ERROR_TOO_BIG` | The file has more bytes than `load_bytes`. |
| 10 | `IDEATEK_RESOURCE_ERROR_NO_ROOM` | A table of the resources module is full. Its readme names the bound. |

## ideatek:ResourceLoad

`ideatek:ResourceLoad` asks for a resource. A module attaches it to a new use entity.
`ideatek_resource_load_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `owner` | `ideatek_entity_t` | 0 | 0 | The entity that holds the use, or 0. |
| `path` | `bridge_text_t` | 8 | No default | The path of the resource. |
| `type` | `bridge_text_t` | 16 | No default | The name of the component of the resource, or an empty text. |
| `settings` | `bridge_text_t` | 24 | No default | The settings of the loader in TOML, or an empty text. |
| `reload_owner` | `uint8_t` | 32 | 0 | 1: a reload of this resource reloads `owner`. 0: it does not. |
| `priority` | `uint8_t` | 33 | 0 | The priority of the load: *The priority*. 255 is the highest. |

Bytes 34 to 39 hold no field.

## ideatek:ResourceAdd

`ideatek:ResourceAdd` makes a resource of a module: *The resources of a module*. A module attaches it to a new use entity.
`ideatek_resource_add_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `resource` | `ideatek_entity_t` | 0 | The entity of the resource that the module made. |
| `type` | `bridge_text_t` | 8 | The name of the component of the resource. |

## ideatek:ResourceHandle

`ideatek:ResourceHandle` names the resource of a use entity. The resources module attaches it.
`ideatek_resource_handle_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `resource` | `ideatek_entity_t` | 0 | The entity of the resource. |

## ideatek:Resource

`ideatek:Resource` gives the path and the states of a resource. The resources module attaches it to the entity of the resource.
`ideatek_resource_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `path` | `bridge_text_t` | 0 | The path. Empty for a resource of a module. |
| `type` | `bridge_text_t` | 8 | The `type` of the load. |
| `state` | `ideatek_load_state_t` | 16 | The state of the resource itself. |
| `dependencies` | `ideatek_load_state_t` | 17 | The state of its dependencies. |
| `recursive` | `ideatek_load_state_t` | 18 | The state of the resource and of its dependencies at each depth. |
| `error` | `ideatek_resource_error_t` | 19 | The first error. |

## ideatek:ResourceLoader

`ideatek:ResourceLoader` tells a loader. A loader module attaches it at `init`.
`ideatek_resource_loader_t` takes 36 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | The name of the loader. |
| `type` | `bridge_text_t` | 8 | The name of the component that the loader gives. |
| `extensions` | `bridge_text_t` | 16 | The lines of the extensions: *The loaders*. |
| `magic` | `bridge_text_t` | 24 | The first bytes of each file of the format, or no bytes. |
| `ranged` | `bool` | 32 | `true` if the loader reads a file in parts: *The ranges*. |

Bytes 33 to 35 hold no field.

## ideatek:ResourceBytes

`ideatek:ResourceBytes` gives the bytes of a file to its loader. The resources module attaches it to the resource.
`ideatek_resource_bytes_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `loader` | `ideatek_entity_t` | 0 | The entity of the loader. |
| `offset` | `uint64_t` | 8 | The place of the first byte of `data` in the file. |
| `size` | `uint64_t` | 16 | The size of the whole file. |
| `data` | `bridge_text_t` | 24 | The bytes of the file, or of the range. |
| `settings` | `bridge_text_t` | 32 | The settings of the loader in TOML. |
| `error` | `ideatek_resource_error_t` | 40 | With a range: `IDEATEK_RESOURCE_ERROR_NONE` or the error of the read. |

Bytes 41 to 47 hold no field.

## ideatek:ResourceRead

`ideatek:ResourceRead` asks for a range of the file: *The ranges*. A ranged loader attaches it to the resource.
`ideatek_resource_read_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `offset` | `uint64_t` | 0 | The first byte to read. |
| `length` | `uint64_t` | 8 | The number of bytes to read, at most `load_bytes`. |
| `priority` | `uint8_t` | 16 | The priority of the read: *The priority*. |

Bytes 17 to 23 hold no field.

## ideatek:ResourceOutcome

`ideatek:ResourceOutcome` gives the end of a read of the loader. The loader module attaches it to the resource.
`ideatek_resource_outcome_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `error` | `ideatek_resource_error_t` | 0 | `IDEATEK_RESOURCE_ERROR_NONE`, or `IDEATEK_RESOURCE_ERROR_LOADER_FAILED`. |

## ideatek:ResourceLabel

`ideatek:ResourceLabel` names the file and the label of a sub-resource. The loader module attaches it.
`ideatek_resource_label_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `resource` | `ideatek_entity_t` | 0 | The entity of the resource of the file. |
| `label` | `bridge_text_t` | 8 | The label. |

## ideatek:RawBytes

`ideatek:RawBytes` gives the bytes of a side file: *The raw bytes*. The resources module attaches it.
`ideatek_raw_bytes_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | The bytes of the file. |

## ideatek:ResourceSource

`ideatek:ResourceSource` tells a source of a source module: *The source modules*.
`ideatek_resource_source_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | The name of the source. |

## ideatek:ResourceSourceRead

`ideatek:ResourceSourceRead` asks a source module for bytes. The resources module attaches it.
`ideatek_resource_source_read_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `source` | `ideatek_entity_t` | 0 | The entity of the source. |
| `offset` | `uint64_t` | 8 | The first byte to read. |
| `length` | `uint64_t` | 16 | The number of bytes to read. With 0, the source module reads until the file ends. |
| `path` | `bridge_text_t` | 24 | The path in the source, with no `source://` and no `#label`. |
| `priority` | `uint8_t` | 32 | The priority of the read: *The priority*. |

Bytes 33 to 39 hold no field.

## ideatek:ResourceSourceResult

`ideatek:ResourceSourceResult` gives the bytes of a read. The source module attaches it.
`ideatek_resource_source_result_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `size` | `uint64_t` | 0 | The size of the whole file. |
| `data` | `bridge_text_t` | 8 | The bytes that were read. |
| `error` | `ideatek_resource_error_t` | 16 | `IDEATEK_RESOURCE_ERROR_NONE`, `IDEATEK_RESOURCE_ERROR_NOT_FOUND` or `IDEATEK_RESOURCE_ERROR_READ_FAILED`. |

Bytes 17 to 23 hold no field.

## ideatek:ResourceReload

`ideatek:ResourceReload` is a tag. A module attaches it to a resource for a reload. The resources module detaches it.

## ideatek:ResourceModified

`ideatek:ResourceModified` is a tag. The resources module attaches it for 1 round, after a reload is done.

## ideatek:ResourceReady

`ideatek:ResourceReady` is a tag. The resources module attaches it for 1 round, when `recursive` becomes `IDEATEK_LOAD_STATE_LOADED`.

## ideatek:ResourceFailed

`ideatek:ResourceFailed` is a tag. The resources module attaches it for 1 round, when a state becomes `IDEATEK_LOAD_STATE_FAILED`.

## ideatek:ResourceVariable

`ideatek:ResourceVariable` gives the value of a name in the paths: *The paths*.
`ideatek_resource_variable_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | The name of the variable in a path, for example `locale`. |
| `value` | `bridge_text_t` | 8 | The text that takes the place of the name. |
| `fallback` | `bridge_text_t` | 16 | The text when no file has `value`. Empty means none. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_load_state_t` | `ideatek_load_state_t` | `ideatek_load_state_t_desc` | `IDEATEK_LOAD_STATE_VALUES` |
| `ideatek:ideatek_resource_error_t` | `ideatek_resource_error_t` | `ideatek_resource_error_t_desc` | `IDEATEK_RESOURCE_ERROR_VALUES` |
| `ideatek:ResourceLoad` | `ideatek_resource_load_t` | `ideatek_resource_load_t_desc` | `IDEATEK_RESOURCE_LOAD_FIELDS` |
| `ideatek:ResourceAdd` | `ideatek_resource_add_t` | `ideatek_resource_add_t_desc` | `IDEATEK_RESOURCE_ADD_FIELDS` |
| `ideatek:ResourceHandle` | `ideatek_resource_handle_t` | `ideatek_resource_handle_t_desc` | `IDEATEK_RESOURCE_HANDLE_FIELDS` |
| `ideatek:Resource` | `ideatek_resource_t` | `ideatek_resource_t_desc` | `IDEATEK_RESOURCE_FIELDS` |
| `ideatek:ResourceLoader` | `ideatek_resource_loader_t` | `ideatek_resource_loader_t_desc` | `IDEATEK_RESOURCE_LOADER_FIELDS` |
| `ideatek:ResourceBytes` | `ideatek_resource_bytes_t` | `ideatek_resource_bytes_t_desc` | `IDEATEK_RESOURCE_BYTES_FIELDS` |
| `ideatek:ResourceRead` | `ideatek_resource_read_t` | `ideatek_resource_read_t_desc` | `IDEATEK_RESOURCE_READ_FIELDS` |
| `ideatek:ResourceOutcome` | `ideatek_resource_outcome_t` | `ideatek_resource_outcome_t_desc` | `IDEATEK_RESOURCE_OUTCOME_FIELDS` |
| `ideatek:ResourceLabel` | `ideatek_resource_label_t` | `ideatek_resource_label_t_desc` | `IDEATEK_RESOURCE_LABEL_FIELDS` |
| `ideatek:RawBytes` | `ideatek_raw_bytes_t` | `ideatek_raw_bytes_t_desc` | `IDEATEK_RAW_BYTES_FIELDS` |
| `ideatek:ResourceSource` | `ideatek_resource_source_t` | `ideatek_resource_source_t_desc` | `IDEATEK_RESOURCE_SOURCE_FIELDS` |
| `ideatek:ResourceSourceRead` | `ideatek_resource_source_read_t` | `ideatek_resource_source_read_t_desc` | `IDEATEK_RESOURCE_SOURCE_READ_FIELDS` |
| `ideatek:ResourceSourceResult` | `ideatek_resource_source_result_t` | `ideatek_resource_source_result_t_desc` | `IDEATEK_RESOURCE_SOURCE_RESULT_FIELDS` |
| `ideatek:ResourceVariable` | `ideatek_resource_variable_t` | `ideatek_resource_variable_t_desc` | `IDEATEK_RESOURCE_VARIABLE_FIELDS` |

```text
{ IDEATEK_LOAD_STATE_NOT_LOADED = 0, IDEATEK_LOAD_STATE_LOADING = 1, IDEATEK_LOAD_STATE_LOADED = 2, IDEATEK_LOAD_STATE_FAILED = 3 }
{ IDEATEK_RESOURCE_ERROR_NONE = 0, IDEATEK_RESOURCE_ERROR_NO_LOADER = 1, IDEATEK_RESOURCE_ERROR_TWO_LOADERS = 2, IDEATEK_RESOURCE_ERROR_NOT_FOUND = 3, IDEATEK_RESOURCE_ERROR_READ_FAILED = 4, IDEATEK_RESOURCE_ERROR_LOADER_FAILED = 5, IDEATEK_RESOURCE_ERROR_NO_LABEL = 6, IDEATEK_RESOURCE_ERROR_WRONG_TYPE = 7, IDEATEK_RESOURCE_ERROR_IGNORED = 8, IDEATEK_RESOURCE_ERROR_TOO_BIG = 9, IDEATEK_RESOURCE_ERROR_NO_ROOM = 10 }
{ ideatek_entity_t owner; bridge_text_t path; bridge_text_t type; bridge_text_t settings; uint8_t reload_owner; uint8_t priority; }
{ ideatek_entity_t resource; bridge_text_t type; }
{ ideatek_entity_t resource; }
{ bridge_text_t path; bridge_text_t type; ideatek_load_state_t state; ideatek_load_state_t dependencies; ideatek_load_state_t recursive; ideatek_resource_error_t error; }
{ bridge_text_t name; bridge_text_t type; bridge_text_t extensions; bridge_text_t magic; bool ranged; }
{ ideatek_entity_t loader; uint64_t offset; uint64_t size; bridge_text_t data; bridge_text_t settings; ideatek_resource_error_t error; }
{ uint64_t offset; uint64_t length; uint8_t priority; }
{ ideatek_resource_error_t error; }
{ ideatek_entity_t resource; bridge_text_t label; }
{ bridge_text_t data; }
{ bridge_text_t name; }
{ ideatek_entity_t source; uint64_t offset; uint64_t length; bridge_text_t path; uint8_t priority; }
{ uint64_t size; bridge_text_t data; ideatek_resource_error_t error; }
{ bridge_text_t name; bridge_text_t value; bridge_text_t fallback; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_load_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_resource_error_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceLoad` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceAdd` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceHandle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Resource` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceLoader` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceBytes` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceRead` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceOutcome` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceLabel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:RawBytes` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceSource` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceSourceRead` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceSourceResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceReload` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceModified` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceReady` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceFailed` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceVariable` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:ResourceAdd`, `ideatek:ResourceReload`, `ideatek:ResourceLoad`, `ideatek:ResourceVariable` | `ideatek:ResourceHandle`, `ideatek:Resource`, `ideatek:ResourceLabel`, `ideatek:ResourceModified`, `ideatek:ResourceReady`, `ideatek:ResourceFailed`, and the types of the fields |
| A loader module | `ideatek:ResourceLoad`, `ideatek:ResourceAdd`, `ideatek:ResourceRead`, `ideatek:ResourceOutcome`, `ideatek:ResourceLabel`, `ideatek:ResourceLoader` | `ideatek:ResourceHandle`, `ideatek:Resource`, `ideatek:ResourceBytes`, `ideatek:RawBytes`, and the types of the fields |
| A source module | `ideatek:ResourceSourceResult`, `ideatek:ResourceSource` | `ideatek:ResourceSourceRead`, and the types of the fields |
| The resources module | `ideatek:FileRequest`, `ideatek:ResourceHandle`, `ideatek:Resource`, `ideatek:ResourceBytes`, `ideatek:ResourceOutcome`, `ideatek:ResourceRead`, `ideatek:RawBytes`, `ideatek:ResourceSourceRead`, `ideatek:ResourceReload`, `ideatek:ResourceModified`, `ideatek:ResourceReady`, `ideatek:ResourceFailed` | `ideatek:ResourceVariable`, `ideatek:ResourceLoad`, `ideatek:ResourceAdd`, `ideatek:ResourceLoader`, `ideatek:ResourceLabel`, `ideatek:ResourceSource`, `ideatek:ResourceSourceResult`, `ideatek:FileResult`, `ideatek:FileChange`, and the types of the fields |

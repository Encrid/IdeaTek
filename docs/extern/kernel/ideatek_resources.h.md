# include/kernel/ideatek_resources.h

`include/kernel/ideatek_resources.h` gives the types of the resources part: the loads, the handles, the loaders and the states of the resources.
`readme.md`, *The contracts*, tells the rules of each contract.
The fields name entities with `ideatek_entity_t` of `include/platform_abstraction/ideatek_world.h`. The resources module reads the files with the files part, `include/platform_abstraction/ideatek_files.h`.

## The roles

- The game loads resources with `ideatek:ResourceLoad`, and reads their handles and their states. The game is a module.
- A loader module tells its loader with `ideatek:ResourceLoader`. It reads `ideatek:ResourceBytes` and attaches its own component of the resource, `ideatek:ResourceOutcome` and `ideatek:ResourceLabel`.
- The resources module gives the handles, reads the files, picks the loaders and writes the states. A deployment has one resources module.

The component of a resource is a type of the loader module, for example `game:Image`. IdeaTek gives no format of a file.
The files of the mode `processed` come from a tool. No tool is part of IdeaTek.

Out: a source on the web, and a source that is embedded in a program.
Comes back: a source on the web, when a target has no content storage.
Comes back: an embedded source, when a module needs resources in its own library.

## The handles

A use entity is an entity with `ideatek:ResourceLoad`. Its `ideatek:ResourceHandle` is a strong handle: it keeps the resource.
The entity of the resource is a weak handle: an id that keeps nothing.
The count of a resource is the number of use entities whose handle names the resource or one of its sub-resources.
When the count becomes 0, the resources module destroys the resource, its sub-resources and each use entity whose `owner` is the resource.
A loader module hears the free with an observer of `BRIDGE_ON_EXIT` of its component.

When the `owner` of a use entity is destroyed, the resources module destroys the use entity. An `owner` of 0 names no entity.

## The loads

A module spawns a use entity and attaches `ideatek:ResourceLoad`. In a later round, the resources module attaches `ideatek:ResourceHandle`.
Two loads with the same `path` and the same `type` get the same resource.
The `settings` of the first load stay. The resources module ignores the `settings` of a later load of the same resource.

1. The resources module reads the meta file: *The meta files*.
2. It picks the loader: *The loaders*.
3. It reads the file with `ideatek:FileRequest` on `IDEATEK_STORAGE_CONTENT`, and copies the bytes: *The bytes of the loads*.
4. It attaches `ideatek:ResourceBytes` to the resource.
5. The loader attaches its component and `ideatek:ResourceOutcome` to the resource.
6. The resources module writes `state`, and detaches `ideatek:ResourceBytes` and `ideatek:ResourceOutcome`.

Each step takes at least 1 round. A result of the files part gives an error of the resource:

| `ideatek_file_status_t` | `ideatek_resource_error_t` |
| --- | --- |
| `IDEATEK_FILE_STATUS_NOT_FOUND` | `IDEATEK_RESOURCE_ERROR_NOT_FOUND` |
| `IDEATEK_FILE_STATUS_TOO_BIG` | `IDEATEK_RESOURCE_ERROR_TOO_BIG` |
| Another status that is not `IDEATEK_FILE_STATUS_OK` | `IDEATEK_RESOURCE_ERROR_READ_FAILED` |

## The paths

A path of a resource is `source://path#label`. The part `source://` and the part `#label` are optional in the text.
The part `path` follows the path rule of the files part.

A path with no source names the default source. The default source is the content storage, or `processed_folder` in the mode `processed`.
A named source is a line `name=folder` of `sources`. The folder is a folder of the content storage.
A name of a source holds only `a` to `z`, `0` to `9` and `_`. A path with an unknown source gets `IDEATEK_RESOURCE_ERROR_NOT_FOUND`.

## The loaders

At `init`, a loader module spawns 1 entity for each of its loaders, with `ideatek:ResourceLoader`.
A line of `extensions` gives 1 extension with no dot, and ends with `\n`. The extension of a path is the text after the last dot of its last name.

- When the meta file names a loader, the resources module takes the loader with that `name`.
- Else, when 1 loader has the `type` of the load, the resources module takes it.
- Else, it takes the loader with the extension of the path, among the loaders of the `type`. An empty `type` allows each loader.

No loader gets `IDEATEK_RESOURCE_ERROR_NO_LOADER`. A name or an extension that 2 loaders hold gets `IDEATEK_RESOURCE_ERROR_TWO_LOADERS`.
A loader of the meta file with another `type` than the load gets `IDEATEK_RESOURCE_ERROR_WRONG_TYPE`.

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
A file with more bytes than `load_bytes` gets `IDEATEK_RESOURCE_ERROR_TOO_BIG`.
The `persistent` of its manifest includes `load_bytes`.

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

With `hot_reload` at `true`, the resources module watches each source with `IDEATEK_FILE_ACTION_WATCH`.
A change of a file or of its meta file reloads the resource, `reload_delay_ms` after the last change.
The wait joins the events of 1 save into 1 reload, and it lets the writer end the file before the read.
`ideatek:ResourceReload` reloads the resource at once.

A reload keeps the entity. `state` becomes `IDEATEK_LOAD_STATE_LOADING`, and the loader attaches its component again.
The loader puts the new values on the labels that stay, and spawns an entity for each new label. A label that is gone gets `IDEATEK_RESOURCE_ERROR_NO_LABEL`.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `hot_reload` | `false` | `true` or `false` | The resources module watches the sources and reloads the changed resources. |
| `reload_delay_ms` | 300 | 0 to 60000 | The milliseconds after the last change of a file, before its reload. |
| `meta_check` | `always` | `always`, `never` or `paths` | The meta files that the resources module reads. |
| `meta_paths` | `[]` | Paths that the path rule allows. | With `paths`: the paths whose meta file the resources module reads. |
| `mode` | `unprocessed` | `unprocessed` or `processed` | The files of the default source. |
| `processed_folder` | `processed` | A path that the path rule allows. | The default source in the mode `processed`. |
| `sources` | `[]` | Lines `name=folder` | The named sources. |
| `load_bytes` | 4194304 | 1 to 1073741824 | The bytes for the bytes of the loads. |

## ideatek:ResourceLoad

`ideatek:ResourceLoad` asks for a resource. A module attaches it to a new use entity.
`ideatek_resource_load_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `owner` | `ideatek_entity_t` | 0 | The entity that holds the use, or 0. |
| `path` | `bridge_text_t` | 8 | The path of the resource. |
| `type` | `bridge_text_t` | 16 | The name of the component of the resource, or an empty text. |
| `settings` | `bridge_text_t` | 24 | The settings of the loader in TOML, or an empty text. |
| `reload_owner` | `uint8_t` | 32 | 1: a reload of this resource reloads `owner`. 0: it does not. |

Bytes 33 to 39 hold no field.

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
| `path` | `bridge_text_t` | 0 | The path. |
| `type` | `bridge_text_t` | 8 | The `type` of the load. |
| `state` | `ideatek_load_state_t` | 16 | The state of the resource itself. |
| `dependencies` | `ideatek_load_state_t` | 17 | The state of its dependencies. |
| `recursive` | `ideatek_load_state_t` | 18 | The state of the resource and of its dependencies at each depth. |
| `error` | `ideatek_resource_error_t` | 19 | The first error. |

### ideatek:ideatek_load_state_t

`ideatek_load_state_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The state |
| --- | --- | --- |
| `IDEATEK_LOAD_STATE_NOT_LOADED` | 0 | No load started. |
| `IDEATEK_LOAD_STATE_LOADING` | 1 | The load runs. |
| `IDEATEK_LOAD_STATE_LOADED` | 2 | The load is done. |
| `IDEATEK_LOAD_STATE_FAILED` | 3 | The load failed: `error` tells why. |

### ideatek:ideatek_resource_error_t

`ideatek_resource_error_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The error |
| --- | --- | --- |
| `IDEATEK_RESOURCE_ERROR_NONE` | 0 | No error. |
| `IDEATEK_RESOURCE_ERROR_NO_LOADER` | 1 | No loader fits the load. |
| `IDEATEK_RESOURCE_ERROR_TWO_LOADERS` | 2 | 2 loaders fit the load. |
| `IDEATEK_RESOURCE_ERROR_NOT_FOUND` | 3 | The path names no file. |
| `IDEATEK_RESOURCE_ERROR_READ_FAILED` | 4 | The files module did not read the file. |
| `IDEATEK_RESOURCE_ERROR_LOADER_FAILED` | 5 | The loader did not read the bytes. |
| `IDEATEK_RESOURCE_ERROR_NO_LABEL` | 6 | The file holds no such label. |
| `IDEATEK_RESOURCE_ERROR_WRONG_TYPE` | 7 | The loader gives another type. |
| `IDEATEK_RESOURCE_ERROR_IGNORED` | 8 | The meta file tells `ignore`. |
| `IDEATEK_RESOURCE_ERROR_TOO_BIG` | 9 | The file has more bytes than `load_bytes`. |

## ideatek:ResourceLoader

`ideatek:ResourceLoader` tells a loader. A loader module attaches it to an entity at `init`.
`ideatek_resource_loader_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | The name of the loader. |
| `type` | `bridge_text_t` | 8 | The name of the component that the loader gives. |
| `extensions` | `bridge_text_t` | 16 | The lines of the extensions: *The loaders*. |

## ideatek:ResourceBytes

`ideatek:ResourceBytes` gives the bytes of a file to its loader. The resources module attaches it to the resource.
`ideatek_resource_bytes_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `loader` | `ideatek_entity_t` | 0 | The entity of the loader. |
| `data` | `bridge_text_t` | 8 | The bytes of the file. |
| `settings` | `bridge_text_t` | 16 | The settings of the loader in TOML. |

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

## The tags

- `ideatek:ResourceReload` is a tag. A module attaches it to a resource for a reload. The resources module detaches it.
- `ideatek:ResourceModified` is a tag. The resources module attaches it for 1 round, after a reload is done.
- `ideatek:ResourceReady` is a tag. The resources module attaches it for 1 round, when `recursive` becomes `IDEATEK_LOAD_STATE_LOADED`.
- `ideatek:ResourceFailed` is a tag. The resources module attaches it for 1 round, when a state becomes `IDEATEK_LOAD_STATE_FAILED`.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_load_state_t` | `ideatek_load_state_t` | `ideatek_load_state_t_desc` | `IDEATEK_LOAD_STATE_VALUES` |
| `ideatek:ideatek_resource_error_t` | `ideatek_resource_error_t` | `ideatek_resource_error_t_desc` | `IDEATEK_RESOURCE_ERROR_VALUES` |
| `ideatek:ResourceLoad` | `ideatek_resource_load_t` | `ideatek_resource_load_t_desc` | `IDEATEK_RESOURCE_LOAD_FIELDS` |
| `ideatek:ResourceHandle` | `ideatek_resource_handle_t` | `ideatek_resource_handle_t_desc` | `IDEATEK_RESOURCE_HANDLE_FIELDS` |
| `ideatek:Resource` | `ideatek_resource_t` | `ideatek_resource_t_desc` | `IDEATEK_RESOURCE_FIELDS` |
| `ideatek:ResourceLoader` | `ideatek_resource_loader_t` | `ideatek_resource_loader_t_desc` | `IDEATEK_RESOURCE_LOADER_FIELDS` |
| `ideatek:ResourceBytes` | `ideatek_resource_bytes_t` | `ideatek_resource_bytes_t_desc` | `IDEATEK_RESOURCE_BYTES_FIELDS` |
| `ideatek:ResourceOutcome` | `ideatek_resource_outcome_t` | `ideatek_resource_outcome_t_desc` | `IDEATEK_RESOURCE_OUTCOME_FIELDS` |
| `ideatek:ResourceLabel` | `ideatek_resource_label_t` | `ideatek_resource_label_t_desc` | `IDEATEK_RESOURCE_LABEL_FIELDS` |

```text
{ IDEATEK_LOAD_STATE_NOT_LOADED = 0, IDEATEK_LOAD_STATE_LOADING = 1, IDEATEK_LOAD_STATE_LOADED = 2, IDEATEK_LOAD_STATE_FAILED = 3 }
{ IDEATEK_RESOURCE_ERROR_NONE = 0, IDEATEK_RESOURCE_ERROR_NO_LOADER = 1, IDEATEK_RESOURCE_ERROR_TWO_LOADERS = 2, IDEATEK_RESOURCE_ERROR_NOT_FOUND = 3, IDEATEK_RESOURCE_ERROR_READ_FAILED = 4, IDEATEK_RESOURCE_ERROR_LOADER_FAILED = 5, IDEATEK_RESOURCE_ERROR_NO_LABEL = 6, IDEATEK_RESOURCE_ERROR_WRONG_TYPE = 7, IDEATEK_RESOURCE_ERROR_IGNORED = 8, IDEATEK_RESOURCE_ERROR_TOO_BIG = 9 }
{ ideatek_entity_t owner; bridge_text_t path; bridge_text_t type; bridge_text_t settings; uint8_t reload_owner; }
{ ideatek_entity_t resource; }
{ bridge_text_t path; bridge_text_t type; ideatek_load_state_t state; ideatek_load_state_t dependencies; ideatek_load_state_t recursive; ideatek_resource_error_t error; }
{ bridge_text_t name; bridge_text_t type; bridge_text_t extensions; }
{ ideatek_entity_t loader; bridge_text_t data; bridge_text_t settings; }
{ ideatek_resource_error_t error; }
{ ideatek_entity_t resource; bridge_text_t label; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_load_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_resource_error_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceLoad` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceHandle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Resource` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceLoader` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceBytes` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceOutcome` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceLabel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceReload` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceModified` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceReady` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ResourceFailed` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares the 3 types of the fields before the 7 structures.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:ResourceLoad` and `ideatek:ResourceReload` | The 3 types of the fields, `ideatek:ResourceHandle`, `ideatek:Resource`, `ideatek:ResourceLabel` and the 3 other tags |
| A loader module | `ideatek:ResourceLoad`, `ideatek:ResourceLoader`, `ideatek:ResourceOutcome` and `ideatek:ResourceLabel` | The 3 types of the fields, `ideatek:ResourceHandle`, `ideatek:Resource` and `ideatek:ResourceBytes` |
| The resources module | `ideatek:ResourceHandle`, `ideatek:Resource`, `ideatek:ResourceBytes`, `ideatek:ResourceOutcome` and 4 tags | The 3 types of the fields, `ideatek:ResourceLoad`, `ideatek:ResourceLoader` and `ideatek:ResourceLabel` |

The resources module also has the grants of the game in the files part.

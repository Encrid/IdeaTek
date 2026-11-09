# src/kernel/resources_files/resources_inside.h

This header holds the records of the resources module, its state, the slots of its types and the prototypes that its sources share.

## The records

Each table of the module is an array of records in the persistent arena, sized by a bound of the settings at the start.

| Record | Table | Content |
| --- | --- | --- |
| `res_resource_t` | `resources`, `max_resources` | 1 resource: its entity, its source, its loader, its parent for a sub-resource, its count of uses, its step, its read and its block. Also the size of its file, its view `ideatek:Resource`, the time of its next reload, and the tags of the round. |
| `res_use_t` | `uses`, `max_uses` | 1 use entity: its entity, its `owner`, its resource, its `priority`, `reload_owner`, and whether it is an addition and whether it has its handle. |
| `res_loader_t` | `loaders`, `max_loaders` | 1 loader: its entity and `ranged`. The texts stay in the component of the loader module. |
| `res_source_t` | `sources`, `max_sources` | 1 source: its name, its folder (for a named source), its entity (for a source module), its watch entity and the status of the watch. |
| `res_variable_t` | `variables`, `max_variables` | 1 variable: its entity. The texts stay in the component. |
| `res_read_t` | `reads`, `max_reads` | 1 read that runs: its request entity, its resource, its kind (meta, file, range), its offset and length, and its result when it came. |

The texts of a resource are in 1 slot of `path_bytes` + `settings_bytes` bytes, next to the records. The slot holds the texts in this order: the full path, the type, the settings, with a zero after the path and after the type.
`res_load_t` is a load as `place_load` reads it: its texts and its split path. `res_arrival_t` is the result of a read as the round reads it: the bytes, the size and the error.
3 hash maps of `docs/intern/kernel/resources_files/resources_hash.c.md` find the records. The keys are the hash of the path and the type of a resource, the entity of a resource, and the entity of a use.

## The steps of a resource

| Step | Meaning |
| --- | --- |
| `STEP_NEW` | The load starts, or starts again for a reload. |
| `STEP_META` | The read of the meta file runs. |
| `STEP_PICK` | The loader is picked, or the pick waits for the bytes. |
| `STEP_READ` | The read of the file waits for a free read. |
| `STEP_READING` | The read of the file runs. |
| `STEP_LOADER` | `ideatek:ResourceBytes` is attached, and the loader works. |
| `STEP_DONE` | The load ended: `state` is `IDEATEK_LOAD_STATE_LOADED` or `IDEATEK_LOAD_STATE_FAILED`. |

## The state

`resources_module` is the 1 global variable: Bridge gives no argument that carries the module between 2 calls, and G4 of the style excepts it.

| Field | Value |
| --- | --- |
| `started`, `broken` | The first round read the settings. A wrong setting breaks the module. |
| `settings` | The 18 settings, as `res_settings_t`: the counts and the choices, and the texts `meta_paths`, `processed_folder` and `sources` as `bridge_text_t`. |
| `resources`, `uses`, `loaders`, `sources`, `variables`, `reads` | The 6 tables, and their counts. |
| `texts` | Where the text slots of the resources begin. |
| `by_key`, `by_entity`, `use_by_entity` | The 3 hash maps. |
| `blocks` | The block list of the `load_bytes` arena: `docs/intern/kernel/resources_files/resources_arena.c.md`. |
| `views` | The views of the loaders for the pick, 1 for each record. |
| `deps`, `recs` | The folds of each resource, for the passes of the fold. |
| `default_watch` | The watch entity of the default source, or 0. |
| `paths_at` | Where the path slots of the reads begin: `RES_PATH` bytes for each read. |
| `now` | The seconds since the first round, from `real_delta` of `ideatek:Frame`. |
| `no_clock` | The deployment has no `ideatek:Frame`, and the line of the log was written. |
| `dirty` | A round changed a state or a use: the fold of the states runs. |
| `round` | The count of the rounds. |
| `lists` | The indexes of the lists of the observers. |

## The names

The slots follow the declarations: `ENTITY`, the 3 enums of the files part, `FILE_REQUEST`, `FILE_RESULT`, `FILE_CHANGE` and `FRAME`. Then come the 21 types of the resources contract in the order of its document, then `SETTINGS`.

`RES_NONE` is the index of nothing, `UINT32_MAX`. `RES_DEFAULT` and `RES_USER` are the 2 sources that are no record. `RES_PASSES` is 8, the most passes of the fold.
`RESOURCES_TYPE` and `RESOURCES_TAG` declare a type of a contract. `RESOURCES_SINGLE` declares a singleton.

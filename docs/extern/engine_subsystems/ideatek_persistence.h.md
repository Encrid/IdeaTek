# include/engine_subsystems/ideatek_persistence.h

`include/engine_subsystems/ideatek_persistence.h` gives the types of the saves: the requests, the answers of each module, the slots, the versions and the settings of the user.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the world part, for the entities of its fields.

## The roles

- The game spawns a request and marks the entities that the save holds.
- Each module with an `ideatek:ChangeTag` of the changes part saves its rows. It writes a FULL for a save, and reads `ideatek:RowsIn` at a load.
- The save module writes the file through the files part of layer 2, and reads it back. When a write fails, the files part keeps the old file.

## The rows of a save

A module saves its rows with the stream of the changes part, keyed by `ideatek:EntityId`. A field that names an entity holds the `ideatek:EntityId` of that entity.
So a save, the network and a store read the same bytes of a module.

## The save

The id module gives each `ideatek:Saved` entity of the groups of the request an `ideatek:EntityId`, if it has none.
Each module of rows writes 1 `ideatek:RowChanges` FULL with `request` at the request. When each tag has written, the save module writes the file.

## The load

The game clears the old world first. The save module reads the file and spawns each prefab with its `ideatek:EntityId`.
It checks each checksum first. A checksum that does not match fails the load with CORRUPT, and no row is written.
Then it writes 1 `ideatek:RowsIn` FULL for each tag, with `source` at the request and the version of the chunk. Each module checks its rows, and answers `ideatek:SaveReady` or `ideatek:SaveRefuse`.

Only when each module is ready does each module apply its rows. So a refused load leaves the world as it was.
A tag with no chunk in the file gets a `ideatek:RowsIn` of version 0 with no data, and its module starts from its defaults. A chunk with no tag is skipped, and the save module logs it once.

## The versions

Each chunk has the `version` of the `ideatek:ChangeTag` of its module. Its module upgrades an older version, as the changes part tells, and answers a newer version with `ideatek:SaveRefuse`.
The save module reads each older version of the format of the file, and refuses a newer one.

## The file

A save is a file in the folder `saves/` of `IDEATEK_STORAGE_USER`. A name of a slot follows the rule of the paths of the files part. All numbers are little-endian.

1. The magic `IDTS`, then the version of the format, a `uint32_t` of 1.
2. The head: the time as an `int64_t` of Unix seconds, and the groups as a `uint64_t`.
   Then the compression as a `uint32_t`: 0 none, 1 LZ4, 2 zstd.
   Then the length of the info as a `uint32_t`, and the info.
   Last, the CRC-32C of the head as a `uint32_t`.
3. The prefabs: a `uint32_t` count, then records of `{uint64_t id, uint64_t prefab}`. `id` is the `ideatek:EntityId`.
4. The chunks: a `uint32_t` count, then records of `{uint64_t tag, uint32_t version, uint32_t length, uint32_t crc, bytes}`.
   The bytes are the records of the FULL of the tag, compressed apart for each chunk. `length` is the length as stored, and `crc` is their CRC-32C.

INFO and LIST read only the head. The CRC-32C is the checksum of Castagnoli, as the commit log of SpacetimeDB uses.

## The live store

A store module of the changes part keeps the world durable between the rounds, as the changes part tells. A world of a server then needs no save to survive a crash.
A save file stays a snapshot that a player names, for example a slot of a solo game. Both hold the same rows, so a world can move from a save to a store and back.

## The settings of the user

The settings of a player are rows of the game. Examples are the bindings, the volumes of the buses, the scale of the UI and the options of the graphics.
The game marks them with a bit of the groups of its own, and saves them in their own slot.
A load of a game uses another bit, so it never touches the settings. A menu of the saves hides a slot by its groups.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `compression` | `none` | `none`, `lz4` or `zstd` | The compression of the files that the save module writes. It reads each of the 3. |

## ideatek:ideatek_save_action_t

`ideatek_save_action_t` tells what a request does.
`ideatek_save_action_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SAVE_ACTION_SAVE` | Write the slot. |
| 1 | `IDEATEK_SAVE_ACTION_LOAD` | Read the slot into the world. |
| 2 | `IDEATEK_SAVE_ACTION_INFO` | Read the head of the slot. |
| 3 | `IDEATEK_SAVE_ACTION_LIST` | Read the head of each slot. |
| 4 | `IDEATEK_SAVE_ACTION_REMOVE` | Delete the slot. |

## ideatek:SaveRequest

`ideatek:SaveRequest` asks the save module to act on a slot.
`ideatek_save_request_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | 1 | The groups of the entities and the rows. |
| `slot` | `bridge_text_t` | 8 | No default | The name of the slot. |
| `info` | `bridge_text_t` | 16 | No default | The bytes of the game for the head, for example the level. |
| `action` | `ideatek_save_action_t` | 24 | No default | What it does. |

Bytes 25 to 31 hold no field.

## ideatek:Saved

`ideatek:Saved` makes a save hold an entity.
`ideatek_saved_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `prefab` | `uint64_t` | 0 | No default | The prefab, by the hash of its name. |
| `groups` | `uint64_t` | 8 | 1 | The groups of the entity. |

## ideatek:SaveReady

`ideatek:SaveReady` tells that a participant can load its chunk.
`ideatek_save_ready_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `request` | `ideatek_entity_t` | 0 | The request. |
| `tag` | `uint64_t` | 8 | The tag of the participant. |

## ideatek:SaveRefuse

`ideatek:SaveRefuse` tells that a participant cannot load its chunk.
`ideatek_save_refuse_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `request` | `ideatek_entity_t` | 0 | The request. |
| `tag` | `uint64_t` | 8 | The tag of the participant. |
| `reason` | `int32_t` | 16 | A reason that the participant defines. |

Bytes 20 to 23 hold no field.

## ideatek:ideatek_save_state_t

`ideatek_save_state_t` tells the state of a request.
`ideatek_save_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SAVE_STATE_PENDING` | The request goes on. |
| 1 | `IDEATEK_SAVE_STATE_DONE` | The request is done. |
| 2 | `IDEATEK_SAVE_STATE_FAILED` | The request failed: `error` tells why. |

## ideatek:ideatek_save_error_t

`ideatek_save_error_t` tells why a request failed.
`ideatek_save_error_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SAVE_ERROR_NONE` | No error. |
| 1 | `IDEATEK_SAVE_ERROR_NOT_FOUND` | No such slot. |
| 2 | `IDEATEK_SAVE_ERROR_READ` | The read failed. |
| 3 | `IDEATEK_SAVE_ERROR_WRITE` | The write failed. |
| 4 | `IDEATEK_SAVE_ERROR_NO_SPACE` | The storage is full. |
| 5 | `IDEATEK_SAVE_ERROR_FORMAT` | The file is not a save, or its format is newer. |
| 6 | `IDEATEK_SAVE_ERROR_VERSION` | A module refused its rows. |
| 7 | `IDEATEK_SAVE_ERROR_CORRUPT` | A checksum of the file does not match. |

## ideatek:SaveResult

`ideatek:SaveResult` is the answer to a request. The save module writes it on the request.
`ideatek_save_result_t` takes 2 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `state` | `ideatek_save_state_t` | 0 | The state. |
| `error` | `ideatek_save_error_t` | 1 | The error. |

## ideatek:SaveInfo

`ideatek:SaveInfo` is the head of 1 save, for INFO and LIST. The save module writes 1 entity for each save.
`ideatek_save_info_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `request` | `ideatek_entity_t` | 0 | The request. |
| `time` | `int64_t` | 8 | The time of the save, in Unix seconds. |
| `bytes` | `uint64_t` | 16 | The size of the file. |
| `groups` | `uint64_t` | 24 | The groups of the save. |
| `slot` | `bridge_text_t` | 32 | The name of the slot. |
| `info` | `bridge_text_t` | 40 | The bytes of the game in the head. |
| `version` | `uint32_t` | 48 | The version of the format. |

Bytes 52 to 55 hold no field.

The save module destroys these entities 1 round after `ideatek:SaveResult`.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_save_action_t` | `ideatek_save_action_t` | `ideatek_save_action_t_desc` | `IDEATEK_SAVE_ACTION_VALUES` |
| `ideatek:SaveRequest` | `ideatek_save_request_t` | `ideatek_save_request_t_desc` | `IDEATEK_SAVE_REQUEST_FIELDS` |
| `ideatek:Saved` | `ideatek_saved_t` | `ideatek_saved_t_desc` | `IDEATEK_SAVED_FIELDS` |
| `ideatek:SaveReady` | `ideatek_save_ready_t` | `ideatek_save_ready_t_desc` | `IDEATEK_SAVE_READY_FIELDS` |
| `ideatek:SaveRefuse` | `ideatek_save_refuse_t` | `ideatek_save_refuse_t_desc` | `IDEATEK_SAVE_REFUSE_FIELDS` |
| `ideatek:ideatek_save_state_t` | `ideatek_save_state_t` | `ideatek_save_state_t_desc` | `IDEATEK_SAVE_STATE_VALUES` |
| `ideatek:ideatek_save_error_t` | `ideatek_save_error_t` | `ideatek_save_error_t_desc` | `IDEATEK_SAVE_ERROR_VALUES` |
| `ideatek:SaveResult` | `ideatek_save_result_t` | `ideatek_save_result_t_desc` | `IDEATEK_SAVE_RESULT_FIELDS` |
| `ideatek:SaveInfo` | `ideatek_save_info_t` | `ideatek_save_info_t_desc` | `IDEATEK_SAVE_INFO_FIELDS` |

```text
{ IDEATEK_SAVE_ACTION_SAVE = 0, IDEATEK_SAVE_ACTION_LOAD = 1, IDEATEK_SAVE_ACTION_INFO = 2, IDEATEK_SAVE_ACTION_LIST = 3, IDEATEK_SAVE_ACTION_REMOVE = 4 }
{ uint64_t groups; bridge_text_t slot; bridge_text_t info; ideatek_save_action_t action; }
{ uint64_t prefab; uint64_t groups; }
{ ideatek_entity_t request; uint64_t tag; }
{ ideatek_entity_t request; uint64_t tag; int32_t reason; }
{ IDEATEK_SAVE_STATE_PENDING = 0, IDEATEK_SAVE_STATE_DONE = 1, IDEATEK_SAVE_STATE_FAILED = 2 }
{ IDEATEK_SAVE_ERROR_NONE = 0, IDEATEK_SAVE_ERROR_NOT_FOUND = 1, IDEATEK_SAVE_ERROR_READ = 2, IDEATEK_SAVE_ERROR_WRITE = 3, IDEATEK_SAVE_ERROR_NO_SPACE = 4, IDEATEK_SAVE_ERROR_FORMAT = 5, IDEATEK_SAVE_ERROR_VERSION = 6, IDEATEK_SAVE_ERROR_CORRUPT = 7 }
{ ideatek_save_state_t state; ideatek_save_error_t error; }
{ ideatek_entity_t request; int64_t time; uint64_t bytes; uint64_t groups; bridge_text_t slot; bridge_text_t info; uint32_t version; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_save_action_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_save_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_save_error_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Saved` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveReady` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveRefuse` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveInfo` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:SaveRequest`, `ideatek:Saved` | The types of the fields |
| A module of rows | `ideatek:SaveReady`, `ideatek:SaveRefuse` | The requests, the rows of a load, the ids, and the field types |
| The save module | `ideatek:RowsIn`, `ideatek:SaveResult`, `ideatek:SaveInfo` | `ideatek:SaveRequest`, `ideatek:Saved`, `ideatek:SaveReady`, `ideatek:SaveRefuse`, `ideatek:ChangeTag`, `ideatek:RowChanges`, `ideatek:EntityId`, and the types of the fields |

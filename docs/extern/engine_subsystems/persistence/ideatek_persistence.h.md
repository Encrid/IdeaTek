# include/engine_subsystems/persistence/ideatek_persistence.h

`include/engine_subsystems/persistence/ideatek_persistence.h` gives the types of the saves: the requests, the chunks of each module, the slots, the versions and the settings of the user.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the world part, for the entities of its fields.

## The roles

- The game spawns a request and marks the entities that the save holds.
- Each module that saves rows is a participant. It writes its own chunk and reads it back.
- The save module writes the file through the files part of layer 2, and reads it back. When a write fails, the files part keeps the old file.

## The participants

At `init`, each module that saves spawns 1 `ideatek:SaveParticipant` with its tag. The tag is the hash of the name of its component.
A participant serializes its rows field by field, keyed by `ideatek:SaveId`. It writes each reference to an entity as a save id.
A module can use the same serialization for its replication.

## The save

The save module gives each `ideatek:Saved` entity of the groups of the request an `ideatek:SaveId`.
Each participant writes 1 `ideatek:SaveChunk` for the request. When each participant has written its chunk, the save module writes the file.

## The load

The game clears the old world first. The save module reads the file and spawns each prefab with its `ideatek:SaveId`.
It writes 1 `ideatek:SaveChunk` for each tag, and each participant checks its chunk. It answers `ideatek:SaveReady` or `ideatek:SaveRefuse`.
Only when each participant is ready does each participant write its rows. So a refused load leaves the world as it was.

A participant with no chunk in the file gets a chunk of version 0 with no data, and starts from its defaults. A chunk with no participant is skipped, and the save module logs it once.

## The versions

Each chunk has the version of its participant. The participant reads an older version and upgrades it in its own code. A newer version gets `ideatek:SaveRefuse`.
The save module reads each older version of the format of the file, and refuses a newer one.

## The file

A save is a file in the folder `saves/` of `IDEATEK_STORAGE_USER`. A name of a slot follows the rule of the paths of the files part. All numbers are little-endian.

1. The magic `IDTS`, then the version of the format, a `uint32_t` of 1.
2. The head: the time as an `int64_t` of Unix seconds, and the groups as a `uint64_t`. Then the length of the info as a `uint32_t`, and the info.
3. The prefabs: a `uint32_t` count, then records of `{uint64_t id, uint64_t prefab}`.
4. The chunks: a `uint32_t` count, then records of `{uint64_t tag, uint32_t version, uint32_t length, bytes}`.

INFO and LIST read only the head.

## The settings of the user

The settings of a player are rows of the game. Examples are the bindings, the volumes of the buses, the scale of the UI and the options of the graphics.
The game marks them with a bit of the groups of its own, and saves them in their own slot.
A load of a game uses another bit, so it never touches the settings. A menu of the saves hides a slot by its groups.

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

## ideatek:SaveId

`ideatek:SaveId` gives an entity its id in a save. The save module writes it.
`ideatek_save_id_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The id of the entity in the save. |

## ideatek:SaveParticipant

`ideatek:SaveParticipant` makes a module save its rows: *The participants*.
`ideatek_save_participant_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `tag` | `uint64_t` | 0 | The tag of the module: the hash of its component name. |

## ideatek:SaveChunk

`ideatek:SaveChunk` holds the rows of 1 participant for a request.
`ideatek_save_chunk_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `request` | `ideatek_entity_t` | 0 | The request. |
| `tag` | `uint64_t` | 8 | The tag of the participant. |
| `data` | `bridge_text_t` | 16 | The rows, serialized by the participant. |
| `version` | `uint32_t` | 24 | The version of the chunk. |

Bytes 28 to 31 hold no field.

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
| 6 | `IDEATEK_SAVE_ERROR_VERSION` | A participant refused its chunk. |

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
| `ideatek:SaveId` | `ideatek_save_id_t` | `ideatek_save_id_t_desc` | `IDEATEK_SAVE_ID_FIELDS` |
| `ideatek:SaveParticipant` | `ideatek_save_participant_t` | `ideatek_save_participant_t_desc` | `IDEATEK_SAVE_PARTICIPANT_FIELDS` |
| `ideatek:SaveChunk` | `ideatek_save_chunk_t` | `ideatek_save_chunk_t_desc` | `IDEATEK_SAVE_CHUNK_FIELDS` |
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
{ uint64_t id; }
{ uint64_t tag; }
{ ideatek_entity_t request; uint64_t tag; bridge_text_t data; uint32_t version; }
{ ideatek_entity_t request; uint64_t tag; }
{ ideatek_entity_t request; uint64_t tag; int32_t reason; }
{ IDEATEK_SAVE_STATE_PENDING = 0, IDEATEK_SAVE_STATE_DONE = 1, IDEATEK_SAVE_STATE_FAILED = 2 }
{ IDEATEK_SAVE_ERROR_NONE = 0, IDEATEK_SAVE_ERROR_NOT_FOUND = 1, IDEATEK_SAVE_ERROR_READ = 2, IDEATEK_SAVE_ERROR_WRITE = 3, IDEATEK_SAVE_ERROR_NO_SPACE = 4, IDEATEK_SAVE_ERROR_FORMAT = 5, IDEATEK_SAVE_ERROR_VERSION = 6 }
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
| `ideatek:SaveId` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveParticipant` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveChunk` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveReady` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveRefuse` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SaveInfo` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:SaveRequest`, `ideatek:Saved` | The types of the fields |
| A participant | `ideatek:SaveParticipant`, `ideatek:SaveChunk`, `ideatek:SaveReady`, `ideatek:SaveRefuse` | `ideatek:SaveRequest`, `ideatek:SaveId`, and the types of the fields |
| The save module | `ideatek:SaveId`, `ideatek:SaveChunk`, `ideatek:SaveResult`, `ideatek:SaveInfo` | `ideatek:SaveRequest`, `ideatek:Saved`, `ideatek:SaveParticipant`, `ideatek:SaveReady`, `ideatek:SaveRefuse`, and the types of the fields |

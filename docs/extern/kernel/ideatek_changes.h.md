# include/kernel/ideatek_changes.h

`include/kernel/ideatek_changes.h` gives the types of the changes of the rows: ids, the stream, the rows in, the durability, the store and the recordings.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the world part, because the requests and the sources are entities.

## The roles

- Each module that keeps rows of its own writes their changes once. The net module, the save module, a store module and a recorder all read the same changes.
- The id module gives each entity that needs it an `ideatek:EntityId`. A deployment has exactly 1 id module.
- A consumer writes `ideatek:RowsIn` for the rows that come in. The module of the tag applies them.
- The game starts and stops a recording.

## The ids

An `ideatek:EntityId` names 1 entity across the saves, the sessions and the servers. The id module gives it once, and never gives it to another entity.
The high 16 bits are the setting `origin` of the id module, for example the number of a shard. The low 48 bits count. So 2 shards never give the same id.
An entity keeps its id when the game saves it, loads it, sends it to a peer, or moves it to another server.

## The stream

At `init`, each module that keeps rows spawns 1 `ideatek:ChangeTag`. The tag is the hash of the name of its component.
In each step, the module writes `ideatek:RowChanges` with the rows that changed. `data` holds records of `{id uint64_t, length uint32_t, bytes}`, little-endian, with no bytes between them.
`id` is the `ideatek:EntityId` of the entity. The module serializes its own fields, and writes each field that names an entity as the `ideatek:EntityId` of that entity.

A STATE holds the newest values, and the latest wins. An EVENT happens once. A REMOVE tells that the row is gone, so a save of a level holds only what changed.
A FULL holds each row, for the `request` that asks for it: a save, or a peer that joins. A `request` of 0 is the stream of the step. A `peer` of 0 is each peer.

## The durability classes

`durability` of a tag tells how a store keeps its changes. IMMEDIATE is durable in the round of the change, for example a trade.
PERIODIC is durable at least each `period` seconds, for example the positions. A period of 0 is each round. NONE never goes to a store, for example the effects.
A save file and the network take each class.

## The rounds

The changes of 1 round are 1 transaction. A store commits them together, or not at all, so a crash never keeps half a trade.
The store numbers each committed round and checks each one with a checksum, in its own format. The commit log of SpacetimeDB is an example.
`ideatek:StoreState` tells the last round that the store committed and the last round on its disk.

## The store

A store module keeps the world durable between the rounds. At start, it loads its newest snapshot, replays its log after it, and writes `ideatek:RowsIn` FULL for each tag.
Each module applies its rows, and the world goes on from the last durable round. A save file is then a snapshot that a player names.

## The rows that come in

`ideatek:RowsIn` holds rows for 1 tag. `source` is the net session, the save request or the store that wrote it, and `version` is the version of the bytes.
The module of the tag reads an older version and upgrades it in its own code. A PREDICTED holds the newest state of the entities that a client owns.

## The recordings

A recorder writes the stream to a file, for a demo or a replay. With PLAY, it writes `ideatek:RowsIn` from the file, as a client does with the network.
The path of a recording is a path of the files part. A recorder can also record the `ideatek:DebugDraw` batches of the debug_draw part.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `origin` | 0 | 0 to 65535 | The high 16 bits of each id of the id module. |

## ideatek:EntityId

`ideatek:EntityId` names an entity across the saves, the sessions and the servers. The id module writes it.
`ideatek_entity_id_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The id: *The ids*. |

## ideatek:ideatek_change_kind_t

`ideatek_change_kind_t` tells the kind of the rows of a change.
`ideatek_change_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CHANGE_KIND_STATE` | The newest values. The latest wins. |
| 1 | `IDEATEK_CHANGE_KIND_EVENT` | An event, once. |
| 2 | `IDEATEK_CHANGE_KIND_REMOVE` | The row is gone. |
| 3 | `IDEATEK_CHANGE_KIND_FULL` | Each row, for a request. |
| 4 | `IDEATEK_CHANGE_KIND_PREDICTED` | The newest state of the entities that a client owns. |

## ideatek:ideatek_durability_t

`ideatek_durability_t` tells how a store keeps the changes of a tag: *The durability classes*.
`ideatek_durability_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_DURABILITY_IMMEDIATE` | Durable in the round of the change. |
| 1 | `IDEATEK_DURABILITY_PERIODIC` | Durable at least each `period` seconds. |
| 2 | `IDEATEK_DURABILITY_NONE` | Never in a store. |

## ideatek:ideatek_change_condition_t

`ideatek_change_condition_t` limits the peers of a tag, as the conditions of Unreal.
`ideatek_change_condition_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CHANGE_CONDITION_ALL` | Each peer of the relevance. |
| 1 | `IDEATEK_CHANGE_CONDITION_OWNER_ONLY` | Only the owner. |
| 2 | `IDEATEK_CHANGE_CONDITION_SKIP_OWNER` | Each peer but the owner. |
| 3 | `IDEATEK_CHANGE_CONDITION_INITIAL_ONLY` | Only in the first send to a peer. |

## ideatek:ChangeTag

`ideatek:ChangeTag` makes a module a writer of changes: *The stream*.
`ideatek_change_tag_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `tag` | `uint64_t` | 0 | No default | The hash of the name of the component of the module. |
| `period` | `float` | 8 | 0 | The seconds of PERIODIC. 0 means each round. |
| `version` | `uint32_t` | 12 | No default | The version of the bytes of the module. |
| `durability` | `ideatek_durability_t` | 16 | `IDEATEK_DURABILITY_PERIODIC` | How a store keeps the changes. |
| `condition` | `ideatek_change_condition_t` | 17 | `IDEATEK_CHANGE_CONDITION_ALL` | The peers that get the changes. |

Bytes 18 to 23 hold no field.

## ideatek:RowChanges

`ideatek:RowChanges` holds the changes of the rows of 1 module in a step.
`ideatek_row_changes_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `tag` | `uint64_t` | 0 | No default | The tag of the module. |
| `data` | `bridge_text_t` | 8 | No default | The records: *The stream*. |
| `request` | `ideatek_entity_t` | 16 | 0 | The request of a FULL. 0 means the stream of the step. |
| `peer` | `uint32_t` | 24 | 0 | The peer of an event to 1 peer. 0 means each peer. |
| `kind` | `ideatek_change_kind_t` | 28 | No default | The kind. |

Bytes 29 to 31 hold no field.

## ideatek:RowsIn

`ideatek:RowsIn` holds rows that come in for 1 tag: *The rows that come in*. A consumer writes it.
`ideatek_rows_in_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `tag` | `uint64_t` | 0 | The tag of the module. |
| `data` | `bridge_text_t` | 8 | The records. |
| `source` | `ideatek_entity_t` | 16 | The net session, the save request or the store. |
| `peer` | `uint32_t` | 24 | The peer that sent the rows. 0 means none. |
| `version` | `uint32_t` | 28 | The version of the bytes. |
| `kind` | `ideatek_change_kind_t` | 32 | The kind. |

Bytes 33 to 39 hold no field.

## ideatek:StoreState

`ideatek:StoreState` is the state of a store, on its entity: *The rounds*. The store module writes it.
`ideatek_store_state_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `round` | `uint64_t` | 0 | The last round that the store committed. |
| `durable` | `uint64_t` | 8 | The last round on the disk of the store. |

## ideatek:ideatek_recording_action_t

`ideatek_recording_action_t` tells what a recording does.
`ideatek_recording_action_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_RECORDING_ACTION_RECORD` | Write the stream to the file. |
| 1 | `IDEATEK_RECORDING_ACTION_PLAY` | Read the file into the world. |
| 2 | `IDEATEK_RECORDING_ACTION_STOP` | Stop. |

## ideatek:Recording

`ideatek:Recording` asks a recorder to record or to play: *The recordings*.
`ideatek_recording_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `file` | `bridge_text_t` | 0 | The path of the file. |
| `action` | `ideatek_recording_action_t` | 8 | What it does. |

Bytes 9 to 11 hold no field.

## ideatek:ideatek_recording_status_t

`ideatek_recording_status_t` tells the state of a recording.
`ideatek_recording_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_RECORDING_STATUS_IDLE` | Nothing goes on. |
| 1 | `IDEATEK_RECORDING_STATUS_RECORDING` | The recorder writes. |
| 2 | `IDEATEK_RECORDING_STATUS_PLAYING` | The recorder reads. |
| 3 | `IDEATEK_RECORDING_STATUS_FAILED` | The file failed. |

## ideatek:RecordingState

`ideatek:RecordingState` is the state of a recording. The recorder writes it.
`ideatek_recording_state_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `round` | `uint64_t` | 0 | The round of the recording now. |
| `status` | `ideatek_recording_status_t` | 8 | The state. |

Bytes 9 to 15 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:EntityId` | `ideatek_entity_id_t` | `ideatek_entity_id_t_desc` | `IDEATEK_ENTITY_ID_FIELDS` |
| `ideatek:ideatek_change_kind_t` | `ideatek_change_kind_t` | `ideatek_change_kind_t_desc` | `IDEATEK_CHANGE_KIND_VALUES` |
| `ideatek:ideatek_durability_t` | `ideatek_durability_t` | `ideatek_durability_t_desc` | `IDEATEK_DURABILITY_VALUES` |
| `ideatek:ideatek_change_condition_t` | `ideatek_change_condition_t` | `ideatek_change_condition_t_desc` | `IDEATEK_CHANGE_CONDITION_VALUES` |
| `ideatek:ChangeTag` | `ideatek_change_tag_t` | `ideatek_change_tag_t_desc` | `IDEATEK_CHANGE_TAG_FIELDS` |
| `ideatek:RowChanges` | `ideatek_row_changes_t` | `ideatek_row_changes_t_desc` | `IDEATEK_ROW_CHANGES_FIELDS` |
| `ideatek:RowsIn` | `ideatek_rows_in_t` | `ideatek_rows_in_t_desc` | `IDEATEK_ROWS_IN_FIELDS` |
| `ideatek:StoreState` | `ideatek_store_state_t` | `ideatek_store_state_t_desc` | `IDEATEK_STORE_STATE_FIELDS` |
| `ideatek:ideatek_recording_action_t` | `ideatek_recording_action_t` | `ideatek_recording_action_t_desc` | `IDEATEK_RECORDING_ACTION_VALUES` |
| `ideatek:Recording` | `ideatek_recording_t` | `ideatek_recording_t_desc` | `IDEATEK_RECORDING_FIELDS` |
| `ideatek:ideatek_recording_status_t` | `ideatek_recording_status_t` | `ideatek_recording_status_t_desc` | `IDEATEK_RECORDING_STATUS_VALUES` |
| `ideatek:RecordingState` | `ideatek_recording_state_t` | `ideatek_recording_state_t_desc` | `IDEATEK_RECORDING_STATE_FIELDS` |

```text
{ uint64_t id; }
{ IDEATEK_CHANGE_KIND_STATE = 0, IDEATEK_CHANGE_KIND_EVENT = 1, IDEATEK_CHANGE_KIND_REMOVE = 2, IDEATEK_CHANGE_KIND_FULL = 3, IDEATEK_CHANGE_KIND_PREDICTED = 4 }
{ IDEATEK_DURABILITY_IMMEDIATE = 0, IDEATEK_DURABILITY_PERIODIC = 1, IDEATEK_DURABILITY_NONE = 2 }
{ IDEATEK_CHANGE_CONDITION_ALL = 0, IDEATEK_CHANGE_CONDITION_OWNER_ONLY = 1, IDEATEK_CHANGE_CONDITION_SKIP_OWNER = 2, IDEATEK_CHANGE_CONDITION_INITIAL_ONLY = 3 }
{ uint64_t tag; float period; uint32_t version; ideatek_durability_t durability; ideatek_change_condition_t condition; }
{ uint64_t tag; bridge_text_t data; ideatek_entity_t request; uint32_t peer; ideatek_change_kind_t kind; }
{ uint64_t tag; bridge_text_t data; ideatek_entity_t source; uint32_t peer; uint32_t version; ideatek_change_kind_t kind; }
{ uint64_t round; uint64_t durable; }
{ IDEATEK_RECORDING_ACTION_RECORD = 0, IDEATEK_RECORDING_ACTION_PLAY = 1, IDEATEK_RECORDING_ACTION_STOP = 2 }
{ bridge_text_t file; ideatek_recording_action_t action; }
{ IDEATEK_RECORDING_STATUS_IDLE = 0, IDEATEK_RECORDING_STATUS_RECORDING = 1, IDEATEK_RECORDING_STATUS_PLAYING = 2, IDEATEK_RECORDING_STATUS_FAILED = 3 }
{ uint64_t round; ideatek_recording_status_t status; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_change_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_durability_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_change_condition_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_recording_action_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_recording_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:EntityId` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ChangeTag` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:RowChanges` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:RowsIn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StoreState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Recording` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:RecordingState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| A module of rows | `ideatek:ChangeTag`, `ideatek:RowChanges` | `ideatek:RowsIn`, `ideatek:EntityId`, and the types of the fields |
| The id module | `ideatek:EntityId` | The types of the fields |
| A consumer | `ideatek:RowsIn`, `ideatek:StoreState`, `ideatek:RecordingState` | `ideatek:ChangeTag`, `ideatek:RowChanges`, `ideatek:EntityId`, `ideatek:Recording`, and the types of the fields |
| The game | `ideatek:Recording` | `ideatek:RecordingState`, `ideatek:StoreState`, and the types of the fields |

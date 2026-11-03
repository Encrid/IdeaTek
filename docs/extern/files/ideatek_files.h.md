# include/files/ideatek_files.h

`include/files/ideatek_files.h` gives the types of the files part: the requests of the game on the files, and their results.
`readme.md`, *The contracts*, tells the rules of each contract.

## The roles

- The game writes `ideatek:FileRequest`, and reads `ideatek:FileResult`.
- The files module writes `ideatek:FileResult`, and reads `ideatek:FileRequest`. A deployment has one files module.

Out: the free space, a rename, a copy, the watch of a folder, and the links of files.
Comes back: the free space, with a console or a phone as target; the read of a dropped file, when a game needs it.
Comes back: a call of Bridge, when a round needs the bytes in the same round.

## The storages

- `IDEATEK_STORAGE_CONTENT` is the folder `content_folder`, in the folder of the deliverable. The files module only reads it.
- `IDEATEK_STORAGE_USER` is the folder `user_folder`, in the folder that the OS gives for the data of programs. The files module reads it and writes it.

A deployment states its own `user_folder`, so that two games keep their files apart.

## The paths

A path is relative to the folder of its storage, in UTF-8. A `/` is between two names.
An empty path names the folder of the storage.
A path with the name `..`, the name `.`, an empty name, a `\` or a `:` gets `IDEATEK_FILE_STATUS_DENIED`.

## The requests

The game spawns an entity and attaches `ideatek:FileRequest` to it.
In a later round, the files module attaches `ideatek:FileResult` to the same entity.
An observer of `BRIDGE_ON_ENTER` tells the game of the result.
The files module destroys the entity in the round after the result. The game that keeps the bytes copies them.
The game writes the path and the bytes of a write in its share.

- `IDEATEK_FILE_ACTION_READ` reads `length` bytes from the byte `offset` of the file. A `length` of 0 reads to the end of the file.
- `IDEATEK_FILE_ACTION_WRITE` puts `data` in place of the whole file. The files module makes each folder of the path that is not there.
- `IDEATEK_FILE_ACTION_LIST` gives 1 line for each entry of the folder.
- `IDEATEK_FILE_ACTION_REMOVE` removes the file, or the folder if the folder is empty.

A write goes to a temporary file first. Then the files module renames the temporary file over the old file, so a failed write keeps the old file.
A write or a removal in `IDEATEK_STORAGE_CONTENT` gets `IDEATEK_FILE_STATUS_DENIED`.

A line of a list ends with `\n`. A file gives its name, a space and its size in decimal.
A folder gives its name and `/`. The files module skips an entry whose name holds `\n`.

## The bytes of the results

At `init`, the files module takes `read_bytes` bytes of its persistent arena.
It puts the bytes of the results of a round in one half, and the bytes of the next round in the other half.
So the bytes of a result stay good until the files module destroys its entity.
A result whose bytes are more than the rest of the half gets `IDEATEK_FILE_STATUS_TOO_BIG`.
The `persistent` of its manifest includes `read_bytes`.

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `read_bytes` | 4194304 | 2 to 1073741824, an even number | The bytes for the results. Each round uses at most the half. |
| `user_folder` | `IdeaTek` | A name that the path rule allows. | The folder of `IDEATEK_STORAGE_USER`. |
| `content_folder` | `content` | A path that the path rule allows. | The folder of `IDEATEK_STORAGE_CONTENT`. |

## ideatek:FileRequest

`ideatek:FileRequest` asks for an action on a file or on a folder.
`ideatek_file_request_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `offset` | `uint64_t` | 0 | With `IDEATEK_FILE_ACTION_READ`: the first byte to read. |
| `length` | `uint64_t` | 8 | With `IDEATEK_FILE_ACTION_READ`: the number of bytes to read. |
| `path` | `bridge_text_t` | 16 | The path. |
| `data` | `bridge_text_t` | 24 | With `IDEATEK_FILE_ACTION_WRITE`: the bytes of the file. |
| `storage` | `ideatek_storage_t` | 32 | The storage of the path. |
| `action` | `ideatek_file_action_t` | 33 | The action. |

Bytes 34 to 39 hold no field.

### ideatek:ideatek_storage_t

`ideatek_storage_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The storage |
| --- | --- | --- |
| `IDEATEK_STORAGE_CONTENT` | 0 | The content of the game. |
| `IDEATEK_STORAGE_USER` | 1 | The files of the user. |

### ideatek:ideatek_file_action_t

`ideatek_file_action_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The action |
| --- | --- | --- |
| `IDEATEK_FILE_ACTION_READ` | 0 | Read a file. |
| `IDEATEK_FILE_ACTION_WRITE` | 1 | Write a file. |
| `IDEATEK_FILE_ACTION_LIST` | 2 | List a folder. |
| `IDEATEK_FILE_ACTION_REMOVE` | 3 | Remove a file or a folder. |

## ideatek:FileResult

`ideatek:FileResult` gives the result of a request.
`ideatek_file_result_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `size` | `uint64_t` | 0 | With `IDEATEK_FILE_ACTION_READ`: the full size of the file in bytes. Else 0. |
| `data` | `bridge_text_t` | 8 | The bytes that were read, or the lines of the list. Else no bytes. |
| `status` | `ideatek_file_status_t` | 16 | The status. |

Bytes 17 to 23 hold no field.

### ideatek:ideatek_file_status_t

`ideatek_file_status_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The status |
| --- | --- | --- |
| `IDEATEK_FILE_STATUS_OK` | 0 | The action is done. |
| `IDEATEK_FILE_STATUS_NOT_FOUND` | 1 | The path names no file and no folder. |
| `IDEATEK_FILE_STATUS_TOO_BIG` | 2 | The bytes do not fit in the half: *The bytes of the results*. |
| `IDEATEK_FILE_STATUS_DENIED` | 3 | The path or the storage does not allow the action. |
| `IDEATEK_FILE_STATUS_FAILED` | 4 | The OS did not do the action. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_storage_t` | `ideatek_storage_t` | `ideatek_storage_t_desc` | `IDEATEK_STORAGE_VALUES` |
| `ideatek:ideatek_file_action_t` | `ideatek_file_action_t` | `ideatek_file_action_t_desc` | `IDEATEK_FILE_ACTION_VALUES` |
| `ideatek:ideatek_file_status_t` | `ideatek_file_status_t` | `ideatek_file_status_t_desc` | `IDEATEK_FILE_STATUS_VALUES` |
| `ideatek:FileRequest` | `ideatek_file_request_t` | `ideatek_file_request_t_desc` | `IDEATEK_FILE_REQUEST_FIELDS` |
| `ideatek:FileResult` | `ideatek_file_result_t` | `ideatek_file_result_t_desc` | `IDEATEK_FILE_RESULT_FIELDS` |

```text
{ IDEATEK_STORAGE_CONTENT = 0, IDEATEK_STORAGE_USER = 1 }
{ IDEATEK_FILE_ACTION_READ = 0, IDEATEK_FILE_ACTION_WRITE = 1, IDEATEK_FILE_ACTION_LIST = 2, IDEATEK_FILE_ACTION_REMOVE = 3 }
{ IDEATEK_FILE_STATUS_OK = 0, IDEATEK_FILE_STATUS_NOT_FOUND = 1, IDEATEK_FILE_STATUS_TOO_BIG = 2, IDEATEK_FILE_STATUS_DENIED = 3, IDEATEK_FILE_STATUS_FAILED = 4 }
{ uint64_t offset; uint64_t length; bridge_text_t path; bridge_text_t data; ideatek_storage_t storage; ideatek_file_action_t action; }
{ uint64_t size; bridge_text_t data; ideatek_file_status_t status; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_storage_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_file_action_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_file_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:FileRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:FileResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares the three enums before `ideatek:FileRequest` and `ideatek:FileResult`.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:FileRequest` | The three enums and `ideatek:FileResult` |
| The files module | `ideatek:FileResult` | The three enums and `ideatek:FileRequest` |

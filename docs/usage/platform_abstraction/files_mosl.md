# BM-IdeaTek-Files-Mosl

The files module of IdeaTek on mosl: the reads, the writes, the lists and the watches of the 2 storages of a game.

## Install

The release of `BM-IdeaTek-Files-Mosl` has the tag `files_mosl-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Files-Mosl.zip` and `sha256.txt`. `BM-IdeaTek-Files-Mosl.zip` has a folder for each target.
In `BM-IdeaTek-Files-Mosl`, each folder holds the module `files_mosl.dll`, `files_mosl.so` or `files_mosl.dylib`, and the notice of mosl.
Each folder of `BM-IdeaTek-Files-Mosl.zip` also holds this readme and the licence.

The module holds the static archive of mosl v0.5.0 inside it, so it needs no library beside it: https://github.com/Encrid/mosl/releases/tag/v0.5.0.
A project names the zip of `BM-IdeaTek-Files-Mosl` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_files_mosl:files@0.1.0`.
Its role is the files module: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The roles*.
The component of its settings is `ideatek_files_mosl:Settings`. It holds the settings of `docs/extern/platform_abstraction/ideatek_files.h.md`, *The bytes of the results*: `read_bytes`, `user_folder` and `content_folder`. It has no setting beyond those 3.

### The folders

The folder of the deliverable is the folder of the program of the process. `content_folder` is in it.
`user_folder` is in the folder of the system for the data of programs:

| System | The folder of the data of programs |
| --- | --- |
| Windows | The variable `APPDATA`. |
| Linux | The variable `XDG_DATA_HOME`, else `.local/share` in the variable `HOME`. |
| macOS | `Library/Application Support` in the variable `HOME`. |

At its start, the module makes `user_folder` and each folder above it that is not there.
A system with no such variable gives no user storage: each request of `IDEATEK_STORAGE_USER` gets `IDEATEK_FILE_STATUS_UNSUPPORTED`, and the log gets 1 line.

### The requests

A worker thread of the module does the requests, in the order of `docs/extern/platform_abstraction/ideatek_files.h.md`, *The order of the requests*.
The system of the module takes the results of the thread in each round, and attaches them.
A read takes the whole file, then keeps the bytes from `offset` for `length`. So a read of a part of a large file costs the whole file.

A refusal, and the start of a watch, need no work of the thread: their results come in the next round.
A list gives the size of each file with 1 call of the system for each entry. A removal of a folder that is not empty gets `IDEATEK_FILE_STATUS_DENIED`.

A watch sees the changes that the system reports, down to every folder below the watched folder.
When the system reports lost changes, the changed path of the round is the watched folder itself. So the game looks at all of it again.
When the watched folder goes, the watch gets `ideatek:FileResult` with `IDEATEK_FILE_STATUS_NOT_FOUND`, and the log gets 1 line. The entity stays until the game destroys it.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The module takes the results and attaches them on the thread of the phases. The worker thread is its own. |
| `reloadable` | `false` | The queue of the requests, the open folders and the watches are not in the storage of Bridge. |
| `spawns` | `"none"` | The module spawns no entity: it attaches its results to the entities of the game. |
| `changes` | 65536 | The attaches of the results and of the changes of a round, and the destroys of the entities of the round before. |
| `persistent` | 4210688 | At least `read_bytes`, plus 16384 for the paths of the 2 storages and the texts of the settings. |
| `scratch` | 16384 | The declarations, and the lines of the log. |
| `log_ring` | 1024 | 32 bytes for each line of the log. |
| `spans` | 16384 | The list that the observer of the requests reads. |

- The system `results` of the module runs in its phase `first`. The deployment maps it to the phase that starts the round.
- A pair puts `ideatek_files_mosl:files:results` before each reader of `ideatek:FileResult` and `ideatek:FileChange` of the first phase.
- The module takes the grants of the files module: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The grants*.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `no thread` | The system refuses the worker thread. The load fails. |
| `no content folder: <path>` | `content_folder` is not a folder beside the program. Each request of `IDEATEK_STORAGE_CONTENT` gets `IDEATEK_FILE_STATUS_NOT_FOUND`. |
| `no user folder: <reason>` | The system has no variable for the data of programs, or refuses to make `user_folder`. Each request of `IDEATEK_STORAGE_USER` gets `IDEATEK_FILE_STATUS_UNSUPPORTED`. |
| `the watch of <path> stopped` | The watched folder went. The watch gets `IDEATEK_FILE_STATUS_NOT_FOUND`. |
| `no result for <entity>: <count> results wait for the next round` | 256 refusals or starts of watches wait for the next round already. The request gets no result. |

A setting outside its range gets the line that `readme.md`, *The contracts*, gives.

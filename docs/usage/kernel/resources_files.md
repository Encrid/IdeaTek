# BM-IdeaTek-Resources-Files

The resources module of IdeaTek on the files part: it loads the files of a game into resources, and reloads them.

## Install

The release of `BM-IdeaTek-Resources-Files` has the tag `resources_files-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Resources-Files.zip` and `sha256.txt`. `BM-IdeaTek-Resources-Files.zip` has a folder for each target.
In `BM-IdeaTek-Resources-Files`, each folder holds the module `resources_files.dll`, `resources_files.so` or `resources_files.dylib`.
Each folder of `BM-IdeaTek-Resources-Files.zip` also holds this readme and the licence.

The module reads the files through the files module of the deployment: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The roles*. Nothing stands beside it.
A project names the zip of `BM-IdeaTek-Resources-Files` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_resources_files:resources@0.1.0`.
Its role is the resources module: `docs/extern/kernel/ideatek_resources.h.md`, *The roles*.
The component of its settings is `ideatek_resources_files:Settings`. It holds the 9 settings of `docs/extern/kernel/ideatek_resources.h.md`, *The settings*, and the 9 bounds of the module:

| Setting | Type | Default | Range | Meaning |
| --- | --- | --- | --- | --- |
| `max_resources` | `uint32_t` | 4096 | 1 to 1048576 | The resources with an entity at one time. |
| `max_uses` | `uint32_t` | 8192 | 1 to 4194304 | The use entities at one time. |
| `max_loaders` | `uint32_t` | 64 | 1 to 4096 | The loaders. |
| `max_sources` | `uint32_t` | 16 | 1 to 1024 | The named sources and the sources of the source modules, together. |
| `max_variables` | `uint32_t` | 64 | 1 to 4096 | The variables of the paths. |
| `max_reads` | `uint32_t` | 64 | 1 to 65536 | The reads of files that run at one time. |
| `max_labels` | `uint32_t` | 4096 | 1 to 1048576 | The sub-resources at one time. |
| `path_bytes` | `uint32_t` | 256 | 16 to 4096 | The bytes of the path of a resource, with its source, its label and its type. |
| `settings_bytes` | `uint32_t` | 256 | 0 to 65536 | The bytes of the `settings` of a resource, from its first load or its meta file. |

A load above `max_resources`, or with a path or settings above their bytes, gets `IDEATEK_RESOURCE_ERROR_NO_ROOM`.
A use entity, a loader, a source, a variable or a label above its bound is not taken, and the log gets 1 line.

### The reads

The module reads a file with an entity of `ideatek:FileRequest` that it spawns, with the `priority` of the load.
A path of the default source or of a named source reads `IDEATEK_STORAGE_CONTENT`. A path of the source `user` reads `IDEATEK_STORAGE_USER`.
A path of a source module gets an entity of `ideatek:ResourceSourceRead`.

The module copies the bytes of each result into a block of its `load_bytes`, in the round of the result. It frees the block when it detaches `ideatek:ResourceBytes`.
A read whose bytes fit in no free block waits for a later round, as the contract says. A file above `load_bytes` gets `IDEATEK_RESOURCE_ERROR_TOO_BIG`, except with a `ranged` loader.

### The meta files

The module reads a meta file as TOML with these lines, and no other line:

- `action = "<word>"` and `loader = "<name>"`, before the first header, each at most 1 time.
- A line that starts with `#`, and an empty line, anywhere.
- The header `[settings]`, 1 time at most. The lines after it, up to the end of the file, are the `settings` of the loader as text.

A meta file with another line gets `IDEATEK_RESOURCE_ERROR_READ_FAILED` on the load, and 1 line of the log names the number of the line.
A loader reads its `settings` with the reader of its own choice.

### The reloads

With `hot_reload` at `true`, the module watches the folder of the default source and of each named source. Each watch is 1 entity of `ideatek:FileRequest`.
A changed path waits `reload_delay_ms` after its last change. The module reads the time in `real_delta` of `ideatek:Frame` of the clock module.
A deployment with no clock module reloads in the round after the change. The log gets 1 line at the first change.

### The owners

In each round, the module looks at the `owner` of each use entity in the entity index of Bridge.
A use entity whose `owner` is dead is destroyed in that round, with the effects that the contract gives.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `false` | No library: any thread of Bridge may run the module. |
| `reloadable` | `false` | The tables of the module are in its persistent arena, but its reload is not tested. |
| `spawns` | 4176 | 1 entity for each resource and each read that runs, and 1 for the watch of each source: `max_resources` + `max_reads` + `max_sources`. A larger bound needs a larger `spawns`. |
| `changes` | 65536 | The handles, the states, the bytes and the tags that a round attaches, plus its destroys of the resources that go. |
| `persistent` | 8388608 | `load_bytes` of the contract, plus 4194304 for the tables, the slots and the maps at their defaults. A larger bound needs a larger `persistent`. |
| `scratch` | 16384 | What `init` declares, and the lines of the log. |
| `log_ring` | 2048 | Each line of the log takes 32 bytes of the ring. |
| `spans` | 16384 | The spans of its queries, the columns of its types and the lists of its observers. |

- The module has 2 systems in its phase `first`, mapped by the deployment to the phase that starts the round: `round`, then `found`.
  In its first call, `found` takes each loader that exists.
  So the module also takes a loader that another module spawned at `init`, before the start of the resources module.
- A pair puts `ideatek_resources_files:resources:round` after `ideatek_files_mosl:files:results`, and before each reader of `ideatek:Resource` and `ideatek:ResourceHandle` of the first phase.
- The module takes the grants of the resources module: `docs/extern/kernel/ideatek_resources.h.md`, *The grants*. It also reads `ideatek:Frame` of `docs/extern/kernel/ideatek_frame.h.md`, for the delay of the reloads.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `the meta file of <path> is refused at line <n>` | A line of the meta file is outside the subset of *The meta files*. |
| `no room for the use <entity>` | A use entity above `max_uses`. The use gets no handle. |
| `no room for the resource <path>` | A load above `max_resources`, when no record is free for its failure. The use gets no handle. |
| `no room for the loader <name>` | A loader above `max_loaders`. The loader is not taken. |
| `no room for the label <label>` | A sub-resource above `max_labels`. The sub-resource gets no `ideatek:Resource`. |
| `no room for the source <name>` | A source above `max_sources`. The source is not taken. |
| `no room for the variable <name>` | A variable above `max_variables`. The variable is not taken. |
| `2 sources have the name <name>` | The module keeps the first, as the contract says. |
| `the source name <name> is refused` | The name of a source module has a character outside `a` to `z`, `0` to `9` and `_`. The source is not taken. |
| `2 variables have the name <name>` | The module keeps the first, as the contract says. |
| `no clock: a change reloads in the round after` | `hot_reload` is `true` and the deployment has no `ideatek:Frame`. |
| `no watch of <folder>: <status>` | The files module refused the watch of a source folder. The files of that source do not reload. |

When a setting is outside its range, the module writes the line of `readme.md`, *The contracts*.

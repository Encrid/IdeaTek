# src/platform_abstraction/files_mosl/files_declare.c

This file gives Bridge the types, the system and the observer of the files module.

## The declarations

The module declares the 3 enums of the contract, then its 3 structures, then its settings: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The declarations*.

| Call | Kind | Name |
| --- | --- | --- |
| System | `first` | `results` |
| Observer | write and detach | `request` on `ideatek:FileRequest` |

`results` states `ideatek:FileResult` and `ideatek:FileChange` as its changes. The observer states no change: it only queues the request.

## Functions

| Function | Result |
| --- | --- |
| `declare_types` | Declares the 7 types. |
| `add_runs` | Declares the system and the observer. |

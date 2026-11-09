# src/kernel/resources_files/resources_copy.c

This file reads the result of a read, and copies its bytes into the arena.

## The result

`read_arrival` reads `ideatek:FileResult` of a read of the files module, or `ideatek:ResourceSourceResult` of a read of a source module. It gives the size of the file, the bytes, and the error of the contract.
A status of the files part maps to an error of the contract. `NOT_FOUND` and `TOO_BIG` keep their meaning, and each other status that is not ok is a failed read.
`copy_bytes` takes a block of the arena for the bytes, and copies them: `docs/intern/kernel/resources_files/resources_arena.c.md`. It gives `false` when no gap holds them.

## Functions

| Function | Result |
| --- | --- |
| `read_arrival` | The size, the bytes and the error of a result. |
| `copy_bytes` | Copies the bytes into a block of the resource. `false` with no gap. |

# src/engine_subsystems/loader_ktx2/ktx2_say.c

This file writes the lines that name a resource, and attaches the outcome.

## The path

`say_path` reads the path of `ideatek:Resource`, and writes it before the rest of the line. An entity with no `ideatek:Resource` gets an empty path.
The resources module attaches `ideatek:Resource` before `ideatek:ResourceBytes`, so the empty path is a guard: `.toolchain/project.toml` excepts its 2 outcomes.

## Functions

| Function | Result |
| --- | --- |
| `say_path` | Writes 1 line of the warn level: the path, then the rest. |
| `give_outcome` | Attaches `ideatek:ResourceOutcome`: `IDEATEK_RESOURCE_ERROR_NONE`, or `IDEATEK_RESOURCE_ERROR_LOADER_FAILED`. |

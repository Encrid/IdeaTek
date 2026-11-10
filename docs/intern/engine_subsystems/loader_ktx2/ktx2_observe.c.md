# src/engine_subsystems/loader_ktx2/ktx2_observe.c

This file adds the 3 observers of the module.

## The observers

| Observer | Events | Term | Changes |
| --- | --- | --- | --- |
| `bytes` | enter, attach | `ideatek:ResourceBytes` | `ideatek:Texture`, `ideatek:Sampler` and `ideatek:ResourceOutcome` |
| `swap` | attach | `ideatek:Texture` | None |
| `gone` | leave | `ideatek:Texture` | None |

## Functions

| Function | Result |
| --- | --- |
| `add_observers` | Adds the 3 observers, and keeps the index of their lists. |

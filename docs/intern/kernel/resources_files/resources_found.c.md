# src/kernel/resources_files/resources_found.c

This file holds the system `found`, which takes the loaders that exist when the module starts.

## The loaders before the start

A loader module spawns its loader at `init`: `docs/extern/kernel/ideatek_resources.h.md`, *The loaders*.
When that `init` runs before the start of the resources module, the observer `loaders` cannot record the loader. The tables do not exist yet, or the observer of the module does not exist yet.
So in its first call, `found` walks its query of `ideatek:ResourceLoader`, and calls `add_loader` for each entity.

No loader has a record yet: the observer records nothing before the start, and no barrier runs between `round` and `found`.
Then `found` sets `found_done`, and each later call does nothing. The observer `loaders` records each later loader.

`found` runs after `round` in the same phase, so `round` has started the module and taken the tables. A broken module stops in `round`, and `found` does not run.

## Functions

| Function | Result |
| --- | --- |
| `add_span` | Calls `add_loader` for each entity of 1 span. |
| `found` | The system: takes the loaders of its query 1 time. |

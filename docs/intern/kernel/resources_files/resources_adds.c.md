# src/kernel/resources_files/resources_adds.c

This file gives a use entity its handle, and places the additions: the resources that a module made in its code.

## An addition

`place_add` reads `ideatek:ResourceAdd` of a use entity. `add_record` gives the record of the entity of the addition. A new entity gets a record with an empty path, the type of the addition, `STEP_DONE` and `IDEATEK_LOAD_STATE_LOADED`.
A full table gets the line `no room for the resource <type>`, and the use gets no handle.

`attach_handle` attaches `ideatek:ResourceHandle` to a use entity, and marks the use as handled. The loads use it too: `docs/intern/kernel/resources_files/resources_uses.c.md`.
`adds` is the observer of `ideatek:ResourceAdd`: `docs/intern/kernel/resources_files/resources_loads.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `attach_handle` | Attaches the handle of a use entity. |
| `add_record` | The record of an addition, new or found. |
| `place_add` | Gives an addition its resource and its handle. |
| `adds` | The observer of the additions. |

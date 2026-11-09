# src/kernel/resources_files/resources_choice.c

This file picks the loader of a resource at `STEP_PICK`.

## The pick

`ask_for` fills the ask of the pick. It names the views of the loaders, the type and the extension of the load, and the bytes of a read that came before.

`pick_for` picks. A load of the type `ideatek:RawBytes` takes the loader `raw` of the module. A resource with a loader from its meta file keeps it. Else `pick_loader` applies the rules: `docs/intern/kernel/resources_files/resources_pick.c.md`.
An error fails the resource. Else the step becomes `STEP_READ`, and a resource that holds its bytes already gives them to the loader at once.

## Functions

| Function | Result |
| --- | --- |
| `ask_for` | Fills the ask of the pick for a resource. |
| `pick_for` | Picks the loader of a resource, or fails it. |

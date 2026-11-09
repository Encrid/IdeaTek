# src/kernel/resources_files/resources_finish.c

This file ends the loads when their loader answers, folds the states and turns the tags of 1 round.

## The outcomes

The observer `outcomes` hears `ideatek:ResourceOutcome` on a resource, and records its error in the record.
In the round, `finish_loads` ends each resource at `STEP_LOADER` with an outcome: `IDEATEK_RESOURCE_ERROR_LOADER_FAILED` fails it, else its state is `IDEATEK_LOAD_STATE_LOADED`. The module detaches `ideatek:ResourceBytes` and `ideatek:ResourceOutcome`, frees the block, and the step is `STEP_DONE`.
A reload that ends attaches `ideatek:ResourceModified`, and reloads each `owner` of its uses that asked `reload_owner`: `docs/intern/kernel/resources_files/resources_reload.c.md`.
An outcome of a range frees its block and detaches the 2 components, and the state stays.

## The fold

`fold_states` runs in a dirty round. Each pass walks the uses with an `owner` that is a resource, and folds the states of their resources into the owner, as `docs/intern/kernel/resources_files/resources_fold.c.md` says. The passes stop when one changes nothing, or at `RES_PASSES`.
`write_view` attaches `ideatek:Resource` to each resource whose view changed. `ideatek:ResourceReady` comes when `recursive` becomes `IDEATEK_LOAD_STATE_LOADED`, and `ideatek:ResourceFailed` when a state becomes `IDEATEK_LOAD_STATE_FAILED`.

## The tags

`turn_tags` runs first in each round. It detaches the tags of the round before: `ideatek:ResourceReady`, `ideatek:ResourceFailed`, `ideatek:ResourceModified` and `ideatek:RawBytes`, and frees the block of a raw resource.

## Functions

| Function | Result |
| --- | --- |
| `outcomes` | The observer of the outcomes. |
| `finish_loads` | Ends the resources whose loader answered. |
| `fold_states` | Folds `dependencies` and `recursive` of each resource. |
| `write_view` | Writes `ideatek:Resource` and the tags of a resource that changed. |
| `turn_tags` | Detaches the tags of the round before. |

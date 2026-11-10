# src/kernel/resources_files/resources_finish.c

This file ends the loads when their loader answers.

## The outcomes

The observer `outcomes` hears `ideatek:ResourceOutcome` on a resource, and records its error in the record.
In the round, `finish_loads` ends each resource at `STEP_LOADER` with an outcome: `IDEATEK_RESOURCE_ERROR_LOADER_FAILED` fails it, else its state is `IDEATEK_LOAD_STATE_LOADED`. The module detaches `ideatek:ResourceBytes` and `ideatek:ResourceOutcome`, frees the block, and the step is `STEP_DONE`.
A reload that ends attaches `ideatek:ResourceModified`, and reloads each `owner` of its uses that asked `reload_owner`: `docs/intern/kernel/resources_files/resources_reload.c.md`.
An outcome of a range frees its block and detaches the 2 components, and the state stays.

The fold of the states and the views are in `docs/intern/kernel/resources_files/resources_states.c.md`, and the tags of 1 round in `docs/intern/kernel/resources_files/resources_tags.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `outcomes` | The observer of the outcomes. |
| `reload_owners` | Reloads each owner that asked `reload_owner` of a reloaded resource. `give_raw` also calls it. |
| `end_load` | Ends 1 load: the state, the tags, the owners and the labels. |
| `finish_loads` | Ends the resources whose loader answered. |

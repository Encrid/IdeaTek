# src/kernel/resources_files/resources_states.c

This file runs the fold of the states in a dirty round, and writes the views of the resources that changed.

## The fold

`fold_states` runs in a dirty round, in up to `RES_PASSES` passes, until a pass changes nothing. When the last pass still changes a state, the round stays dirty, and the fold goes on in the next round.
`fold_uses` walks the uses whose `owner` is a resource. It folds the state and the `recursive` of the resource of each use into its owner: `docs/intern/kernel/resources_files/resources_fold.c.md`.
`fold_views` folds the own state of each resource into its `recursive`, and writes `dependencies`, `recursive` and the first error of the view. A view that differs marks the resource changed.

## The views

`write_view` attaches `ideatek:Resource` to a resource that changed. `ideatek:ResourceReady` comes when `recursive` becomes `IDEATEK_LOAD_STATE_LOADED`. The tags that the round made pending come with it: `ideatek:ResourceFailed` and `ideatek:ResourceModified`. The tags go in the next round: `docs/intern/kernel/resources_files/resources_tags.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `fold_uses` | Folds the dependencies into their owners. |
| `fold_views` | Writes the folded states into the views. `true` when a view moved. |
| `write_view` | Writes `ideatek:Resource` and the tags of a resource that changed. |
| `fold_states` | Runs the passes, then writes the views that changed. |

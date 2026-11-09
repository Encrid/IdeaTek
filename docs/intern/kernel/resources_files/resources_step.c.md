# src/kernel/resources_files/resources_step.c

This file moves a resource through the steps of its load, up to the bytes that its loader gets.

## The steps

`step_resource` runs in each round for each resource that is not at `STEP_READING`, `STEP_LOADER` or `STEP_DONE`:

| Step | Action |
| --- | --- |
| `STEP_NEW` | With `meta_check` at `always`, or at `paths` with the path in `meta_paths`: the read of the meta file starts, and the step is `STEP_META`. Else the step is `STEP_PICK`. |
| `STEP_META` | Nothing: the result moves the step. |
| `STEP_PICK` | `pick_loader` gives the loader, or an error that fails the resource, or asks for the bytes. The step is `STEP_READ`. |
| `STEP_READ` | Nothing here: `start_reads` starts the read by priority. |

A failed pick, a meta file with `ignore`, or a refused meta file fails the resource at once. `fail_resource` writes the state, the error and the step `STEP_DONE`.

## The meta file

`apply_meta` takes what `meta_read` gives. `ignore` gives `IDEATEK_RESOURCE_ERROR_IGNORED`. A refused file gives `IDEATEK_RESOURCE_ERROR_READ_FAILED`, and the line of the meta file of `docs/usage/kernel/resources_files.md`, *The messages*, with the number of the refused line.
A name of a loader is resolved at once, with the rules of the pick by name. Settings of the meta file go to the slot of the resource when the load gave none. Settings above `settings_bytes` give `IDEATEK_RESOURCE_ERROR_NO_ROOM`.

## The bytes

`give_bytes` attaches `ideatek:ResourceBytes` with the entity of the loader, the offset, the size of the file and the block. The settings of the slot come with it, and no error. The step is `STEP_LOADER`.
With the loader `raw` of the module itself, `give_bytes` attaches `ideatek:RawBytes` in place of `ideatek:ResourceBytes`, and the resource is loaded at once. The tag goes in the next round, and the block with it.

## Functions

| Function | Result |
| --- | --- |
| `fail_resource` | Writes the failure of a resource. |
| `wants_meta` | `true` when the settings ask the meta file of a path. |
| `apply_meta` | Applies a meta file to a resource. |
| `pick_for` | Builds the views of the loaders and picks the loader of a resource. |
| `step_resource` | Moves 1 resource by 1 step. |
| `give_bytes` | Gives the bytes of a resource to its loader. |

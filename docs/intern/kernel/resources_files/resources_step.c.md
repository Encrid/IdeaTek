# src/kernel/resources_files/resources_step.c

This file moves a resource through the steps of its load, up to the bytes that its loader gets.

## The steps

`step_resource` runs in each round for each resource that is not at `STEP_READING`, `STEP_LOADER` or `STEP_DONE`:

| Step | Action |
| --- | --- |
| `STEP_NEW` | With `meta_check` at `always`, or at `paths` with the path in `meta_paths`: the read of the meta file starts, and the step is `STEP_META`. Else the step is `STEP_PICK`. |
| `STEP_META` | Nothing: the result moves the step. |
| `STEP_PICK` | `pick_for` gives the loader, or an error that fails the resource, or asks for the bytes. The step is `STEP_READ`. The step lasts no round: `step_resource` and `apply_meta` pick at once. |
| `STEP_READ` | Nothing here: `start_reads` starts the read by priority. |

A failed pick, a meta file with `ignore`, or a refused meta file fails the resource at once. `fail_resource` writes the state, the error and the step `STEP_DONE`.

## The meta file and the pick

The meta file goes through `docs/intern/kernel/resources_files/resources_apply.c.md`, and the pick through `docs/intern/kernel/resources_files/resources_choice.c.md`.

## The bytes

`give_bytes` attaches `ideatek:ResourceBytes` with the entity of the loader, the offset, the size of the file and the block. The settings of the slot come with it, and no error. The step is `STEP_LOADER`.
With the loader `raw` of the module itself, `give_bytes` attaches `ideatek:RawBytes` in place of `ideatek:ResourceBytes`, and the resource is loaded at once. The tag goes in the next round, and the block with it.

The end of a reload of a raw resource reloads each owner that asked `reload_owner`.
The end of a load by a loader does the same: `docs/intern/kernel/resources_files/resources_finish.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `fail_resource` | Writes the failure of a resource. |
| `wants_meta` | `true` when the settings ask the meta file of a path. |
| `step_resource` | Moves 1 resource by 1 step. |
| `give_raw` | Gives the bytes of a resource of the loader `raw`. |
| `give_bytes` | Gives the bytes of a resource to its loader. |

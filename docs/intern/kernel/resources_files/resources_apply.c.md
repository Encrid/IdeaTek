# src/kernel/resources_files/resources_apply.c

This file applies a meta file to a resource.

## The meta file

`apply_meta` gets the bytes of the meta file, or no bytes when the file was not found: then the defaults apply.
`meta_read` reads the bytes: `docs/intern/kernel/resources_files/resources_meta.c.md`. A refused file writes the line of the meta file with the number of the line, and the error is `IDEATEK_RESOURCE_ERROR_READ_FAILED`.
`meta_error` applies the 3 rules, in this order, and keeps the first error:

1. A name of a loader picks the loader at once: `named_loader`, with the rules of the pick by name.
2. `ignore` gives `IDEATEK_RESOURCE_ERROR_IGNORED`.
3. `take_settings` copies the settings of the meta file into the slot of the resource, when the load gave none. Settings above `settings_bytes` give `IDEATEK_RESOURCE_ERROR_NO_ROOM`.

An error fails the resource. Else the step becomes `STEP_PICK`, and `pick_for` picks the loader in the same round.

## Functions

| Function | Result |
| --- | --- |
| `take_settings` | Copies the settings of the meta file. `false` when they do not fit. |
| `named_loader` | Picks the loader that the meta file names. The error of the pick. |
| `meta_error` | The error of the 3 rules, or none. |
| `apply_meta` | Applies a meta file, or its absence, to a resource. |

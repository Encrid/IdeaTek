# src/kernel/resources_files/resources_handles.c

This file gives the handles of the loads of `path#label`, and marks the labels of a reload.

## The handles of the labels

A load of `path#label` waits for the resource of `path`. When that resource is done, `label_handle` gives the use its handle:

- When `path` failed, the handle names the resource of `path`.
- Else `label_target` looks for the sub-resource with the label: `find_label`. A label that no sub-resource holds gets a new record with a spawned entity, `IDEATEK_LOAD_STATE_FAILED` and `IDEATEK_RESOURCE_ERROR_NO_LABEL`.

`place_labels` attaches `ideatek:Resource` to each new sub-resource, in the round after the loader spawned it.

## The labels of a reload

`mark_labels` runs when a load ends. A sub-resource that the loader did not attach `ideatek:ResourceLabel` to during the reload gets `IDEATEK_RESOURCE_ERROR_NO_LABEL`. Each other sub-resource takes the state of its parent.

## Functions

| Function | Result |
| --- | --- |
| `find_label` | The sub-resource of a parent and a label, or `RES_NONE`. |
| `place_labels` | Writes the view of each new sub-resource. |
| `label_target` | The sub-resource of a label, found or made as failed. |
| `label_handle` | Gives a use of `path#label` its handle, when its parent is done. |
| `mark_labels` | Marks the gone labels and the states of the labels that stay. |

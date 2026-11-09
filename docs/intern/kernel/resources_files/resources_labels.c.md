# src/kernel/resources_files/resources_labels.c

This file holds the sub-resources: the observer `labels`, their records, and the handles of the loads of `path#label`.

## The labels

A loader spawns 1 entity for each sub-resource, with `ideatek:ResourceLabel` and its component. The observer takes a record for it: the entity, the parent, the path `path#label` and the type of the parent. The record is a resource with a parent, counted by `max_labels`.
A label above `max_labels` gets no record and the line `no room for the label <label>`.
In the round, `place_labels` attaches `ideatek:Resource` to each new sub-resource, with the state of its parent.

During a reload, the loader attaches `ideatek:ResourceLabel` again on each label that stays, and the observer marks the record as seen. When the reload ends, a sub-resource that was not seen gets `IDEATEK_LOAD_STATE_FAILED` and `IDEATEK_RESOURCE_ERROR_NO_LABEL`.

## The handles of the labels

A load of `path#label` waits for the resource of `path`. When that resource is done, `label_handle` looks for the sub-resource with the label, and gives the use its handle.
A label that no sub-resource holds gets a new record, with a spawned entity, `IDEATEK_LOAD_STATE_FAILED` and `IDEATEK_RESOURCE_ERROR_NO_LABEL`; the handle names it. When `path` itself failed, the handle names the resource of `path`.

## Functions

| Function | Result |
| --- | --- |
| `labels` | The observer of `ideatek:ResourceLabel`. |
| `find_label` | The sub-resource of a parent and a label, or `RES_NONE`. |
| `place_labels` | Attaches `ideatek:Resource` to the new sub-resources. |
| `label_handle` | Gives a use of `path#label` its handle, when its parent is done. |
| `mark_labels` | Marks the gone labels of a reloaded resource. |

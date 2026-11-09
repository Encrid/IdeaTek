# src/kernel/resources_files/resources_labels.c

This file holds the observer `labels`, which records the sub-resources.

## The labels

A loader spawns 1 entity for each sub-resource, with `ideatek:ResourceLabel` and its component. The observer takes a record for it: the entity, the parent, the path `path#label` and the type of the parent. The record is a resource with a parent, counted by `max_labels`.
A label above `max_labels` gets no record and the line `no room for the label <label>`: `docs/intern/kernel/resources_files/resources_label_records.c.md`.
During a reload, the loader attaches `ideatek:ResourceLabel` again on each label that stays, and the observer marks the record as seen.
The handles of the loads of `path#label` are in `docs/intern/kernel/resources_files/resources_handles.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `new_label` | Takes and binds the record of a sub-resource, or writes the line. |
| `hear_label` | Records 1 label that the observer heard. |
| `labels` | The observer of `ideatek:ResourceLabel`. |

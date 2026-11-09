# src/kernel/resources_files/resources_label_records.c

This file takes the record of a sub-resource.

## The record of a label

`label_path` writes `path#label` from the path of the parent and the label.
`label_record` counts the sub-resources, and refuses one above `max_labels`. Else it takes a record with that path, the type of the parent and no settings.

## Functions

| Function | Result |
| --- | --- |
| `count_labels` | How many sub-resources the table holds. |
| `label_path` | The path of a sub-resource. |
| `label_record` | A record for a sub-resource, or `RES_NONE`. |

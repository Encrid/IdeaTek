# src/kernel/resources_files/resources_order.c

This file starts the waiting reads of files, by priority.

## The order

`best_waiting` gives the resource at `STEP_READ` with the highest priority. 2 resources with 1 priority come in the order of their records.
`start_reads` starts the best waiting read, then the next, while a read record is free: `docs/intern/kernel/resources_files/resources_queue.c.md`. A started resource is at `STEP_READING`.

## Functions

| Function | Result |
| --- | --- |
| `best_waiting` | The waiting resource with the highest priority, or `RES_NONE`. |
| `start_reads` | Starts the waiting reads by priority. |

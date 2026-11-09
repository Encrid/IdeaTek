# src/kernel/resources_files/resources_queue.c

This file takes a read record and sends 1 read.

## A read

`start_read` takes a free record with `free_read`, fills it with `fill_read`, then `send_read` sends it.
`fill_read` sets the kind, the offset and the length. A meta file or a whole file reads from 0 with no length. The first read of a `ranged` loader asks `head_bytes`. A range asks its own offset and length.

`send_read` writes the path of the file: `docs/intern/kernel/resources_files/resources_reads.c.md`. A path whose variable has no value fails the resource with `IDEATEK_RESOURCE_ERROR_NOT_FOUND`. Then it spawns the entity of the read, and asks the files module or the source module.
With no free record, or a refused spawn, the read does not start: the resource waits for a later round.

## Functions

| Function | Result |
| --- | --- |
| `free_read` | A free record of the reads, or `RES_NONE`. |
| `fill_read` | Fills a record for a resource and a kind. |
| `send_read` | Spawns and asks 1 read. `false` when it cannot start. |
| `start_read` | Starts 1 read of a kind for a resource. `false` when no read can start. |

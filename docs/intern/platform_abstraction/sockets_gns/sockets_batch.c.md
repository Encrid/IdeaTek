# src/platform_abstraction/sockets_gns/sockets_batch.c

This file holds the 3 helpers of the batches. They read a batch component, attach a batch of the half, and write the line of a malformed batch.

## The batches

`batch_of` gives the `ideatek_batch_t` of a component of an entity, from the share. The observers of the batches out, and the observer of the lanes, call it.

`attach_batch` attaches a batch of the current side of the half to the sockets entity, from a start to the bytes used. A batch with no record does nothing.
The module checks no room in `changes` before an attach. A round that attaches more than `changes` of the deployment fails, with the line of Bridge. The readme of the part gives the room that a round needs.

`say_malformed` writes the line of a malformed batch, with the offset of the record that does not fit. The caller leaves the rest of the batch.

## The branches

| Case | Result |
| --- | --- |
| A batch of the half with no record. | No attach. |
| A batch with a record. | An attach to the sockets entity. |

## Functions

| Function | Result |
| --- | --- |
| `batch_of` | Gives the batch component of an entity. |
| `attach_batch` | Attaches a batch of the half to the sockets entity, when it holds a record. |
| `say_malformed` | Writes the line of a malformed batch. |

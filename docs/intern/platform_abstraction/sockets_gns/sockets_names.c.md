# src/platform_abstraction/sockets_gns/sockets_names.c

This file runs the observer of `ideatek:AddressRequest`, and publishes the results of the names.

## The request

The observer `address` gets each write of `ideatek:AddressRequest`, under `lock`:

1. `free_name` finds a free slot of `names`. With none, `queue_name` says the bound of the names, and attaches `ideatek:AddressResult` with `IDEATEK_ADDRESS_STATUS_FAILED` at once. The entity goes in a later round.
2. `copy_name` copies the name from the share of the game, up to `SOCKETS_NAME` bytes with its zero. A name of 0 bytes or too long writes `the request of <id>: name of <count> bytes is outside 1 to 255`. A name outside the arena writes the same line, with `the arena` as its range.
3. A name that fits takes the slot in the state `SOCKETS_WAITING`. A name that does not fit takes it as done, with `IDEATEK_ADDRESS_STATUS_FAILED`. The round publishes it like the others.

After its triggers, the observer wakes the worker thread once: `docs/intern/platform_abstraction/sockets_gns/sockets_resolve.c.md`.

## The results

`publish_names` runs in each round, under `lock`. For each done slot with room in the half, it writes the addresses into the half, and attaches `ideatek:AddressResult` to the entity. Then it puts the entity in `gone`, and frees the slot.
A result with no room in the half waits for the next round.

## The branches

| Case | Result |
| --- | --- |
| No free slot. | Its line, and `IDEATEK_ADDRESS_STATUS_FAILED` at once. |
| A name that does not fit. | Its line, and `IDEATEK_ADDRESS_STATUS_FAILED` in the next round. |
| A name that fits. | The thread looks it up, and a later round publishes the result. |
| No room for a result. | It waits for the next round. |

## Functions

| Function | Result |
| --- | --- |
| `free_name` | Gives the index of a free slot of the names, or `max_addresses`. |
| `copy_name` | Copies the name of a request into a slot. Gives `false` with its line when the name does not fit. |
| `queue_name` | Takes a slot for a request, or fails the request at once. |
| `address` | Queues the names of the triggers under the lock, and wakes the thread once. |
| `publish_names` | Attaches the result of each done name that fits, and frees its slot. |

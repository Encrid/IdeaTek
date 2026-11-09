# src/platform_abstraction/sockets_gns/sockets_names.c

This file runs the observer of `ideatek:AddressRequest`, the worker thread that asks the OS, and the results of the names.

## The request

The observer `address` gets each write of `ideatek:AddressRequest`:

1. `copy_name` copies the name from the share of the game, up to `SOCKETS_NAME` bytes with its zero. A name that does not fit, or that is outside the arena, gets `IDEATEK_ADDRESS_STATUS_FAILED` at once, with the line `the request of <id>: name of <count> bytes is outside <range>`.
2. With no free slot of `names`, the request gets `IDEATEK_ADDRESS_STATUS_FAILED` at once, with the line of the bound.
3. Else the name takes a slot under `lock`, in the state `SOCKETS_WAITING`. The observer wakes the thread once after its triggers.

A result at once is a slot in the state `SOCKETS_DONE` with its status: the round publishes it like the others.

## The thread

`names_loop` runs until `stop` is set. Under `lock`, it takes the first waiting name in the order of the table, and marks it running. It asks the OS outside the lock.

`resolve` calls `getaddrinfo` with no service, for any family, over UDP. It writes each address of the answer as `ideatek_address_t` with the port 0, up to `SOCKETS_ADDRESSES`: an IPv4 address goes to its IPv6 form `::ffff:a.b.c.d`.
No address gives `IDEATEK_ADDRESS_STATUS_NOT_FOUND`. An error of the OS gives `IDEATEK_ADDRESS_STATUS_FAILED`. 1 address or more gives `IDEATEK_ADDRESS_STATUS_OK`.

On Windows, the thread uses the Winsock that GameNetworkingSockets started, so it starts after GameNetworkingSockets does, in the first round: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`. With no name, the thread sleeps on `wake`, at most `SOCKETS_WAIT` nanoseconds.
The thread calls no function of Bridge.

## The results

`publish_names` runs in each round, under `lock`. For each done slot, it writes the addresses into the half, and attaches `ideatek:AddressResult` to the entity. Then it puts the entity in `gone`, and frees the slot.
A result with no room in `changes` or in the half waits for the next round.

## The branches

| Case | Result |
| --- | --- |
| A name that does not fit. | Its line, and `IDEATEK_ADDRESS_STATUS_FAILED` in the next round. |
| No free slot. | Its line, and `IDEATEK_ADDRESS_STATUS_FAILED` in the next round. |
| No name waits. | The thread sleeps until the event or `SOCKETS_WAIT`. |
| A name with addresses. | `IDEATEK_ADDRESS_STATUS_OK` and the addresses, in a later round. |
| A name with no address. | `IDEATEK_ADDRESS_STATUS_NOT_FOUND`. |
| An error of the OS. | `IDEATEK_ADDRESS_STATUS_FAILED`. |
| No room for a result. | It waits for the next round. |
| `stop` is set. | The loop ends. |

## Functions

| Function | Result |
| --- | --- |
| `copy_name` | Copies the name of a request into a slot. Gives `false` with its line when the name does not fit. |
| `address` | Queues the names of the triggers under the lock, and wakes the thread once. |
| `resolve` | Asks the OS for the addresses of a name, and fills the slot with its status. |
| `names_loop` | Runs the names until `stop`. |
| `publish_names` | Attaches the result of each done name that fits. |

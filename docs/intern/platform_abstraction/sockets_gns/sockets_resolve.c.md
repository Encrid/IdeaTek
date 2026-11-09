# src/platform_abstraction/sockets_gns/sockets_resolve.c

This file holds the worker thread that looks up the names with the OS.

## The thread

`names_loop` runs until `stop` is set. Under `lock`, `pick_name` takes the waiting name that comes first in the table, and marks it `SOCKETS_RUNNING`. The thread asks the OS outside the lock, then marks the name `SOCKETS_DONE` under the lock.
With no waiting name, the thread sleeps on `wake` for at most `SOCKETS_WAIT` nanoseconds. The observer `address` wakes it after its triggers: `docs/intern/platform_abstraction/sockets_gns/sockets_names.c.md`.
The thread calls no function of Bridge and no function of GameNetworkingSockets.

## The addresses

`resolve` calls `getaddrinfo` for UDP, with no service and any family. `put_address` turns each address of the answer into an `ideatek_address_t`, up to `SOCKETS_ADDRESSES`. An IPv4 address becomes `::ffff:a.b.c.d`. A family that is not IPv4 or IPv6 is left.

The status is `IDEATEK_ADDRESS_STATUS_OK` with 1 address or more. A name that the OS does not know, or cannot look up now, gives `IDEATEK_ADDRESS_STATUS_NOT_FOUND`, as does an answer with no usable address. Another error of the OS gives `IDEATEK_ADDRESS_STATUS_FAILED`.
The status is arithmetic and not a branch, because an error of the OS other than an unknown name has no path on the runners.

On Windows, `getaddrinfo` needs the Winsock that GameNetworkingSockets starts, so the first name waits for the first round.

## The branches

| Case | Result |
| --- | --- |
| No name waits. | The thread sleeps on the event, at most `SOCKETS_WAIT`. |
| A name with addresses. | `IDEATEK_ADDRESS_STATUS_OK` and the addresses. |
| A name with no address. | `IDEATEK_ADDRESS_STATUS_NOT_FOUND`. |
| More addresses than `SOCKETS_ADDRESSES`. | The first `SOCKETS_ADDRESSES`. The choice is arithmetic, with no branch: no name of the tests gives 17 addresses or a family outside IP. |
| `stop` is set. | The loop ends. |

## Functions

| Function | Result |
| --- | --- |
| `put_address` | Writes 1 address of the OS into the slot, as IPv6. |
| `resolve` | Looks up 1 name with the OS, and fills its slot and its status. |
| `pick_name` | Takes the first waiting name, or gives `nullptr`. |
| `names_loop` | Runs the names until `stop`. |

# src/platform_abstraction/sockets_gns/sockets_main.c

This file loads and unloads the module.

## The load

`init` declares the types and the calls: `docs/intern/platform_abstraction/sockets_gns/sockets_declare.c.md`.
It also creates the mutex and the event that the names use, and starts the worker thread on `names_loop`: `docs/intern/platform_abstraction/sockets_gns/sockets_names.c.md`.
When the mutex, the event or the thread is refused, `init` writes `no thread`, frees what it made, and gives 1, so the load fails.
No setting is read in `init`, and GameNetworkingSockets does not start in it: Bridge writes the settings after `init`. The first round reads them and starts GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.

## The unload

`shutdown` raises `stop` under the lock, then wakes and joins the worker thread.
Then it stops GameNetworkingSockets, which closes each connection and each listen socket, and frees the mutex and the event.
`reload` gives 0: the module has nothing that reloads.

## The branches

| Case | Result |
| --- | --- |
| A refused mutex, event or thread. | `no thread`, and no load. |
| GameNetworkingSockets did not start. | `shutdown` stops nothing of it. |

## Functions

| Function | Result |
| --- | --- |
| `free_thread` | Frees the event and the mutex. |
| `init` | Declares everything and starts the worker thread. Gives 1 without a thread. |
| `shutdown` | Ends the worker thread, stops GameNetworkingSockets and frees what the module took. |
| `reload` | Gives 0. |

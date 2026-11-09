# src/platform_abstraction/sockets_gns/sockets_main.c

This file loads and unloads the module, and holds its state.

## The load

`init` keeps the api of Bridge in `sockets_module.api`, for the callbacks of GameNetworkingSockets, which get no api: `docs/intern/platform_abstraction/sockets_gns/sockets_status.c.md`.
It declares the types and the calls: `docs/intern/platform_abstraction/sockets_gns/sockets_declare.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_runs.c.md`.
It also creates the mutex and the event that the names use, and starts the worker thread on `names_loop`: `docs/intern/platform_abstraction/sockets_gns/sockets_resolve.c.md`.
When the mutex, the event or the thread is refused, `init` writes `no thread`, frees what it made, and gives 1, so the load fails.
No setting is read in `init`, and GameNetworkingSockets does not start in it: Bridge writes the settings after `init`. The first round reads them and starts GameNetworkingSockets: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.

## The unload

`shutdown` raises `stop` under the lock, then wakes and joins the worker thread.
Then `close_listens` closes each listen socket, and GameNetworkingSockets stops, which closes each connection. At the end, `shutdown` frees the mutex and the event.
`reload` gives 0: the module has nothing that reloads.

## The branches

| Case | Result |
| --- | --- |
| A refused mutex, event or thread. | `no thread`, and no load. |
| GameNetworkingSockets did not start. | `shutdown` stops nothing of it. |

## Functions

| Function | Result |
| --- | --- |
| `close_listens` | Closes each listen socket of the table. |
| `free_thread` | Frees the event and the mutex. |
| `init` | Declares everything and starts the worker thread. Gives 1 without a thread. |
| `shutdown` | Ends the worker thread, stops GameNetworkingSockets and frees what the module took. |
| `reload` | Gives 0. |

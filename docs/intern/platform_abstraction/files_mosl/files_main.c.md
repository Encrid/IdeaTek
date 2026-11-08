# src/platform_abstraction/files_mosl/files_main.c

This file loads and unloads the module.

## The load

`init` declares the types and the calls: `docs/intern/platform_abstraction/files_mosl/files_declare.c.md`.
Then it makes the mutex and the event of the jobs, and starts the worker thread on `work_loop`: `docs/intern/platform_abstraction/files_mosl/files_work.c.md`.
A refused mutex, event or thread writes `no thread`, and `init` gives 1: the load fails.
No setting is read in `init`: Bridge writes them after it. The first round reads them: `docs/intern/platform_abstraction/files_mosl/files_start.c.md`.

## The unload

`shutdown` sets `stop`, wakes the worker thread and joins it. Then it stops each watch, frees the bytes of each job that is done, and frees the mutex and the event.
`reload` gives 0, because nothing of the module can reload.

## The branches

| Case | Result |
| --- | --- |
| The system refuses the thread. | The line of the thread, and the load fails. |
| A job is done at the unload. | Its bytes go back to the heap. |

## Functions

| Function | Result |
| --- | --- |
| `init` | Declares the types and the calls, and starts the worker thread. Gives 1 when the thread does not start. |
| `shutdown` | Ends the worker thread, stops the watches and frees what the module took. |
| `reload` | Gives 0. |

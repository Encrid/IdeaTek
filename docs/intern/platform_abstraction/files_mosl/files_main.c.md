# src/platform_abstraction/files_mosl/files_main.c

This file loads and unloads the module.

## The load

`init` declares the types and the calls: `docs/intern/platform_abstraction/files_mosl/files_declare.c.md`.
Then it makes the mutex, the event `wake` of the jobs and the event `finished` of their ends.
It starts the worker thread on `work_loop`: `docs/intern/platform_abstraction/files_mosl/files_work.c.md`.
A refused mutex, event or thread writes `no thread`, frees what was made, and `init` gives 1: the load fails.
No setting is read in `init`: Bridge writes them after it. The first round reads them: `docs/intern/platform_abstraction/files_mosl/files_start.c.md`.

## The unload

`shutdown` sets `stop` under the lock, wakes the worker thread and joins it. Then it stops each watch, frees the bytes of each job, and frees the mutex and the 2 events.
`reload` gives 0, because nothing of the module can reload.

## The branches

| Case | Result |
| --- | --- |
| The system refuses the mutex, the event or the thread. | The line of the thread, and the load fails. |
| A job holds bytes at the unload. | Its bytes go back to the heap. |

## Functions

| Function | Result |
| --- | --- |
| `free_thread` | Frees the 2 events and the mutex. |
| `init` | Declares the types and the calls, and starts the worker thread. Gives 1 when the thread does not start. |
| `free_jobs` | Frees the bytes of each job. |
| `shutdown` | Ends the worker thread, stops the watches and frees what the module took. |
| `reload` | Gives 0. |

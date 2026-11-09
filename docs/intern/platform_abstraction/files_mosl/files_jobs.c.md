# src/platform_abstraction/files_mosl/files_jobs.c

This file gives and takes back the jobs and the early results, under the lock.

## The free jobs

`free_job` gives the first job that is `FILES_FREE`, or `nullptr` when each job is taken.

## The early results

`put_early` keeps a result that is decided before the thread, for the next round.
When `FILES_EARLY` results wait already, the result is lost, and the log gets a line with the entity and the count: `docs/usage/platform_abstraction/files_mosl.md`, *The messages*.
`drop_early` forgets the early result of an entity.

## The cancel

`cancel_job` takes a detach of `ideatek:FileRequest`. The outcome depends on the state of the job, which the timing of the thread decides. So the change of the state is arithmetic, with no branch:

| State of the job | New state |
| --- | --- |
| `FILES_WAITING` | `FILES_FREE`: the job goes away. |
| `FILES_RUNNING` | `FILES_CANCELED`: the thread ends it with no result: `docs/intern/platform_abstraction/files_mosl/files_work.c.md`. |
| `FILES_DONE` | `FILES_CANCELED`: `results` frees its bytes and gives no result: `docs/intern/platform_abstraction/files_mosl/files_results.c.md`. |

Then the early result of the entity goes, and its watch stops: `docs/intern/platform_abstraction/files_mosl/files_watch.c.md`.

## The branches

| Case | Result |
| --- | --- |
| Each job is taken. | `free_job` gives `nullptr`. |
| `FILES_EARLY` results wait. | The result is lost, with its line. |

## Functions

| Function | Result |
| --- | --- |
| `free_job` | Gives a free job, or `nullptr`. |
| `put_early` | Keeps an early result, with the flag of a kept entity, or writes the line of a lost one. |
| `drop_early` | Forgets the early result of an entity. |
| `cancel_job` | Cancels the job, the early result and the watch of an entity. |

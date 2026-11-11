# src/platform_abstraction/files_mosl/files_request.c

This file takes the requests of the game, and queues them for the worker thread.

## The request

The observer `request` gets each write and each detach of `ideatek:FileRequest`:

1. A detach cancels the job of the entity: `docs/intern/platform_abstraction/files_mosl/files_jobs.c.md`.
2. A write copies the path of the request. Then `judge` gives it a status before it runs, from the table below.
3. A watch that `judge` allows starts in the observer: `docs/intern/platform_abstraction/files_mosl/files_watch.c.md`. Its status is the status of the start.
4. Each other request that `judge` allows takes a free job, with the round of its arrival. Its `due` gives the round of its result, and its `seq` gives its order:`docs/intern/platform_abstraction/files_mosl/files_due.c.md`. The thread does it, and `results` publishes it when the thread is done.
5. Each other status is an early result: `results` publishes it in the next round, or in its round with `deterministic_rounds`.

The observer holds `lock` for all the triggers of a barrier, then wakes the thread once. So the thread sees the requests of a round together, and takes them in the order of their priorities.

| Request | Status |
| --- | --- |
| A path of `FILES_PATH` bytes or more, or a path that the rule refuses. | `IDEATEK_FILE_STATUS_DENIED` |
| A storage or an action outside its enum. | `IDEATEK_FILE_STATUS_DENIED` |
| A write or a removal in the content storage. | `IDEATEK_FILE_STATUS_DENIED` |
| A watch in the user storage. | `IDEATEK_FILE_STATUS_DENIED` |
| A request of the content storage, and no content folder. | `IDEATEK_FILE_STATUS_NOT_FOUND` |
| A request of the user storage, and no user folder. | `IDEATEK_FILE_STATUS_UNSUPPORTED` |
| No free job. | `IDEATEK_FILE_STATUS_FAILED` |

## The branches

| Case | Result |
| --- | --- |
| A detach. | The job of the entity goes: `files_jobs.c.md`. |
| A refused request. | Its status in the next round. |
| A watch. | The status of its start in the next round. |
| A request that the thread does. | Its status when the thread is done. |
| No free job. | `IDEATEK_FILE_STATUS_FAILED` in the next round. |

## Functions

| Function | Result |
| --- | --- |
| `copy_path` | Copies the text of the path of a request. Gives `false` when the path does not fit. |
| `judge` | Gives the status of a request before it runs, or `IDEATEK_FILE_STATUS_OK` for a request that the thread does. |
| `fill_job` | Writes a request into a free job, which then waits. |
| `queue_job` | Judges a request, and starts its watch, fills its job or keeps its early result. |
| `request` | Reads the triggers of the observer under the lock, then wakes the thread. |

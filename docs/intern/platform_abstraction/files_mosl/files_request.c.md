# src/platform_abstraction/files_mosl/files_request.c

This file takes the requests of the game, and queues them for the worker thread.

## The request

The observer `request` gets each write and each detach of `ideatek:FileRequest`:

1. A detach cancels the job of the entity. A waiting job goes away, a running job ends with no result, and a watch stops.
2. A write copies the value and its path into a new job, with the round of its arrival.
3. `judge` gives the job a status before it runs, from the table below. A refused job is done at once, with no work of the thread.
4. A job that is not refused waits for the thread; `results` publishes it when the thread is done.

The observer holds `lock` for all the triggers of a barrier, then wakes the thread once. So the thread sees the requests of a round together, and takes them in the order of their priorities.

| Request | Status |
| --- | --- |
| A path that the rule refuses. | `IDEATEK_FILE_STATUS_DENIED` |
| A write or a removal in the content storage. | `IDEATEK_FILE_STATUS_DENIED` |
| A watch in the user storage. | `IDEATEK_FILE_STATUS_DENIED` |
| A request of the content storage, and no content folder. | `IDEATEK_FILE_STATUS_NOT_FOUND` |
| A request of the user storage, and no user folder. | `IDEATEK_FILE_STATUS_UNSUPPORTED` |
| An action outside the enum. | `IDEATEK_FILE_STATUS_DENIED` |
| No room in `jobs`. | `IDEATEK_FILE_STATUS_FAILED` |

A watch that `judge` allows starts in the observer: `docs/intern/platform_abstraction/files_mosl/files_watch.c.md`. Its job is done at once, with the status of the start.

## The branches

| Case | Result |
| --- | --- |
| A detach of a request that waits. | No result. |
| A detach of a request that runs. | The thread ends it, and no result comes. |
| A refused request. | Its status in the next round. |
| A request that the thread does. | Its status when the thread is done. |

## Functions

| Function | Result |
| --- | --- |
| `cancel_job` | Cancels the job of an entity, and stops its watch. |
| `copy_path` | Copies the text of the path of a request into a job. Gives `false` when the path does not fit. |
| `judge` | Gives the status of a request before it runs, or `IDEATEK_FILE_STATUS_OK` for a request that the thread does. |
| `queue_job` | Makes the job of a request, judges it, and wakes the thread or starts the watch. |
| `request` | Reads the triggers of the observer. |

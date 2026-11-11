# src/platform_abstraction/files_mosl/files_inside.h

This header holds the state of the files module, the slots of its types, its limits and the shared names of its sources.

## The state

The state is 1 global variable, `files_module`, as in the other modules. The rule G4 of the style has its exception for it.
The worker thread and the thread of the phases share `jobs`, `done`, `early`, `watches` and `stop`, under `lock`. Each other field belongs to the thread of the phases.

| Field | Value |
| --- | --- |
| `started`, `broken` | The first round read the settings. A wrong setting breaks the module. |
| `stop` | The flag that ends the worker thread. |
| `has_content`, `has_user` | The storage is there. |
| `thread`, `lock`, `wake` | The worker thread, the mutex of the shared fields, and the event that wakes the thread. |
| `finished` | The event that the worker thread sets at the end of each job, for a round that waits: `docs/intern/platform_abstraction/files_mosl/files_due.c.md`. |
| `jobs` | The requests that the thread does, as `files_job_t`. |
| `done`, `done_count` | The jobs that the thread finished, in the order of their end, as indexes into `jobs`. |
| `early`, `early_count` | The results that the thread of the phases decides, as `files_early_t`: the refusals and the starts of the watches. |
| `watches`, `watch_count` | The watches that run, as `files_watch_t`. |
| `half` | The 2 halves of `read_bytes`, as `files_half_t`: the bytes that the results and the changes of a round hold. |
| `gone`, `gone_count` | The result entities of the last round, which the next round destroys. |
| `content`, `user` | The paths of the 2 storages, in the persistent arena. |
| `round` | The count of the rounds, for the order of the jobs and for `due`. |
| `rounds` | The value of `deterministic_rounds`. |
| `seq` | The count of the jobs that arrived, for the order of their requests. |
| `request_list` | The list of the observer `request`, by its index. |

## The jobs

`files_job_t` is 1 request from its arrival to its result:

| Field | Value |
| --- | --- |
| `entity` | The entity of the request. |
| `request` | The value of `ideatek:FileRequest`. |
| `bytes` | The bytes of a write, in the share of the game. |
| `path` | The path of the request, as a text with a zero at its end, of fewer than `FILES_PATH` bytes. |
| `data`, `data_len` | The bytes of the result, which the worker thread takes from the heap, and `results` frees. |
| `size` | The size of the file of a read. |
| `status` | The status of the result. |
| `state` | `FILES_FREE`, `FILES_WAITING`, `FILES_RUNNING`, `FILES_DONE` or `FILES_CANCELED`. |
| `round` | The round of the arrival, for the order of 1 priority. |
| `due` | The first round of its result: `round` plus `rounds`. |
| `seq` | The place of the request in the order of the arrivals. |

A job is `FILES_FREE` before a request and after its result. A cancel turns a waiting job into a free one, and a running or done job into a canceled one: `docs/intern/platform_abstraction/files_mosl/files_jobs.c.md`.

`files_early_t` is 1 result that the thread of the phases decides: its entity, its status, `keep` and `due`. `keep` is `true` for the start of a watch, whose entity stays. `due` is as in a job.

`files_watch_t` is 1 watch: its entity, its `os_watch_t`, its path in the content storage, and whether the round before attached a change.
`files_half_t` is the 2 halves of a block of bytes: its place, its size, the bytes used and the side of the round. The window module keeps its drops the same way.
`files_action_t` is the function of 1 action of the worker thread: `docs/intern/platform_abstraction/files_mosl/files_work.c.md`.

## The limits

| Constant | Value | Limit |
| --- | --- | --- |
| `FILES_QUEUE` | 256 | The most jobs at a time. A request over it gets `IDEATEK_FILE_STATUS_FAILED`. |
| `FILES_EARLY` | 256 | The most results of the thread of the phases that wait for a round. A result over it is lost, with its line. |
| `FILES_WATCHES` | 16 | The most watches at a time. A watch over it gets `IDEATEK_FILE_STATUS_FAILED`. |
| `FILES_DEPTH` | 64 | The depth of a watch: the folders below the watched folder that it sees. |
| `FILES_STACK` | 262144 | The bytes of the stack of the worker thread. |
| `FILES_WAIT` | 100000000 | The nanoseconds that the worker thread waits for a job before it looks again. |
| `CHANGE_HEAD` | 24 | The bytes of a change in `changes` before its value; `CHANGE_SIZE` and `CHANGE_BYTES` add the value rounded up to 8, as in the window module. |

`FILES_PATH` is in `files_paths.h`, so that the paths program uses it alone. This header includes `files_paths.h`.

## The slots

The slots are, in the order of the declarations: `STORAGE`, `ACTION`, `STATUS`, `REQUEST`, `RESULT`, `CHANGE`, then `SETTINGS`.
`files_settings_t` holds `read_bytes`, `user_folder`, `content_folder` and `deterministic_rounds`, as the table of the contract orders them.

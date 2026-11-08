# src/platform_abstraction/files_mosl/files_inside.h

This header holds the state of the files module, the slots of its types, its limits and the shared names of its sources.

## The state

The state is 1 global variable, `files_module`, as in the other modules. The rule G4 of the style has its exception for it.
The worker thread and the thread of the phases share `jobs`, under `lock`. Each other field belongs to the thread of the phases.

| Field | Value |
| --- | --- |
| `started`, `broken` | The first round read the settings. A wrong setting breaks the module. |
| `thread`, `lock`, `wake`, `stop` | The worker thread, the mutex of the jobs, the event that wakes the thread, and the flag that ends it. |
| `jobs`, `job_count` | The requests that wait, run or are done, as `files_job_t`. |
| `done`, `done_count` | The jobs that the thread finished, in the order of their end, as indexes into `jobs`. |
| `watches`, `watch_count` | The watches that run, as `files_watch_t`. |
| `half` | The 2 halves of `read_bytes`, as `files_half_t`: the bytes that the results and the changes of a round hold. |
| `gone`, `gone_count` | The result entities of the last round, which the next round destroys. |
| `content`, `user` | The paths of the 2 storages, empty when the storage is not there. |
| `request_list` | The list of the observer `request`, by its index. |

## The jobs

`files_job_t` is 1 request from its arrival to its result:

| Field | Value |
| --- | --- |
| `entity` | The entity of the request. |
| `request` | The value of `ideatek:FileRequest`. |
| `path` | The path of the request, as a text with a zero at its end, of at most `FILES_PATH` bytes. |
| `data`, `data_len` | The bytes of the result, which the worker thread takes from the heap, and `results` frees. |
| `size` | The size of the file of a read. |
| `status` | The status of the result. |
| `state` | `FILES_WAITING`, `FILES_RUNNING`, `FILES_DONE` or `FILES_CANCELED`. |
| `round` | The round of the arrival, for the order of 1 priority. |

`files_watch_t` is 1 watch: its entity, its `os_watch_t` and its path in the content storage.
`files_half_t` is the 2 halves of a block of bytes: its place, its size, the bytes used and the side of the round. The window module keeps its drops the same way.

## The limits

| Constant | Value | Limit |
| --- | --- | --- |
| `FILES_QUEUE` | 256 | The most jobs at a time. A request over it gets `IDEATEK_FILE_STATUS_FAILED`. |
| `FILES_WATCHES` | 16 | The most watches at a time. A watch over it gets `IDEATEK_FILE_STATUS_FAILED`. |
| `FILES_PATH` | 4096 | The most bytes of a path with its storage, and of a text of a setting. |
| `FILES_DEPTH` | 64 | The depth of a watch: the folders below the watched folder that it sees. |
| `FILES_STACK` | 262144 | The bytes of the stack of the worker thread. |
| `FILES_WAIT` | 100000000 | The nanoseconds that the worker thread waits for a job before it looks again. |
| `CHANGE_HEAD` | 24 | The bytes of a change in `changes` before its value; `CHANGE_SIZE` and `CHANGE_BYTES` add the value rounded up to 8, as in the window module. |

## The slots

The slots are, in the order of the declarations: `STORAGE`, `ACTION`, `STATUS`, `REQUEST`, `RESULT`, `CHANGE`, then `SETTINGS`.
`files_settings_t` holds `read_bytes`, `user_folder` and `content_folder`, as the table of the contract orders them.

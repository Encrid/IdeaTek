# src/platform_abstraction/files_mosl/files_results.c

This file runs the system `results` in the first phase: it publishes the jobs that are done, and the changes of the watches.

## The round

`results` does these steps:

1. The first round reads the settings and finds the storages: `docs/intern/platform_abstraction/files_mosl/files_start.c.md`. A broken module gives 1.
2. `turn_results` destroys the entities of the results of the last round, except the watches, and turns the halves.
3. Under `lock`, `take_results` takes each done job. `publish` copies its bytes into the half, attaches `ideatek:FileResult` to the entity, and frees the bytes. The entity goes to `gone`, so the next round destroys it.
4. The watches give their changes: `docs/intern/platform_abstraction/files_mosl/files_watch.c.md`.

When the bytes of a result are more than the room left in the half, the status is `IDEATEK_FILE_STATUS_TOO_BIG`, and no bytes come.
A publish that has no room in `changes` leaves the job done for the next round, as the window module leaves a drop.

## The branches

| Case | Result |
| --- | --- |
| A job is done. | Its result, and its entity goes in the next round. |
| The bytes of a result do not fit in the half. | `IDEATEK_FILE_STATUS_TOO_BIG`. |
| No room in `changes`. | The job waits for the next round. |

## Functions

| Function | Result |
| --- | --- |
| `turn_results` | Destroys the result entities of the round before, and swaps the sides of the halves. |
| `put_bytes` | Copies bytes into the side of the round, and gives their text. |
| `publish` | Attaches the result of a done job to its entity. |
| `take_results` | Publishes each job that is done, under the lock. |
| `results` | Runs the steps of the round. Gives 1 when the module is broken. |

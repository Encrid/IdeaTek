# src/platform_abstraction/files_mosl/files_results.c

This file runs the system `results` in the first phase: it publishes the results that are ready, and the changes of the watches.

## The round

`results` does these steps:

1. The first round reads the settings and finds the storages: `docs/intern/platform_abstraction/files_mosl/files_start.c.md`. A broken module gives 1 in each round.
2. `turn_results` destroys the entities of the results of the round before, except the watches, and turns the half: `docs/intern/platform_abstraction/files_mosl/files_half.c.md`.
3. Under `lock`, `take_results` publishes each early result that is due, then the done jobs that are due: `docs/intern/platform_abstraction/files_mosl/files_due.c.md`. `publish` attaches `ideatek:FileResult` to the entity, and puts the entity in `gone`, so the next round destroys it. The entity of a watch that started is kept: the game destroys it. `publish_job` copies the bytes of a job into the half first, and frees them. A canceled job is freed with no result.
4. The watches give their changes: `docs/intern/platform_abstraction/files_mosl/files_watch.c.md`.

When the bytes of a result are more than the room left in the half, the status is `IDEATEK_FILE_STATUS_TOO_BIG`, and no bytes come.
A result whose status is not `IDEATEK_FILE_STATUS_OK` carries no bytes, also when its job wrote some, for example the lines of a list that stopped.
A result that has no room in `changes` waits for the next round, as the window module leaves a drop.

## The branches

| Case | Result |
| --- | --- |
| An early result waits. | Its result, and its entity goes in the next round. |
| A job is done. | Its result, and its entity goes in the next round. |
| A done job was canceled. | Its bytes go back to the heap, and no result comes. |
| The bytes of a result do not fit in the half. | `IDEATEK_FILE_STATUS_TOO_BIG`. |
| No room in `changes`. | The result waits for the next round. |

## Functions

| Function | Result |
| --- | --- |
| `publish` | Attaches a result to its entity, and marks the entity for the next round unless it is kept. |
| `publish_job` | Copies the bytes of a done job into the half, publishes it, and frees it. |
| `take_results` | Publishes the early results that are due and fit in `changes`, then the done jobs, under the lock. |
| `results` | Runs the steps of the round. Gives 1 when the module is broken. |

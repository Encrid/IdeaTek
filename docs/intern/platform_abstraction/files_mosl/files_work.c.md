# src/platform_abstraction/files_mosl/files_work.c

This file runs the worker thread, which does the jobs that wait.

## The loop

`work_loop` runs until `stop` is set:

1. Under `lock`, `pick_job` takes the waiting job with the highest `priority` and the earliest `round`, and marks it as running. The key of a job is its priority, then the rounds left, so 1 comparison picks it.
2. With no job, the thread waits on `wake` for at most `FILES_WAIT` nanoseconds, then looks again.
3. With a job, `run_job` does the action: `docs/intern/platform_abstraction/files_mosl/files_actions.c.md`. The thread calls no function of Bridge.
4. Under `lock`, the job becomes done with its status, or stays canceled when the game detached it while it ran. A done job goes to `done`.

The thread reads the bytes of a write in the share of the game, at the place that the request gives. The game keeps them until the result: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The requests*.

## The branches

| Case | Result |
| --- | --- |
| No job waits. | The thread waits for the event, or for `FILES_WAIT`. |
| 2 jobs with 1 priority. | The one of the earlier round runs first. |
| The game detaches a job that runs. | The job ends with no result. |
| `stop` is set. | The loop ends. |

## Functions

| Function | Result |
| --- | --- |
| `pick_job` | Takes the waiting job that runs next, under the lock, or gives `nullptr`. |
| `run_job` | Does the action of a job, and gives its status. |
| `work_loop` | Runs the jobs until `stop`. |

# src/platform_abstraction/files_mosl/files_work.c

This file runs the worker thread, which does the jobs that wait.

## The loop

`work_loop` runs until `stop` is set:

1. Under `lock`, `pick_job` takes the waiting job with the highest key, and marks it as running. `job_key` gives the key of a job: its priority, then the rounds left, so 1 comparison picks it. A job that does not wait has the key 0.
2. With no job, the thread waits on `wake` for at most `FILES_WAIT` nanoseconds, then looks again.
3. With a job, `run_job` does the action: `docs/intern/platform_abstraction/files_mosl/files_actions.c.md` and `docs/intern/platform_abstraction/files_mosl/files_list.c.md`. The thread calls no function of Bridge.
4. Under `lock`, `finish_job` makes the job done with its status, and puts it in `done`. A job that the game canceled while it ran becomes free, and its bytes go back to the heap. The timing of the cancel is outside the tests, so the change is arithmetic, with no branch.

The thread reads the bytes of a write where the request points into the share of the game. The game keeps them until the result: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The requests*.

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
| `job_key` | Gives the key of a job: its priority, then the rounds left; 0 for a job that does not wait. |
| `pick_job` | Takes the waiting job that runs next, under the lock, or gives `nullptr`. Tells when `stop` is set. |
| `run_job` | Does the action of a job on its storage, and gives its status. |
| `finish_job` | Makes a job done under the lock, or frees a canceled one. |
| `work_loop` | Runs the jobs until `stop`. |

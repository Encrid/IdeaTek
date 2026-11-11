# src/platform_abstraction/files_mosl/files_due.c

This file gives the results in their rounds: `docs/extern/platform_abstraction/ideatek_files.h.md`, *The rounds of a result*.

## The rounds

Each job and each early result has `due`: the round of its request plus `deterministic_rounds`. A result is given in the round `due` or later, never before.
With a `deterministic_rounds` of 0, `due` is the round of the request. So each done job comes in the order of its end, as before.

With a `deterministic_rounds` above 0, `take_jobs` does 2 more steps under the lock:

1. `wait_due` waits for each job that is due and that the worker thread has not ended. It unlocks, waits on the event `finished`, and locks again. It names each wait in 1 line of the log.
2. It sorts the done jobs by `seq`, the order of the requests. So the results of 1 round come in the order of their requests.

Then `take_jobs` gives each done job that is due, while `changes` has room. It keeps the other jobs for a later round.
The wait skips a canceled job, since it gives no result.

## Functions

| Function | Result |
| --- | --- |
| `by_seq` | Compares 2 done jobs by the order of their requests. |
| `wait_due` | Waits for the jobs that are due and not ended, with 1 line for each. |
| `take_jobs` | Gives the done jobs that are due, in their order, while `changes` has room. |
| `take_rounds` | Checks `deterministic_rounds` and keeps it. `false` with its line when it is outside its range. |

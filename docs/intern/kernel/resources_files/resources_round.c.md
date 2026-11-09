# src/kernel/resources_files/resources_round.c

This file is the system `round` of the module, in the first phase.

## The round

1. The first round starts the module: `docs/intern/kernel/resources_files/resources_start.c.md`. When the start fails, each round gives 1.
2. `turn_tags` detaches the tags of the round before.
3. `tick_time` moves the time.
4. `take_results` applies the results of the reads that came.
5. `place_uses` and `place_labels` give the new uses and the new sub-resources their records.
6. `start_reloads` starts the reloads that are due.
7. `step_resource` moves each resource that waits, then `start_reads` and `start_ranges` start the reads by priority.
8. `finish_loads` ends the loads whose loader answered.
9. `check_owners` and `free_dead` free what nothing keeps.
10. In a dirty round, `fold_states` folds the states and writes the views.

The changes of the round apply at the barrier of the phase, so the game reads 1 state of each resource in a round.

## Functions

| Function | Result |
| --- | --- |
| `step_all` | Walks the resources that wait, for step 7. |
| `round` | Runs the 10 steps. Gives 1 when the module is broken. |

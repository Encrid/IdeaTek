# src/platform_abstraction/input_sdl3/input_runs.c

This file joins the motions that follow each other into 1 event, for the mouse and for the fingers.
`merge_motion` turns it on: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The settings of the input module*.

## A run

A run is 1 event of motion that can grow: its slot, its value, its key, and its entity.
The key of a run of the mouse is its mask of buttons. The key of a run of a finger is the id of the finger.
The module has at most 1 run at a time, in `input_module.run`.

`joins_run` tells if a motion joins the run. It joins when `merge_motion` is `true`, a run is pending, and the run has the same slot and key.
A motion that does not join the run ends it with `flush_run`, then starts a new run with `start_run`.

`start_run` spawns the entity of the run at once with `spawn_entity`, and keeps the value and the key.
The rank of the run is the rank of its first motion, from `next_order`.
Thus the run keeps its room in the round, and its place among the other events.

`flush_run` attaches the value of the run to its entity, keeps the entity in `items`, and ends the run.
`take_events` calls it before each event that does not join the run, and at the end of the take: `docs/intern/platform_abstraction/input_sdl3/input_take.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A motion with no pending run. | A new run. |
| A motion with the slot and the key of the run, with `merge_motion` at `true`. | It joins the run. |
| A motion with another key. | The run ends, and a new run starts. |
| `merge_motion` at `false`. | Each motion is a run of 1 motion. |
| An event that is not a motion. | The run ends before it. |
| The end of the take. | The pending run ends. |
| A motion of the mouse after a motion of a finger. | The run of the finger ends, and a new run starts. |

## Functions

| Function | Result |
| --- | --- |
| `joins_run` | Tells if a motion joins the pending run. |
| `start_run` | Ends the pending run, and starts a run with a new entity. |
| `flush_run` | Attaches the value of the pending run to its entity, and ends the run. |

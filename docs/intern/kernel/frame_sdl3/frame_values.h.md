# src/kernel/frame_sdl3/frame_values.h

This header gives the types and the functions of the values of a round, for the module and for the program that tests them.

## The types

`frame_clock_t` holds the time between the rounds: `pending`, `elapsed`, `real_elapsed` and `index`.
`frame_rule_t` holds the values of the round: `scale`, `rate`, `most_steps` and `paused`.

## Functions

| Function | Result |
| --- | --- |
| `start_round` | The values of 1 round: `docs/intern/kernel/frame_sdl3/frame_values.c.md`. |
| `smoothed` | The time of a refresh rate, when the real time is near one. |

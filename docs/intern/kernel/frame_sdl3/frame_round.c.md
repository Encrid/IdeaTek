# src/kernel/frame_sdl3/frame_round.c

This file runs the system `round` in the first phase.

## The round

`round` does these steps:

1. The first round reads the settings: `docs/intern/kernel/frame_sdl3/frame_settings.c.md`. A broken module gives 1.
2. It applies the request: `docs/intern/kernel/frame_sdl3/frame_request.c.md`.
3. With `sleeper` at `clock` and a `max_rate`, it waits until `1 / max_rate` passed since the last round, with `SDL_DelayPrecise`. The first round does not wait.
4. It reads the real time: `SDL_GetTicksNS` since the start of the last round, or `fixed_round` with `time_source` at `fixed`. The first round reads 0.
5. With `smoothing` at `refresh`, the time goes through `smoothed`: `docs/intern/kernel/frame_sdl3/frame_values.c.md`.
6. It computes the values with `start_round`. The runs of the fixed phase start at 0.

Then it writes `ideatek:Frame` and `ideatek:Tick`.
With `sleeper` at `events` and a window, it writes `ideatek:EventWait` on the window: `1 / max_rate`, or 0 with no limit. The first write attaches it.

## The branches

| Case | Result |
| --- | --- |
| A broken module. | 1 in each round. |
| `sleeper` at `clock` with a limit. | A wait before the read of the time. |
| `sleeper` at `events` with a window. | The wait of the window. |
| `time_source` at `fixed`. | `fixed_round` in each round after the first. |

## Functions

| Function | Result |
| --- | --- |
| `wait_turn` | Waits for the turn of the round with `sleeper` at `clock`, and keeps the start of the round. |
| `read_real` | Gives the real time of the round. |
| `write_wait` | Writes `ideatek:EventWait` of the window. |
| `round` | Runs the steps of the round. Gives 1 when the module is broken. |

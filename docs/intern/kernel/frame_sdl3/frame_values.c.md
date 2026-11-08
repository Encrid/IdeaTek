# src/kernel/frame_sdl3/frame_values.c

This file computes the values of a round, with no call of Bridge or of SDL3, so a program tests it alone.

## The values

`start_round` follows the pseudo code of `docs/extern/kernel/ideatek_frame.h.md`, *The values of a round*, with no branch:

1. `delta` is `real` times `scale`, or 0 in a pause.
2. The pending time grows by `delta`. The steps are the whole part of the pending time divided by `step`.
3. More steps than `most_steps` cut `delta` by the time of the other steps, and the steps become `most_steps`.
4. The pending time loses the time of the steps. `index`, `real_elapsed` and `elapsed` grow.
5. It fills `ideatek:Frame`, and `step`, `steps` and `fraction` of `ideatek:Tick`.

It leaves `tick` and the `elapsed` of `ideatek:Tick` to the fixed phase: `docs/intern/kernel/frame_sdl3/frame_step.c.md`.
The count of the steps is a `double` until it is cut to `most_steps`, so a large scale gives no overflow of an integer.

## The smoothing

`smoothed` gives the time of the row of `REFRESH` whose range holds `real`, else `real`.
The 9 rows are the table of `docs/extern/kernel/ideatek_frame.h.md`, *The smoothing*.

## The branches

| Case | Result |
| --- | --- |
| A pause. | `delta` 0, and `index` grows. |
| More steps than `most_steps`. | `most_steps` steps, and a smaller `delta`. |
| `real` in the range of a row. | The time of the row. |
| `real` in no range. | `real`. |

## Functions

| Function | Result |
| --- | --- |
| `start_round` | Advances the clock by a round, and fills the frame and the steps of the tick. |
| `smoothed` | Gives the time of a refresh rate near `real`, else `real`. |

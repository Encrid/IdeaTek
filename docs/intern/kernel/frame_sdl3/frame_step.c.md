# src/kernel/frame_sdl3/frame_step.c

This file runs the system `step` in the fixed phase.

## The step

Bridge applies a write at the barrier of a run, so the readers of a run see the `ideatek:Tick` that the run before wrote.
`round` writes the first step of the round: `docs/intern/kernel/frame_sdl3/frame_round.c.md`. Each run of `step` counts itself.
While a next run exists, `step` writes the next step, `tick` plus 1 and `elapsed` plus `step`, and gives `BRIDGE_AGAIN`. The last run gives 0.

With `steps` 0, the 1 run does nothing and gives 0. The readers see `steps` 0 and do nothing.
The host runs the phase again on `BRIDGE_AGAIN`, up to the cap of `again`: https://github.com/Encrid/Bridge-Host/blob/v0.5.0/readme.md.

## The branches

| Case | Result |
| --- | --- |
| A run with a next run. | The next step, and `BRIDGE_AGAIN`. |
| The last run, or `steps` 0. | 0. |

## Functions

| Function | Result |
| --- | --- |
| `step` | Writes the next step while a next run exists, and asks for the phase again. |

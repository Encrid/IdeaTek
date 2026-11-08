# src/kernel/frame_sdl3/frame_step.c

This file runs the system `step` in the fixed phase.

## The step

Each run of the fixed phase below `steps` is a step: `tick` grows by 1, and the `elapsed` of `ideatek:Tick` grows by `step`.
The module writes `ideatek:Tick`, and gives `BRIDGE_AGAIN` while runs remain.
With `steps` 0, the 1 run does no step and gives 0. The readers see `steps` 0 and do nothing.
The host runs the phase again on `BRIDGE_AGAIN`, up to the cap of `again`: https://github.com/Encrid/Bridge-Host/blob/v0.5.0/readme.md.

## The branches

| Case | Result |
| --- | --- |
| A run below `steps`. | A step, and `BRIDGE_AGAIN` when runs remain. |
| The last run, or `steps` 0. | 0. |

## Functions

| Function | Result |
| --- | --- |
| `step` | Does 1 step of the round, and asks for the phase again while steps remain. |

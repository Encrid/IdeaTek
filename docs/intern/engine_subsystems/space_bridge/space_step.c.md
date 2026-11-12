# src/engine_subsystems/space_bridge/space_step.c

This file runs the system `step` of the fixed phase: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:PreviousWorldTransform*.

`step` runs in each pass of the fixed phase. When the tick changed, it writes the place of each row of its query into the place before, with the tick of the last pass. It writes in place, with `remember`: `docs/intern/engine_subsystems/space_bridge/space_previous.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `step_all` | Keeps the place before of each interpolated entity, when `tick` changed. |
| `step` | The system of the fixed phase. Runs `step_all` through `run_started`. |

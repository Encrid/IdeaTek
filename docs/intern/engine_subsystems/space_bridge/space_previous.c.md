# src/engine_subsystems/space_bridge/space_previous.c

This file keeps `ideatek:PreviousWorldTransform`: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:PreviousWorldTransform*.

`previous` hears `ideatek:Interpolated` enter: an entity with a place gets its place before at the place of now and the tick of now. It hears the tag leave, and detaches the place before.
An entity with no place yet gets its place before from `place`: `docs/intern/engine_subsystems/space_bridge/space_one.c.md`.
The system `step` uses `remember` and `get_tick` too: `docs/intern/engine_subsystems/space_bridge/space_step.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `remember` | Sets the place before of 1 entity to its place: an attach when the entity has none, else a write in place. |
| `get_tick` | The tick of `ideatek:Tick`, or 0 with no clock. |
| `previous_all` | Attaches or detaches the place before, for each entity of the list. |
| `previous` | The observer of `ideatek:Interpolated`. Runs `previous_all` through `run_started`. |

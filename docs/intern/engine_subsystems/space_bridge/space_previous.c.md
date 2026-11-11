# src/engine_subsystems/space_bridge/space_previous.c

This file keeps `ideatek:PreviousWorldTransform`: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:PreviousWorldTransform*.

`previous` hears `ideatek:Interpolated` enter: an entity with a place gets its place before at the place of now and the tick of now. It hears the tag leave, and detaches the place before.
An entity with no place yet gets its place before from `place`: `docs/intern/engine_subsystems/space_bridge/space_one.c.md`.
`step` runs in each pass of the fixed phase. When the tick changed, it writes the place of each row of its query into the place before, with the tick of the last pass. It writes in place.

## Functions

| Function | Result |
| --- | --- |
| `remember` | Sets the place before of 1 entity to its place: an attach when the entity has none, else a write in place. |
| `get_tick` | The tick of `ideatek:Tick`, or 0 with no clock. |
| `previous` | The observer of `ideatek:Interpolated`. Gives 0. |
| `step` | The system of the fixed phase. Gives 0. |

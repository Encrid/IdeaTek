# src/engine_subsystems/space_bridge/space_previous.c

This file keeps `ideatek:PreviousWorldTransform`: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:PreviousWorldTransform*.

## The observer

`previous` hears `ideatek:Interpolated` enter: it attaches `ideatek:PreviousWorldTransform` with the place of now and the `tick` of now.
It hears `ideatek:Interpolated` leave: it detaches `ideatek:PreviousWorldTransform`.

## The system

`step` runs first in each pass of the fixed phase, before the movers. When `tick` of `ideatek:Tick` differs from `tick` of `space_module`, it walks its query.
For each row, it attaches `ideatek:PreviousWorldTransform` with the `ideatek:WorldTransform` of the row and the old tick. Then it keeps the new tick.

## Functions

| Function | Result |
| --- | --- |
| `remember` | Attaches the place before of 1 entity. |
| `previous` | The observer of `ideatek:Interpolated`. Gives 0. |
| `step` | The system of the fixed phase. Gives 0. |

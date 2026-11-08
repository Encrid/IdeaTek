# src/kernel/frame_sdl3/frame_declare.c

This file gives Bridge the types, the 2 systems and the observer of the clock module.

## The declarations

The module declares `ideatek:ideatek_entity_t` and the 2 enums `ideatek_window_mode_t` and `ideatek_surface_t` first, because `ideatek:WindowState` names them.
Then it declares `ideatek:WindowState` and `ideatek:EventWait` of the window contract, the 3 types of the frame contract, and its settings.

| Call | Kind | Name |
| --- | --- | --- |
| System | `first` | `round` |
| System | `fixed` | `step` |
| Observer | write and detach | `window` on `ideatek:WindowState` |

`round` states `ideatek:Frame`, `ideatek:Tick` and `ideatek:EventWait` as its changes. `step` states `ideatek:Tick`.

## Functions

| Function | Result |
| --- | --- |
| `declare_types` | Declares the 9 types. |
| `add_runs` | Declares the 2 systems and the observer. |

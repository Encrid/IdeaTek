# src/kernel/frame_sdl3/frame_inside.h

This header holds the state of the clock module, the slots of its types and the shared names of its sources.

## The state

The global variable `frame_module` holds the state, because Bridge passes no argument that holds it. The rule G4 of the style excepts it, as for the other modules.

| Field | Value |
| --- | --- |
| `started`, `broken` | The first round read the settings. A wrong setting breaks the module. |
| `most_steps`, `fixed_round` | The 2 counts of the settings. |
| `fixed`, `events`, `refresh` | The 3 choices of the settings: `time_source` at `fixed`, `sleeper` at `events`, `smoothing` at `refresh`. |
| `rule`, `max_rate` | The values of `ideatek:TimeRequest` that the module applied: the values of a round, and the limit of the rate. |
| `seen` | The last value of `ideatek:TimeRequest` that the module read. |
| `clock` | The time between the rounds, as `frame_clock_t`. |
| `tick` | The value of `ideatek:Tick`. The fixed phase advances its `tick` and its `elapsed`. |
| `run` | The runs of the fixed phase in the round. |
| `last_start` | The time of SDL3 at the start of the last round, in nanoseconds. |
| `window` | The entity of the window, or 0. |
| `window_list` | The index of the list that the observer `window` reads. |

## The names

The slots follow the order of the declarations: `ENTITY`, `WINDOW_MODE`, `SURFACE`, `WINDOW_STATE`, `EVENT_WAIT`, `FRAME`, `TICK`, `TIME_REQUEST` and `SETTINGS`.
`frame_settings_t` is the struct of the settings, in the order of the table of the contract.
`FRAME_TYPE` declares a type of a contract, and `FRAME_TEXT` is the most bytes of a text of a setting in a line.
`docs/intern/kernel/frame_sdl3/frame_values.h.md` gives the types of the values.

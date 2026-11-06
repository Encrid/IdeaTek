# src/platform_abstraction/window_sdl3/window_main.c

This file holds the manifest of the window module, its exports `init`, `shutdown` and `reload`, and the variable `window_module`.
`docs/intern/platform_abstraction/window_sdl3/window_inside.h.md`, *The state of the module*, gives the fields of the variable.

## The start

`bridge_init` does these steps:

1. It starts the video of SDL3 with `sdl3_start_subsystem` and `SDL_INIT_VIDEO`.
2. If SDL3 refuses, it gives 1 after the line of no video.
3. It keeps the row of `find_driver` in `driver`.
4. `enable_events` turns on the types of events that the module takes.
5. `declare_types` declares the types, in the order of their slots.
6. `add_runs` adds the system `events` and the observers `request` and `clipboard`.
7. It gives 0.

`docs/intern/platform_abstraction/window_sdl3/window_events.c.md`, *The ranges of the module*, gives the types of step 4.

Bridge calls no `shutdown` after an `init` that gave 1. The start then holds no subsystem: `docs/extern/platform_abstraction/sdl3_start/readme.md`, *Usage*.
The module does not read its settings in `init`, because Bridge writes them after `init`: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#the-settings-of-a-module.
Thus the first round reads them: `docs/intern/platform_abstraction/window_sdl3/window_items.c.md`, *The first round*.

## The declarations

Each type states the kind, the storage layout and `on_destroy` of its contract: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The declarations*.
`ideatek_window_sdl3:Settings` states `BRIDGE_SINGLETON`.

| Types | `on_unload` | Reason |
| --- | --- | --- |
| `ideatek:Display`, `ideatek:VideoMode`, `ideatek:Drop` and `ideatek:Paste` | `BRIDGE_SWEEP` | The module spawns these entities, and they carry nothing else. |
| Each other type | `BRIDGE_STRIP` | An entity of the game keeps the type, or the type is a singleton. |

## The runs

| Run | Kind | Phase or events | Terms | Changes |
| --- | --- | --- | --- | --- |
| `events` | A system | `first` | None. | `ideatek:WindowState`, `ideatek:Display`, `ideatek:VideoMode`, `ideatek:Drop` and `ideatek:Paste` |
| `request` | An observer | `BRIDGE_ON_ENTER`, `BRIDGE_ON_ATTACH` and `BRIDGE_ON_LEAVE` | `ideatek:WindowRequest`, `BRIDGE_READ` and `BRIDGE_AND` | None. |
| `clipboard` | An observer | `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` | `ideatek:ClipboardRequest`, `BRIDGE_READ` and `BRIDGE_AND` | None. |

`events` has no term. A term of a singleton is optional, and a query of optional terms alone walks each table: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge_abi.h.md#bridge_term_t.
Thus `events` runs 1 time in each run of its phase. It reads the settings through their kept cell, and `ideatek:EventWait` of the window with `get_cell`.

A pair of the deployment orders `events` after the clock module. Other pairs put it before each module that reads the queue of SDL3.
The deployment maps the phase `first` of the module to the first phase of the round.

## The stop

`bridge_shutdown` calls `close_window`: `docs/intern/platform_abstraction/window_sdl3/window_open.c.md`, *The close*.
Then it stops the video with `sdl3_stop_subsystem` and `SDL_INIT_VIDEO`, and gives 0.
The module saves no row, because each of its rows is a fact of the system that the next start reads again.
`bridge_reload` gives 1, because the module is not reloadable.

## The branches

| Case | Result |
| --- | --- |
| SDL3 does not start the video. | The line of no video, and the load fails. |
| SDL3 starts the video. | The load gives 0, and the module has 14 types, 1 system and 2 observers. |
| A reload. | It gives 1. |
| The end of the run. | The window goes, then the video of SDL3. |

## Functions

| Function | Result |
| --- | --- |
| `bridge_init` | Starts the video, declares the types, and adds the runs. Gives 0, or 1 when SDL3 refuses. |
| `declare_types` | Declares the 14 types in the order of their slots. |
| `add_runs` | Adds the system `events` and the observers `request` and `clipboard`. |
| `bridge_shutdown` | Destroys the window, and stops the video. Gives 0. |
| `bridge_reload` | Gives 1. |

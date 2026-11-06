# src/platform_abstraction/sdl3_start/sdl3_start.h

This header gives the 2 functions of the library part `sdl3_start`, which starts and stops a subsystem of SDL3 for a module.
It includes `SDL3/SDL_init.h` for `SDL_InitFlags`, and `stddef.h` for `size_t`.

## The part

The package of the part is `BL-IdeaTek-SDL3-Start`: `docs/extern/platform_abstraction/sdl3_start/readme.md`.
Each module that starts a subsystem of SDL3 links the archive, for example the window module and the input module.
Thus each module holds its own copy of the code, and the shared library of the part `sdl3` holds the one state of SDL3.

The part includes no header of Bridge, because it is a library.
Thus it writes no line in a log. It gives the reason of a failure in a buffer of the caller, and the module writes the line.

The archive compiles with `-fvisibility=hidden`.
Thus the functions stay out of the exports of each module, and 2 modules never call the copy of the other.

## The rules of a caller

`docs/extern/platform_abstraction/sdl3_start/readme.md`, *Usage*, gives the rules of a caller.

## Functions

| Function | Result |
| --- | --- |
| `sdl3_start_subsystem(flags, reason, size)` | Starts the subsystems of `flags`, with no handler of SDL3 for the signals of the system. At the first start, it also turns off the types of events that no module takes. Gives `true`, or `false` with the reason in `reason`. |
| `sdl3_stop_subsystem(flags)` | Stops the subsystems of `flags`. Then it stops SDL3 when no subsystem is left. |

`docs/intern/platform_abstraction/sdl3_start/sdl3_start.c.md` tells the steps of each function.

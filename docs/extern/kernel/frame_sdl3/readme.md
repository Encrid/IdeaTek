# BM-IdeaTek-Frame-SDL3

The clock module of IdeaTek on the timer of SDL3: the time of each round, the fixed steps and the limit of the rate.

## Install

The release of `BM-IdeaTek-Frame-SDL3` has the tag `frame_sdl3-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Frame-SDL3.zip` and `sha256.txt`. `BM-IdeaTek-Frame-SDL3.zip` has a folder for each target.
In `BM-IdeaTek-Frame-SDL3`, each folder holds the module `frame_sdl3.dll`, `frame_sdl3.so` or `frame_sdl3.dylib`.
Each folder of `BM-IdeaTek-Frame-SDL3.zip` also holds this readme and the licence.

Like the window module, it links `BL-IdeaTek-SDL3` by its name, and expects that shared library in its own folder.
So the deployment puts the module in the folder of that library: `docs/extern/platform_abstraction/sdl3/readme.md`.
The module starts no subsystem of SDL3, because the timer of SDL3 needs none.
A project names the zip of `BM-IdeaTek-Frame-SDL3` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_frame_sdl3:clock@0.1.0`.
Its role is the clock module: `docs/extern/kernel/ideatek_frame.h.md`, *The roles*.

The component of its settings is `ideatek_frame_sdl3:Settings`. It holds the settings of `docs/extern/kernel/ideatek_frame.h.md`, *The settings of the clock module*: `most_steps`, `time_source`, `fixed_round`, `sleeper` and `smoothing`. It has no setting beyond those 5.
The real time comes from `SDL_GetTicksNS`. The wait of `sleeper` at `"clock"` comes from `SDL_DelayPrecise`.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The module reads the time and waits on the thread of the phases. |
| `reloadable` | `false` | The time of the last round is not in the storage of Bridge. |
| `spawns` | `"none"` | The module spawns no entity. |
| `changes` | 64 | The module writes its 3 singletons in each round, and `ideatek:EventWait` of the window. |
| `persistent` | `"none"` | The module keeps no bytes of its own. |
| `scratch` | 4096 | Room for the declarations and for the lines of the log. |
| `log_ring` | 512 | The ring of the log, 32 bytes for each line. |
| `spans` | 1024 | The room of the list of the observer of the window. |

- The module runs its system `round` in its phase `first`, and its system `step` in its phase `fixed`. The deployment maps `first` to the first phase of the round, and `fixed` to the fixed phase.
- The deployment names the fixed phase in `again`, with a cap of at least `most_steps`. `step` asks for its phase again until the round did its steps.
- A pair puts `ideatek_frame_sdl3:clock:round` before `ideatek_window_sdl3:window:events`, and before each reader of the first phase.
- The module takes the grants of the clock module: `docs/extern/kernel/ideatek_frame.h.md`, *The grants*. With `sleeper` at `"events"`, it also takes the grants of the clock module of `docs/extern/platform_abstraction/ideatek_window.h.md`, *The grants*.

### The messages

The lines of the log of the module:

| Line | Cause |
| --- | --- |
| `scale <value> is refused` | `scale` of `ideatek:TimeRequest` is not finite, or is below 0. The module keeps the last scale. |
| `rate <value> is refused` | `rate` is 0, or more than 1000. The module keeps the last rate. |
| `max_rate <value> is refused` | `max_rate` is more than 1000. The module keeps the last limit. |
| `paused <value> is refused` | `paused` is not 0 or 1. The module keeps the last value. |

For a setting outside its range, the line is the one of `readme.md`, *The contracts*.

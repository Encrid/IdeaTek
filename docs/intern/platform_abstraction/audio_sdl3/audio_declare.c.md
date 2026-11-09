# src/platform_abstraction/audio_sdl3/audio_declare.c

This file declares the types, the system and the 2 observers of the audio module to Bridge.

## The declarations

The order of the declarations is: `ideatek:ideatek_entity_t`, the 3 enums, the 5 structures, then the settings: `docs/extern/platform_abstraction/ideatek_audio.h.md`, *The declarations*.
`declare_fields` declares the field types, and `declare_values` the structures and the settings.

| Call | Kind | Name |
| --- | --- | --- |
| System | `first` | `round` |
| Observer | enter, attach and leave | `request` on `ideatek:AudioRequest` |
| Observer | enter and attach | `frames` on `ideatek:AudioFrames` |

`round` states `ideatek:AudioDevice`, `ideatek:AudioRecorded` and `ideatek:AudioState` as its changes.
The 2 observers state no change: `request` marks a stream, and `frames` puts bytes in a stream of SDL3.

## Functions

| Function | Result |
| --- | --- |
| `declare_fields` | Declares the entity type and the 3 enums. |
| `declare_values` | Declares the 5 structures and the settings. |
| `declare_types` | Declares the 10 types in the order of the slots. |
| `add_runs` | Declares the system and the 2 observers. |

# src/platform_abstraction/audio_sdl3/audio_main.c

This file loads and unloads the module.

## The load

`init` starts the audio of SDL3 with `sdl3_start_subsystem` of `docs/intern/platform_abstraction/sdl3_start/sdl3_start.c.md`.
A refused start writes `no audio: <reason>`, and `init` gives 1: the load fails.
Then it declares the types and the calls: `docs/intern/platform_abstraction/audio_sdl3/audio_declare.c.md`.
`enable_events` turns on the 3 types of the audio devices, `SDL_EVENT_AUDIO_DEVICE_ADDED` to `SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED`, which the first start turned off.
The settings come after the load, so `init` reads none. The first round reads them: `docs/intern/platform_abstraction/audio_sdl3/audio_start.c.md`.

## The unload

`shutdown` closes the 2 streams: `docs/intern/platform_abstraction/audio_sdl3/audio_stream.c.md`. Then it stops the audio of SDL3 with `sdl3_stop_subsystem`.
`reload` gives 0: the module keeps nothing that a reload could carry.

## The branches

| Case | Result |
| --- | --- |
| SDL3 does not start its audio. | The line of the audio, and the load fails. |
| A stream is open at the unload. | The stream closes. |

## Functions

| Function | Result |
| --- | --- |
| `enable_events` | Turns on the 3 types of event of the audio devices. |
| `init` | Starts the audio, declares the types and the calls. Gives 1 when the audio does not start. |
| `shutdown` | Closes the streams and stops the audio. |
| `reload` | Gives 0. |

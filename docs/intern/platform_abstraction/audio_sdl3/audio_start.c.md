# src/platform_abstraction/audio_sdl3/audio_start.c

This file checks the settings and lists the devices in the first round.

## The first round

`start_audio` does these steps:

1. It checks `device_frames` with `check_frames`. A wrong value writes the line of `readme.md`, *The contracts*, and the module is broken.
2. It copies `stream_name` from the arena into `name`. A text outside the arena, empty, or of 256 bytes or more, writes the line of the setting. The module is then broken.
3. It sets the hints of SDL3 with `SDL_HINT_OVERRIDE`: `SDL_HINT_AUDIO_DEVICE_SAMPLE_FRAMES` when `device_frames` is not 0, and `SDL_HINT_AUDIO_DEVICE_STREAM_NAME`.
4. It takes the 2 halves of the recorded frames from the persistent arena.
5. It reads the default id of each direction with `SDL_GetDefaultAudioDeviceID`.
6. It lists the devices of each direction with `SDL_GetAudioPlaybackDevices` and `SDL_GetAudioRecordingDevices`, and gives each one its entity: `docs/intern/platform_abstraction/audio_sdl3/audio_devices.c.md`.

A list that SDL3 refuses writes `no device list: <reason>`, and the direction has no device until an event adds one.
The hints come before the first open of a device, so each device of the module gets them.

## The branches

| Case | Result |
| --- | --- |
| `device_frames` outside its range. | Its line, and `round` gives 1 in each round. |
| `stream_name` empty, too long, or outside the arena. | Its line, and `round` gives 1 in each round. |
| `device_frames` at 0. | No hint of the frames: SDL3 picks the buffer. |
| SDL3 gives no list of a direction. | The line of the list, and no device of the direction. |

## Functions

| Function | Result |
| --- | --- |
| `copy_name` | Copies `stream_name` into `name`, or writes its line. |
| `list_direction` | Gives an entity to each device of a direction, or writes the line of the list. |
| `start_audio` | Runs the steps of the first round. Gives `false` when the module is broken. |

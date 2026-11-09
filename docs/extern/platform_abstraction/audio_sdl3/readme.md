# BM-IdeaTek-Audio-SDL3

The audio module of IdeaTek on SDL3: the devices of the system, and 1 stream in each direction.

## Install

The release of `BM-IdeaTek-Audio-SDL3` has the tag `audio_sdl3-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Audio-SDL3.zip` and `sha256.txt`. `BM-IdeaTek-Audio-SDL3.zip` has a folder for each target.
In `BM-IdeaTek-Audio-SDL3`, each folder holds the module `audio_sdl3.dll`, `audio_sdl3.so` or `audio_sdl3.dylib`.
Each folder of `BM-IdeaTek-Audio-SDL3.zip` also holds this readme and the licence.

The module links `BL-IdeaTek-SDL3` by the name of its shared library, which must be in the folder of the module.
So the module and that library share 1 folder of the deployment: `docs/extern/platform_abstraction/sdl3/readme.md`.
The module needs the SDL3 of IdeaTek, because it calls `SDL_GetDefaultAudioDeviceID`, which that library adds: `docs/extern/platform_abstraction/sdl3/readme.md`, *The additions of IdeaTek*.
A project names the zip of `BM-IdeaTek-Audio-SDL3` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_audio_sdl3:audio@0.1.0`.
Its role is the audio module: `docs/extern/platform_abstraction/ideatek_audio.h.md`, *The roles*.
The component of its settings is `ideatek_audio_sdl3:Settings`. The contract has no setting, so the 2 settings are the ones of the module:

| Setting | Type | Default | Range | Meaning |
| --- | --- | --- | --- | --- |
| `device_frames` | `uint32_t` | 0 | 0, or 64 to 65536 | The frames of 1 buffer of a device. 0 takes the value that SDL3 picks for the device. A smaller buffer gives less latency and more work. |
| `stream_name` | text | `IdeaTek` | 1 to 255 bytes | The name of the streams of the module in the mixer of the system. PulseAudio and PipeWire show it. |

The module reads the settings in its first round, before it opens a device.

### The devices

At its start, the module spawns an entity for each device of `SDL_GetAudioPlaybackDevices` and `SDL_GetAudioRecordingDevices`, up to 32 devices.
A 33rd device gets no entity, and the log gets 1 line. A name longer than 127 bytes is cut at a code point, and the log gets 1 line.
The events of SDL3 add and remove the devices in each round. The module asks `SDL_GetDefaultAudioDeviceID` for the default of each direction in each round. When the default changes, both the old and the new default get `ideatek:AudioDevice` again.

### The streams

The module opens 1 `SDL_AudioStream` for each direction, on the device that the request names, in the format of the request. SDL3 converts the format to the one of the device.

A playback device that does not open gives `IDEATEK_AUDIO_STATUS_NO_DEVICE`. A recording device that does not open gives `IDEATEK_AUDIO_STATUS_DENIED`. On macOS, the system refuses the device when the user refuses the microphone.
`IDEATEK_AUDIO_STATUS_PENDING` and `IDEATEK_AUDIO_STATUS_SUSPENDED` have no path on the 6 targets of the release. No desktop system asks the user during the run, and none stops the audio of a game. The 2 statuses come with the mobile targets.

In each round, the system of the module:

1. Takes the devices that SDL3 added or removed, and the new default.
2. Applies each `ideatek:AudioRequest` that changed. A new device, target or direction closes the stream and opens a new one. A new rate or new channels set the format of the stream. A new `ahead` sets `wanted`.
3. Puts the batch of `ideatek:AudioFrames` of the round before in the playback stream, if its `at` is the count.
4. Takes the recorded frames of the recording stream into the half of the round, and attaches `ideatek:AudioRecorded`.
5. Attaches `ideatek:AudioState` to each stream whose state changed by a byte.

So the frames that the game writes in a round play in the next round. `ahead` covers that round.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The module reads the events of SDL3 on the thread of the phases. The audio thread of SDL3 is its own. |
| `reloadable` | `false` | Bridge does not store the devices and the streams of SDL3. |
| `spawns` | 32 | 1 entity for each device, up to 32. |
| `changes` | 16384 | The attaches of the devices, the 2 states and the recorded batch of a round. |
| `persistent` | 4202496 | The 2 halves of the recorded frames of the contract, plus 8192 for the texts of the settings. |
| `scratch` | 16384 | The room of the declarations and of the log lines. |
| `log_ring` | 2048 | 64 lines of the log of 32 bytes. |
| `spans` | 16384 | The lists of the 2 observers: the requests, and the frames. |

- The system `round` of the module is in its phase `first`. The deployment maps this phase to the start of the round.
- The module pumps the event queue of SDL3 in its round. With the window module, a pair puts `ideatek_window_sdl3:window:events` before `round`, so the wait of the window comes first.
- A pair puts the game before `ideatek_audio_sdl3:audio:round` when the game wants its frames played in the same round. Else they play in the next round.
- The module takes the grants of the audio module: `docs/extern/platform_abstraction/ideatek_audio.h.md`, *The grants*.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `no audio: <reason>` | SDL3 does not start its audio. The load fails. |
| `no device list: <reason>` | SDL3 gives no list of the devices of a direction. |
| `more than 32 devices: the device <id> gets no entity` | The system has a 33rd device. |
| `the name of the device <id> is longer than 127 bytes` | The name of a device does not fit in `name`. |
| `the entity <id> asks for a second <direction> stream, and gets none` | A second entity gets `ideatek:AudioRequest` of a direction that has a stream. |
| `the request of <id>: <field> <value> is outside <range>` | A field of the request is outside its range. No stream opens. |
| `no <direction> stream: <reason>` | SDL3 does not open the stream. The status is `IDEATEK_AUDIO_STATUS_NO_DEVICE` for a playback, and `IDEATEK_AUDIO_STATUS_DENIED` for a recording. |
| `the device of <id> is not there, so the stream uses the default` | The chosen device went, or is not a device of the direction. The status is `IDEATEK_AUDIO_STATUS_FELL_BACK`. |

A setting outside its range writes the line that `readme.md`, *The contracts*, states.

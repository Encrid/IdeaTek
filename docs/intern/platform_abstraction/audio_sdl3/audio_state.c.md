# src/platform_abstraction/audio_sdl3/audio_state.c

This file fills and attaches `ideatek:AudioState` of each stream.

## The state

`fill_state` gives the state of a stream in the round:

| Field | Value |
| --- | --- |
| `device` | The entity of the device that the stream plays on: the chosen device, or the default device of the direction. 0 with no stream. |
| `dropped` | `dropped` of the stream. |
| `rate`, `channels` | The format of the device of the stream, from `SDL_GetAudioDeviceFormat` on the id that the stream opened. 0 with no stream. |
| `queued` | For a playback, `SDL_GetAudioStreamQueued` divided by the bytes of a frame of the request. 0 for a recording, because the round took its frames. |
| `wanted` | For a playback, `frames_wanted` of `ahead` and `queued`. 0 for a recording. |
| `status` | The status of the stream. |

`device_entity` gives the entity of the device of a stream. It is the device of the id that the stream opened, or the default device of the direction.

## The attach

`attach_state` compares the state of the round with `last`. If a byte differs, it attaches the state, and keeps it as `last`.
A stream with no entity attaches nothing.

## The branches

| Case | Result |
| --- | --- |
| A stream whose state changed. | 1 attach. |
| A stream whose state did not change. | No attach. |
| A stream on the default, after a change of the default. | The state names the new device. |
| No stream of a direction. | No attach. |

## Functions

| Function | Result |
| --- | --- |
| `device_entity` | Gives the entity of the device that a stream plays on. |
| `fill_state` | Gives the state of a stream in the round. |
| `attach_state` | Attaches the state of a stream when a byte changed. |

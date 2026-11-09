# src/platform_abstraction/audio_sdl3/audio_inside.h

Each source of the part `audio_sdl3` includes this header.
It includes SDL3, BB-Native, `include/platform_abstraction/ideatek_audio.h`, the header of the part `sdl3_start` and `audio_values.h`.
It holds the slots, the limits and the types of the module, with the functions shared by its sources.

## The module

The part fills the role of the audio module: `docs/usage/platform_abstraction/audio_sdl3.md`.
Its identity is `ideatek_audio_sdl3:audio@0.1.0`, and its package is `BM-IdeaTek-Audio-SDL3`.
It is pinned: the pump of the events of SDL3 runs on the thread of the phases, while SDL3 owns its audio thread.
It is not reloadable: the devices and the streams live in SDL3, outside the storage of Bridge.

## The slots

A slot is the index of its declaration, from 0. `declare_types` of `docs/intern/platform_abstraction/audio_sdl3/audio_declare.c.md` keeps this order:

| Slot | Name | Type |
| --- | --- | --- |
| 0 to 3 | No name | The field types: `ideatek:ideatek_entity_t` and the 3 enums of the audio. |
| 4 | `DEVICE` | `ideatek:AudioDevice` |
| 5 | `REQUEST` | `ideatek:AudioRequest` |
| 6 | `FRAMES` | `ideatek:AudioFrames` |
| 7 | `RECORDED` | `ideatek:AudioRecorded` |
| 8 | `STATE` | `ideatek:AudioState` |
| 9 | `SETTINGS` | `ideatek_audio_sdl3:Settings` |

## The limits

| Constant | Value | Limit |
| --- | --- | --- |
| `AUDIO_DEVICES` | 32 | The most devices that get an entity. The manifest states it as `spawns`. |
| `AUDIO_NAME` | 128 | The bytes of the name of a device, with its zero. |
| `AUDIO_REASON` | 256 | The buffer of the reason that SDL3 gives for a refusal. |
| `AUDIO_HALF_FRAMES` | 65536 | The frames of 1 half of the recorded frames: the most `ahead`. |
| `AUDIO_HALF_BYTES` | 2097152 | The bytes of 1 half: 65536 frames of 8 channels of 4 bytes. |
| `AUDIO_TEXT` | 256 | The bytes of the copy of `stream_name`, with its zero. |
| `CHANGE_HEAD` | 24 | The head of a change in `changes`. The 2 macros `CHANGE_SIZE` and `CHANGE_BYTES` give the cost of a change with its value, as the window module counts it. |

The ranges of the fields of a request are in `audio_values.h`: `docs/intern/platform_abstraction/audio_sdl3/audio_values.h.md`.

## The settings

`ideatek_audio_sdl3:Settings` is the component of the settings. The macro `AUDIO_SETTINGS_FIELDS` gives its fields:

| Field | C type | Setting |
| --- | --- | --- |
| `device_frames` | `uint32_t` | `device_frames`. |
| `stream_name` | `bridge_text_t` | `stream_name`. Its text is in the persistent share of the module, where Bridge puts it. |

## The types

| Type | Field | Value |
| --- | --- | --- |
| `audio_device_t` | `id` | The id of the physical device in SDL3. |
| | `entity` | The entity of the device. |
| | `recording` | `true` for a recording device. |
| | `is_default` | The value that the last `ideatek:AudioDevice` of the entity holds. |
| `audio_stream_t` | `stream` | The `SDL_AudioStream` of the direction, or `nullptr`. |
| | `entity` | The entity of the request, or 0 with no request. |
| | `applied` | The request that the stream follows. |
| | `last` | The last `ideatek:AudioState` that the module attached. |
| | `count` | The frames that the stream played or recorded, from 0. |
| | `dropped` | The frames that the module dropped. |
| | `device` | The id that the stream opened: a physical id, or the default id of its direction. |
| | `status` | The status of the stream. |
| | `pending` | A request came, and the round applies it. |
| | `lost` | The device of the stream went, and the round opens the stream again. |

## The state

The state is 1 global variable, `audio_module`, as in the other modules. An exception of the rule G4 of the style covers it.

| Field | Value |
| --- | --- |
| `started`, `broken` | The first round read the settings. A wrong setting breaks the module. |
| `devices`, `device_count` | The devices with an entity, as `audio_device_t`. |
| `defaults` | The id of the default device of each direction, 0 with none. |
| `streams` | The 2 streams, by direction: `IDEATEK_AUDIO_DIRECTION_PLAYBACK` then `IDEATEK_AUDIO_DIRECTION_RECORDING`. |
| `half`, `side` | The place of the 2 halves of the recorded frames in the persistent arena, and the side of the round. |
| `name` | The copy of `stream_name`, for the hint of SDL3. |
| `request_list`, `frames_list` | The lists of the 2 observers, by their index. |

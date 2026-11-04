# include/platform/audio/ideatek_audio.h

`include/platform/audio/ideatek_audio.h` gives the types of the audio part: the audio devices, 1 playback stream and 1 recording stream.
`readme.md`, *The contracts*, tells the rules of each contract.
A device and a request name an entity of `include/platform/world/ideatek_world.h`, so the audio header includes it.

## The roles

- The game writes `ideatek:AudioRequest` and `ideatek:AudioFrames`.
- The audio module writes `ideatek:AudioDevice`, `ideatek:AudioRecorded` and `ideatek:AudioState`. A deployment has one audio module.
- The game and the audio module read all the types of this header.

Out: integer samples, the metadata of the OS for a stream, raw recording, and more than 1 stream for each direction.
Comes back: integer samples or the metadata of the OS, when a platform needs them.
Comes back: raw recording for music, and more streams for each direction.

## The devices

The audio module spawns an entity for each audio device when it starts, and when the OS adds a device.
It attaches `ideatek:AudioDevice` to the entity. When the OS removes the device, the audio module destroys the entity.
When the default device of the OS changes, the audio module attaches `ideatek:AudioDevice` again to the old and the new default.

## The streams

The game spawns an entity and attaches `ideatek:AudioRequest` to it.
The audio module opens a stream for the first entity of each direction, and attaches `ideatek:AudioState` to the same entity.
For each other entity of a direction, the log of the audio module gets 1 line, and no stream opens.
A request with a field out of its range opens no stream either, and the audio module logs it.

When the game attaches `ideatek:AudioRequest` again, the audio module applies each field that changed.
To close the stream, the game destroys the entity. The audio module observes `BRIDGE_ON_LEAVE` of `ideatek:AudioRequest`.
In each round, the audio module attaches `ideatek:AudioState` again if a byte of it changed.

With `IDEATEK_AUDIO_TARGET_DEFAULT`, the stream follows the default device of the OS.
With `IDEATEK_AUDIO_TARGET_CHOSEN`, the stream uses the device entity in `device`.
If that device goes, or if `device` is not a device of the direction, the stream uses the default device.
Then the status is `IDEATEK_AUDIO_STATUS_FELL_BACK`. The stream stays on the default device when the chosen device comes back.

The streams go on when the window loses the focus.
When the OS stops the audio of the game, for example for a call on a phone, the status is `IDEATEK_AUDIO_STATUS_SUSPENDED`.

## The frames

A sample is a `float` from -1 to 1. A frame holds 1 sample for each channel, in the order of the channels.
The frames of a batch follow each other, with no bytes between them.
The audio module converts the rate and the channels of the request to those of the device.

| Channels | The order of the channels |
| --- | --- |
| 1 | mono |
| 2 | front left, front right |
| 3 | front left, front right, low frequency |
| 4 | front left, front right, back left, back right |
| 5 | front left, front right, low frequency, back left, back right |
| 6 | front left, front right, front center, low frequency, back left, back right |
| 7 | front left, front right, front center, low frequency, back center, side left, side right |
| 8 | front left, front right, front center, low frequency, back left, back right, side left, side right |

## The playback

The game attaches `ideatek:AudioFrames` to the entity of the playback stream, in each round that has frames for it.
At `init`, the game takes the bytes for its frames from its share. It fills these bytes again in each round.
The audio module puts each batch once in the queue of the device.

The audio module counts the frames of the stream from 0.
A batch whose `at` is the count goes into the queue, and the count grows by `frames`.
The audio module drops a batch with a different `at`, and adds its `frames` to `dropped`.
`wanted` is the number of frames that bring the queue to `ahead` frames. The game sends at most `wanted` frames.
When the queue is empty, the device plays silence.

## The recording

Before the first recording stream, the audio module asks the OS for the permission of the user.
While the user chooses, the status is `IDEATEK_AUDIO_STATUS_PENDING`. If the user refuses, the status is `IDEATEK_AUDIO_STATUS_DENIED`.
The echo cancellation and the noise reduction of the OS stay on.

In each round that has recorded frames, the audio module attaches `ideatek:AudioRecorded` to the entity of the recording stream.
`at` is the number of frames that the stream recorded before the batch.
A batch holds at most `ahead` frames. The audio module drops the older frames, and adds their number to `dropped`.

At `init`, the audio module takes 4194304 bytes of its persistent arena: 2 halves of 65536 frames of 8 channels.
The rounds use the 2 halves in turn for their recorded frames.
So the bytes of `ideatek:AudioRecorded` stay good for 1 round.

## ideatek:AudioDevice

`ideatek:AudioDevice` gives an audio device.
`ideatek_audio_device_t` takes 130 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `char[128]` | 0 | The name of the device. It is UTF-8 text that ends with a NUL byte. |
| `direction` | `ideatek_audio_direction_t` | 128 | The direction of the device. |
| `is_default` | `bool` | 129 | `true` if the device is the default device of the OS for its direction. |

### ideatek:ideatek_audio_direction_t

`ideatek_audio_direction_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The direction |
| --- | --- | --- |
| `IDEATEK_AUDIO_DIRECTION_PLAYBACK` | 0 | The frames go to a speaker or a headset. |
| `IDEATEK_AUDIO_DIRECTION_RECORDING` | 1 | The frames come from a microphone. |

## ideatek:AudioRequest

`ideatek:AudioRequest` asks for a stream.
`ideatek_audio_request_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `device` | `ideatek_entity_t` | 0 | With `IDEATEK_AUDIO_TARGET_CHOSEN`: the device entity. |
| `rate` | `uint32_t` | 8 | The frames per second of the game, 8000 to 384000. |
| `ahead` | `uint32_t` | 12 | The frames that the queue holds, 1 to 65536. |
| `direction` | `ideatek_audio_direction_t` | 16 | The direction of the stream. |
| `target` | `ideatek_audio_target_t` | 17 | The device of the stream. |
| `channels` | `uint8_t` | 18 | The channels of the game, 1 to 8. |

Bytes 19 to 23 hold no field.

### ideatek:ideatek_audio_target_t

`ideatek_audio_target_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The device |
| --- | --- | --- |
| `IDEATEK_AUDIO_TARGET_DEFAULT` | 0 | The default device of the OS for the direction. |
| `IDEATEK_AUDIO_TARGET_CHOSEN` | 1 | The device entity in `device`. |

## ideatek:AudioFrames and ideatek:AudioRecorded

`ideatek:AudioFrames` holds a batch that the game plays. `ideatek:AudioRecorded` holds a batch that the device recorded.
Both have the C type `ideatek_audio_frames_t`, which takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `at` | `uint64_t` | 0 | The number of the first frame of the batch in the stream. |
| `frames` | `uint32_t` | 8 | The number of frames of the batch. |
| `data` | `bridge_text_t` | 12 | The frames: *The frames*. |

Bytes 20 to 23 hold no field.

## ideatek:AudioState

`ideatek:AudioState` gives the state of a stream.
`ideatek_audio_state_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `device` | `ideatek_entity_t` | 0 | The device entity of the stream. 0 with `IDEATEK_AUDIO_STATUS_NO_DEVICE`. |
| `dropped` | `uint64_t` | 8 | The number of frames that the audio module dropped. |
| `rate` | `uint32_t` | 16 | The frames per second of the device. |
| `queued` | `uint32_t` | 20 | The frames in the queue of the device. |
| `wanted` | `uint32_t` | 24 | For playback: the frames that the game sends next. For recording: 0. |
| `channels` | `uint8_t` | 28 | The channels of the device. |
| `status` | `ideatek_audio_status_t` | 29 | The status. |

Bytes 30 and 31 hold no field.

### ideatek:ideatek_audio_status_t

`ideatek_audio_status_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The status |
| --- | --- | --- |
| `IDEATEK_AUDIO_STATUS_RUNNING` | 0 | The stream runs on the device of its request. |
| `IDEATEK_AUDIO_STATUS_FELL_BACK` | 1 | The stream runs on the default device, because the chosen device is not there. |
| `IDEATEK_AUDIO_STATUS_NO_DEVICE` | 2 | The OS has no device for the direction. |
| `IDEATEK_AUDIO_STATUS_SUSPENDED` | 3 | The OS stopped the audio of the game. |
| `IDEATEK_AUDIO_STATUS_PENDING` | 4 | The user did not choose yet to permit the recording. |
| `IDEATEK_AUDIO_STATUS_DENIED` | 5 | The user did not permit the recording. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_audio_direction_t` | `ideatek_audio_direction_t` | `ideatek_audio_direction_t_desc` | `IDEATEK_AUDIO_DIRECTION_VALUES` |
| `ideatek:ideatek_audio_target_t` | `ideatek_audio_target_t` | `ideatek_audio_target_t_desc` | `IDEATEK_AUDIO_TARGET_VALUES` |
| `ideatek:ideatek_audio_status_t` | `ideatek_audio_status_t` | `ideatek_audio_status_t_desc` | `IDEATEK_AUDIO_STATUS_VALUES` |
| `ideatek:AudioDevice` | `ideatek_audio_device_t` | `ideatek_audio_device_t_desc` | `IDEATEK_AUDIO_DEVICE_FIELDS` |
| `ideatek:AudioRequest` | `ideatek_audio_request_t` | `ideatek_audio_request_t_desc` | `IDEATEK_AUDIO_REQUEST_FIELDS` |
| `ideatek:AudioFrames` | `ideatek_audio_frames_t` | `ideatek_audio_frames_t_desc` | `IDEATEK_AUDIO_FRAMES_FIELDS` |
| `ideatek:AudioRecorded` | `ideatek_audio_frames_t` | `ideatek_audio_frames_t_desc` | `IDEATEK_AUDIO_FRAMES_FIELDS` |
| `ideatek:AudioState` | `ideatek_audio_state_t` | `ideatek_audio_state_t_desc` | `IDEATEK_AUDIO_STATE_FIELDS` |

```text
{ IDEATEK_AUDIO_DIRECTION_PLAYBACK = 0, IDEATEK_AUDIO_DIRECTION_RECORDING = 1 }
{ IDEATEK_AUDIO_TARGET_DEFAULT = 0, IDEATEK_AUDIO_TARGET_CHOSEN = 1 }
{ IDEATEK_AUDIO_STATUS_RUNNING = 0, IDEATEK_AUDIO_STATUS_FELL_BACK = 1, IDEATEK_AUDIO_STATUS_NO_DEVICE = 2, IDEATEK_AUDIO_STATUS_SUSPENDED = 3, IDEATEK_AUDIO_STATUS_PENDING = 4, IDEATEK_AUDIO_STATUS_DENIED = 5 }
{ char name[128]; ideatek_audio_direction_t direction; bool is_default; }
{ ideatek_entity_t device; uint32_t rate; uint32_t ahead; ideatek_audio_direction_t direction; ideatek_audio_target_t target; uint8_t channels; }
{ uint64_t at; uint32_t frames; bridge_text_t data; }
{ ideatek_entity_t device; uint64_t dropped; uint32_t rate; uint32_t queued; uint32_t wanted; uint8_t channels; ideatek_audio_status_t status; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_audio_direction_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_audio_target_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_audio_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AudioDevice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AudioRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AudioFrames` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AudioRecorded` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AudioState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

Before the five structures, a module declares the three enums and `ideatek:ideatek_entity_t`.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:AudioRequest` and `ideatek:AudioFrames` | `ideatek:ideatek_entity_t`, the three enums and the three types of the audio module |
| The audio module | `ideatek:AudioDevice`, `ideatek:AudioRecorded` and `ideatek:AudioState` | `ideatek:ideatek_entity_t`, the three enums and the two types of the game |

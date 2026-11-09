# src/platform_abstraction/audio_sdl3/audio_stream.c

This file opens, changes and closes the 2 streams of SDL3.

## The device of a stream

`pick_device` gives the id that a stream opens:

| Request | Id | Status |
| --- | --- | --- |
| `IDEATEK_AUDIO_TARGET_DEFAULT` | `SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK` or `SDL_AUDIO_DEVICE_DEFAULT_RECORDING` | `IDEATEK_AUDIO_STATUS_RUNNING` |
| `IDEATEK_AUDIO_TARGET_CHOSEN`, and `device` is a device of the direction | The id of that device | `IDEATEK_AUDIO_STATUS_RUNNING` |
| `IDEATEK_AUDIO_TARGET_CHOSEN`, and `device` is not | The default id, and the log gets the line of the device that is not there | `IDEATEK_AUDIO_STATUS_FELL_BACK` |

A stream on the default id follows the default device inside SDL3, with no open of the module.

## The open

`open_stream` opens the stream of a request:

```pseudo
FUNCTION open_stream(stream : audio_stream_t) RETURNS BOOLEAN
   DECLARE spec : SDL_AudioSpec
   spec.format <- SDL_AUDIO_F32
   spec.channels <- stream.applied.channels
   spec.freq <- stream.applied.rate
   stream.device <- pick_device(stream)
   stream.stream <- SDL_OpenAudioDeviceStream(stream.device, spec, nullptr, nullptr)
   IF stream.stream = nullptr THEN
      RETURN FALSE
   ENDIF
   CALL SDL_ResumeAudioStreamDevice(stream.stream)
   RETURN TRUE
ENDFUNCTION
```

`SDL_OpenAudioDeviceStream` opens a logical device of SDL3 and binds a stream in the format of the request. The conversion to the format of the device is inside SDL3.
The logical device starts paused, so `SDL_ResumeAudioStreamDevice` starts it.
A refused open writes the line of the stream. A playback then has the status no device, and a recording the status denied.

## The close

`close_stream` destroys the stream with `SDL_DestroyAudioStream`, which also closes the logical device that `SDL_OpenAudioDeviceStream` opened.
When the entity leaves, the count, the dropped frames and the last state go to 0. They stay when the stream opens again on another device.

## A change

`apply_stream` runs in each round for each stream:

1. A stream that is not `pending` and not `lost` changes nothing.
2. A pending request is read with `bridge_native_find_value`, and checked with `check_request`. A fault writes the line of the request, and the stream stays as it was.
3. A new `direction`, `target` or `device`, or a lost device, closes the stream and opens it again with `open_stream`.
4. A new `rate` or new `channels` on an open stream set the format of the stream with `SDL_SetAudioStreamFormat`.
5. A new `ahead` changes `wanted` alone, in the state of the round.

A lost stream opens again on the default, with `IDEATEK_AUDIO_STATUS_FELL_BACK`.

## The branches

| Case | Result |
| --- | --- |
| A request on the default. | A stream on the default id. |
| A request on a device of the direction. | A stream on that device. |
| A request on a device that is not there, or of the other direction. | A stream on the default, the status fell back, and the line of the device. |
| SDL3 refuses the open of a playback. | The line of the stream, and the status no device. |
| SDL3 refuses the open of a recording. | The line of the stream, and the status denied. |
| A field outside its range. | The line of the request, and no change. |
| A new rate or new channels. | The format of the stream changes. |
| A new target, device or direction. | The stream closes and opens again. |
| The device of the stream goes. | The stream opens again on the default, and the status is fell back. |
| The entity of the stream leaves. | The stream closes. |

## Functions

| Function | Result |
| --- | --- |
| `pick_device` | Gives the id that a stream opens, and sets its status. |
| `open_stream` | Opens the stream of SDL3 for a request. Gives `false` when SDL3 refuses. |
| `close_stream` | Destroys the stream of SDL3, and its logical device. |
| `read_request` | Reads and checks the pending request of a stream. Gives `false` with a fault. |
| `apply_stream` | Applies a pending request, or opens a lost stream again. |

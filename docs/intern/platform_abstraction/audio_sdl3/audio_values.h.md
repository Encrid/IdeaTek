# src/platform_abstraction/audio_sdl3/audio_values.h

This header gives the checks and the counts of the audio module that call no SDL3 and no Bridge. A test program checks them alone.

`audio_fault_t` names the field of a request that is outside its range: its name, its value and the text of its range.
The ranges are the ones of `docs/extern/platform_abstraction/ideatek_audio.h.md`, *ideatek:AudioRequest*:

| Constant | Value | Field |
| --- | --- | --- |
| `AUDIO_RATE_LEAST` | 8000 | `rate` |
| `AUDIO_RATE_MOST` | 384000 | `rate` |
| `AUDIO_AHEAD_MOST` | 65536 | `ahead`, from 1 |
| `AUDIO_CHANNELS_MOST` | 8 | `channels`, from 1 |
| `AUDIO_FRAMES_LEAST` | 64 | `device_frames`, or 0 |
| `AUDIO_FRAMES_MOST` | 65536 | `device_frames` |

The functions are in `docs/intern/platform_abstraction/audio_sdl3/audio_values.c.md`.

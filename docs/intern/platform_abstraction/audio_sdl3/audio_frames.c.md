# src/platform_abstraction/audio_sdl3/audio_frames.c

This file puts the frames of the game in the playback stream, and reads the frames that the recording stream recorded.

## The playback

The observer `frames` hears `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` of `ideatek:AudioFrames`. For each trigger on the entity of the playback stream, `put_frames` reads the batch:

1. A batch with `at` equal to the count goes into the stream with `SDL_PutAudioStreamData`. Its bytes are `frames` times `channels` samples of 4 bytes. The count grows by `frames`.
2. A batch with another `at` goes nowhere, and `dropped` grows by `frames`.
3. A batch whose `data` has fewer bytes than its frames need is a batch with another `at`.

A trigger on another entity, or with no open stream, changes nothing. The bytes come from the share of the game, at `data.at`.
The stream of SDL3 keeps the bytes and converts them on its thread, so the observer makes 1 copy.

## The recording

`take_recorded` runs in each round:

1. With no open recording stream, nothing.
2. `SDL_GetAudioStreamAvailable` gives the bytes that the stream recorded. The frames over `ahead` are the oldest. `drop_oldest` reads them into the half and forgets them, and `dropped` grows by their number.
3. `attach_recorded` reads the frames left, at most `ahead`, into the half of the round with `SDL_GetAudioStreamData`.
4. With 1 frame or more, `ideatek:AudioRecorded` goes to the entity. `at` is the count, `frames` their number, and `data` their bytes in the half. The count grows by the dropped and the kept frames.

A half holds 65536 frames of 8 channels, so the frames left always fit. `drop_oldest` reads the half at most each `AUDIO_HALF_FRAMES` frames, so a long pause of the game costs rounds of reads, not memory.

## The branches

| Case | Result |
| --- | --- |
| A batch at the count. | The frames go in the stream. |
| A batch at another count. | The frames are dropped. |
| A batch with too few bytes. | The frames are dropped. |
| A batch on another entity. | Nothing. |
| Recorded frames, at most `ahead`. | `ideatek:AudioRecorded` with all of them. |
| Recorded frames over `ahead`. | The oldest are dropped, and the batch holds `ahead` frames. |
| No recorded frame. | No `ideatek:AudioRecorded`. |

## Functions

| Function | Result |
| --- | --- |
| `put_frames` | Puts a batch in the playback stream, or drops it. |
| `bridge_frames` | Reads each trigger. |
| `drop_oldest` | Reads and forgets the frames over `ahead`. |
| `attach_recorded` | Reads the frames left into the half, and attaches the batch when it has a frame. |
| `take_recorded` | Counts the recorded frames, drops the oldest, and keeps the rest. |

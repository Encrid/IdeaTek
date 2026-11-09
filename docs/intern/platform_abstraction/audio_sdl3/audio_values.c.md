# src/platform_abstraction/audio_sdl3/audio_values.c

This file checks a request and counts the frames, with no call of SDL3 or of Bridge, so the program `audio_values` tests it alone.

## The checks

`check_request` reads each field of an `ideatek:AudioRequest` in the order of the table, and stops at the first field outside its range:

| Field | Range | Text of the range |
| --- | --- | --- |
| `direction` | 0 to 1 | `0 to 1` |
| `target` | 0 to 1 | `0 to 1` |
| `rate` | 8000 to 384000 | `8000 to 384000` |
| `ahead` | 1 to 65536 | `1 to 65536` |
| `channels` | 1 to 8 | `1 to 8` |

It gives `true` for a request inside the ranges, else `false` with the fault.
`check_frames` gives `true` for a `device_frames` of 0, or of 64 to 65536.

## The counts

`frames_wanted` gives `ahead` minus `queued`, or 0 when the queue holds `ahead` frames or more. The result is arithmetic, with no branch.
`frames_over` gives the frames over `ahead` among `frames`, or 0. The result is arithmetic, with no branch.

## The branches

| Case | Result |
| --- | --- |
| A request inside the ranges. | `true`. |
| A field outside its range. | `false`, and the fault names the first such field. |
| `queued` at or over `ahead`. | `frames_wanted` gives 0. |
| `frames` over `ahead`. | `frames_over` gives the difference. |

## Functions

| Function | Result |
| --- | --- |
| `check_request` | Gives `true` for a request inside the ranges, else `false` and the first fault. |
| `check_frames` | Gives `true` for a `device_frames` of 0, or of 64 to 65536. |
| `frames_wanted` | Gives the frames that bring the queue to `ahead`. |
| `frames_over` | Gives the frames over `ahead`. |

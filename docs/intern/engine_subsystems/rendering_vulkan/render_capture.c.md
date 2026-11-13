# src/engine_subsystems/rendering_vulkan/render_capture.c

This file takes the requests of `ideatek:CaptureRequest`, and records their copies: `docs/extern/engine_subsystems/ideatek_rendering.h.md`, *The captures*.

## The requests

The observer `request` hears each attach and each change of `ideatek:CaptureRequest`. `keep_request` puts the entity in a free place of `captures`, as waiting.
A change of an entity that waits or copies makes it wait again. A leave frees its place.
With no free place, the request is not kept: `docs/usage/engine_subsystems/rendering_vulkan.md`, *The messages*.

## The copy

`record_captures` takes the captures that wait, and copies at most 1 in each round. The others wait for the next rounds.
`get_status` checks a capture against the round, and keeps its part in pixels and the round:

1. A target that is not the window, or a round with no camera on the window, gives `IDEATEK_CAPTURE_STATUS_NO_TARGET`.
2. A part of more bytes than the half of `capture_bytes` gives `IDEATEK_CAPTURE_STATUS_TOO_BIG`.
3. Else the capture is good.

A capture that is not good gets its answer at once: `docs/intern/engine_subsystems/rendering_vulkan/render_finish.c.md`.
For a good capture, `copy_part` records a copy of the part into the read buffer of the slot.
Then it records a barrier for the reads of the host. The capture keeps the slot.

## Functions

| Function | Result |
| --- | --- |
| `keep_request` | Keeps, waits again or frees 1 request. |
| `request` | The observer of `ideatek:CaptureRequest`. Gives 0. |
| `copy_part` | Records the copy of the part of 1 capture into the read buffer of the slot. |
| `get_status` | The status of 1 capture in the round, with its part in pixels. |
| `record_captures` | Answers the captures that are not good, and records the copy of the first good one. |

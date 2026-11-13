# src/engine_subsystems/rendering_vulkan/render_finish.c

This file ends the captures, and attaches `ideatek:Capture`.

`answer_capture` attaches the result of 1 capture to its entity, and frees its place. With `IDEATEK_CAPTURE_STATUS_OK`, it first copies the pixels from the read buffer into the half of the captures.
The halves turn at each good capture: the rule of the contract on the life of the bytes. A result that is not good has empty pixels and a size of 0.

`finish_captures` gives its pixels to each capture that copies in the slot of the round. That copy is of an earlier round, because `draw` copies only after this step.
The GPU finished it: `docs/intern/engine_subsystems/rendering_vulkan/render_device.c.md`, *The slots*.
So the answer depends on the rounds only, not on the speed of the GPU. It comes before `record_round` uses the read buffer of the slot again.

`fail_captures` runs when the device goes or changes: each capture that copies gets `IDEATEK_CAPTURE_STATUS_FAILED`.
`skip_captures` runs in a round with no image of the window: each capture that waits gets `IDEATEK_CAPTURE_STATUS_NO_TARGET`.

## Functions

| Function | Result |
| --- | --- |
| `answer_capture` | Attaches 1 result, and frees the place of the capture. |
| `finish_captures` | Gives their pixels to the captures that copied in the slot of the round. |
| `fail_captures` | Ends each capture that copies as failed. |
| `skip_captures` | Ends each capture that waits as without a target. |

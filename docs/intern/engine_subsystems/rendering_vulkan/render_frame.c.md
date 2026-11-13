# src/engine_subsystems/rendering_vulkan/render_frame.c

This file runs the system `draw`: the work of the module in each round.

`draw` does these steps:

1. The first round runs `start_render`. A broken module gives 1.
2. It reads `index` of `ideatek:Frame`, or 0 with no `ideatek:Frame`, and the device: `docs/intern/engine_subsystems/rendering_vulkan/render_device.c.md`. With no device, it stops there.
3. It fits the own image to the window: `docs/intern/engine_subsystems/rendering_vulkan/render_window.c.md`. With no image, it stops there.
4. It ends the captures of the slot of the round: `docs/intern/engine_subsystems/rendering_vulkan/render_finish.c.md`.
5. `record_round` records the command buffer of the slot, and `submit_round` submits it on `queue`.

`record_round` does these steps:

1. It clears the own image to black, then moves it to the layout of a color attachment.
2. Each camera of the window clears its viewport to black, in the order of `order`. A pass of dynamic rendering does it.
3. It moves the own image to the layout of a source of a copy. Then it records the copy of a capture: `docs/intern/engine_subsystems/rendering_vulkan/render_capture.c.md`.
4. It records the copy for the window.

The submit comes after the work of `begin` of the GPU module on the same queue, and before its work of `present`.
So the image of the swapchain is in the layout of a drawer when the copy starts, and the copy leaves it in that layout.

## Functions

| Function | Result |
| --- | --- |
| `clear_view` | Records the clear of the viewport of 1 camera. |
| `record_round` | Records the work of the round in 1 command buffer. |
| `submit_round` | Submits the command buffer on `queue`. |
| `draw` | The system of the phase `last`. A broken module gives 1; a working one gives 0. |

# src/engine_subsystems/rendering_vulkan/render_main.c

This file has `init`, `shutdown` and `reload`. The manifest of the module is in its own section of the binary.

`init` clears `render_module`, then makes the declarations and adds the system and the observer: `docs/intern/engine_subsystems/rendering_vulkan/render_declare.c.md`.
The settings come from Bridge only after `init`. So the first round of `draw` reads them: `docs/intern/engine_subsystems/rendering_vulkan/render_settings.c.md`.

`shutdown` waits until the device is idle. Then it frees each object of Vulkan of the module: `docs/intern/engine_subsystems/rendering_vulkan/render_device.c.md`.
`reload` gives 1, since the module states that it cannot reload.

## Functions

| Function | Result |
| --- | --- |
| `init` | 0. |
| `shutdown` | 0, after the objects of Vulkan are free. |
| `reload` | 1. |

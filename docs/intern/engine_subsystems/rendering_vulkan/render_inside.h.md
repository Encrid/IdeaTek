# src/engine_subsystems/rendering_vulkan/render_inside.h

This header holds what the sources of the module share: its state, its slots, its settings and its names.

## The state

The state is 1 global variable, `render_module`, since Bridge passes no argument that could hold it.
G4 of the style allows this 1 global, as it does for the other modules.

| Field | Value |
| --- | --- |
| `settings` | The values of the settings, as `render_settings_t`, kept at the start. |
| `device` | The handles of the device and the objects of the module on it, as `render_device_t`: `docs/intern/engine_subsystems/rendering_vulkan/render_device.c.md`. |
| `target` | The own image of the window, as `render_target_t`: `docs/intern/engine_subsystems/rendering_vulkan/render_targets.c.md`. |
| `captures` | The captures that wait or copy, as `render_capture_t`: `docs/intern/engine_subsystems/rendering_vulkan/render_capture.c.md`. |
| `cameras`, `camera_count` | The entities with `ideatek:Camera`, kept by the observer `cameras`. |
| `window` | The entity with `ideatek:VulkanSwapchain`, kept by the observer `window`. |
| `round` | `index` of `ideatek:Frame` in the round. |
| `slots`, `lists` | The slot of each declared type that the module uses, and the slot of the list of each observer. |
| `choices` | The index of the word of `diffuse`, `specular`, `shading` and `world_precision`. |
| `halves`, `side` | The 2 halves of `capture_bytes` in the persistent arena, and the half of the next capture. |
| `started`, `broken` | The settings were read; a wrong setting stops the module. |

## The settings

`render_settings_t` holds the 18 settings of `docs/usage/engine_subsystems/rendering_vulkan.md`, *The settings*.
Each number is a `double`, so Bridge fills it from an integer or a float, and 1 reader checks them all. The module turns a number into an integer where it uses it.
A choice of words is a `bridge_text_t`, and `occlusion` and `stats` are `bool`.

`render_number_t` is a row of the number table. It holds the name, the offset, the lowest and the highest value, and the text of the range.
It also tells whether the lowest value is excluded, and whether the value must be even.
`render_words_t` is a row of the word table: the name, the offset and the 2 words. `RENDER_AT` gives the offset of a field.

## The names

`CAMERA` to `SETTINGS` name the slots that `declare_types` keeps, and `SLOT_COUNT` is their count. `LIST_DEVICE` to `LIST_REQUESTS` name the observers.
`CAPTURE_FREE`, `CAPTURE_WAITING` and `CAPTURE_COPYING` are the states of a capture.
`RENDER_SLOTS` is 3, the most `frames_in_flight` of the GPU module. `RENDER_CAMERAS` is 64, and `RENDER_CAPTURES` is 16.

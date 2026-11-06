# src/platform_abstraction/window_sdl3/window_inside.h

Each source of the part `window_sdl3` includes this header.
It includes SDL3, BB-Native, `include/platform_abstraction/ideatek_window.h` and `include/platform_abstraction/ideatek_window_clipboard.h`, and the header of the part `sdl3_start`.
It gives the slots, the limits, the types of the module, and the functions that one source gives to another.

## The module

The part fills the role of the window module, with the clipboard: `docs/extern/platform_abstraction/window_sdl3/readme.md`.
Its identity is `ideatek_window_sdl3:window@0.1.0`, and its package is `BM-IdeaTek-Window-SDL3`.
It is pinned, because SDL3 drives the windows and the events on 1 thread, and macOS needs the main thread.
It is not reloadable, because SDL3 keeps the window outside of the storage of Bridge.

## The slots

Bridge gives the slots in the order of the declarations, from 0: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge_abi.h.md#bridge_entries_t.
Each slot is a `constexpr`, and `declare_types` of `docs/intern/platform_abstraction/window_sdl3/window_main.c.md` keeps this order:

| Slot | Name | Type |
| --- | --- | --- |
| 0 to 4 | No name | The field types: `ideatek:ideatek_entity_t` and the 4 enums of the window. |
| 5 | `REQUEST` | `ideatek:WindowRequest` |
| 6 | `STATE` | `ideatek:WindowState` |
| 7 | `DISPLAY` | `ideatek:Display` |
| 8 | `VIDEO_MODE` | `ideatek:VideoMode` |
| 9 | `DROP` | `ideatek:Drop` |
| 10 | `CLIPBOARD` | `ideatek:ClipboardRequest` |
| 11 | `PASTE` | `ideatek:Paste` |
| 12 | `EVENT_WAIT` | `ideatek:EventWait` |
| 13 | `SETTINGS` | `ideatek_window_sdl3:Settings` |

## The limits

| Constant | Value | Limit |
| --- | --- | --- |
| `WINDOW_SPAWNS` | 256 | The most entities that the module spawns in 1 round. The manifest states it as `spawns`. |
| `WINDOW_DISPLAYS` | 16 | The most displays that get an entity. |
| `WINDOW_MODES` | 255 | The most video modes of 1 display. With its display, they fit in the spawns of 1 round. |
| `WINDOW_PEEKS` | 64 | The most key presses that the module reads in 1 round to find a paste. |
| `WINDOW_REASON` | 256 | The size of the buffer for the reason of a failure of SDL3. |

## The settings

`ideatek_window_sdl3:Settings` is the component of the settings: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#the-settings-of-a-module.
It is a singleton of the namespace of the module, so it needs no grant.
The macro `WINDOW_SETTINGS_FIELDS` gives its fields, and the header gives its C type and its descriptor with the macros of `bridge_schema.h`:

| Field | C type | Setting |
| --- | --- | --- |
| `icon` | `bridge_text_t` | `icon`. Bridge copies the text into the persistent share of the module. |
| `drop_bytes` | `uint32_t` | `drop_bytes`. |
| `clipboard_bytes` | `uint32_t` | `clipboard_bytes`. |
| `close_on_request` | `bool` | `close_on_request`. |

The defaults and the ranges are in `docs/extern/platform_abstraction/ideatek_window.h.md`, *The settings of the window module*, and `docs/extern/platform_abstraction/ideatek_window_clipboard.h.md`, *The bytes of the pastes*.

## The types

| Type | Field | Value |
| --- | --- | --- |
| `window_half_t` | `at` | Where the bytes of the 2 halves start, in the shared memory. 0 before the first round. |
| `window_half_t` | `size` | The bytes of 1 half. |
| `window_half_t` | `used` | The bytes of the half of the round that hold an item. |
| `window_half_t` | `side` | 0 or 1: the half of the round. |
| `window_mode_t` | `mode` | A video mode, as SDL3 gives it. |
| `window_mode_t` | `entity` | The entity of `ideatek:VideoMode` for the mode. |
| `window_display_t` | `id` | The `SDL_DisplayID` of the display. |
| `window_display_t` | `entity` | The entity of `ideatek:Display` for the display. |
| `window_display_t` | `modes` | The video modes of the display, `mode_count` of them. |
| `window_display_t` | `listed` | How many modes of `modes` are in the list of the exclusive modes of SDL3. They come first. |
| `window_display_t` | `attached` | The last value of `ideatek:Display` that the module attached. |
| `window_driver_t` | `name` | A name that `SDL_GetCurrentVideoDriver` gives, or null for each other name. |
| `window_driver_t` | `surface` | The `ideatek_surface_t` of the windows of that driver. |
| `window_driver_t` | `flags` | The flags of `SDL_CreateWindow` that the driver needs. |
| `window_driver_t` | `paste` | The key modifier of a paste on that system: `SDL_KMOD_GUI` or `SDL_KMOD_CTRL`. |
| `window_driver_t` | `display_key` | The property of the window that gives `native_display`, or null. |
| `window_driver_t` | `window_key` | The property of the window that gives `native_window`, or null. |

`docs/intern/platform_abstraction/window_sdl3/window_state.c.md`, *The driver*, gives the rows of `window_driver_t`.

## The state of the module

The module keeps 1 `window_module_t`, the global variable `window_module` of `window_main.c`.
The exports of a module get only the API of Bridge, so the state of SDL3 between 2 calls is in this variable.

| Field | Value |
| --- | --- |
| `window` | The `SDL_Window` of the module, or null. |
| `view` | The Metal view of the window on macOS, or null. |
| `driver` | The row of the video driver: *The types*. |
| `entity` | The entity of the window: the first entity that got `ideatek:WindowRequest`. 0 before it. |
| `applied` | The last `ideatek:WindowRequest` that the module applied. |
| `state` | The last `ideatek:WindowState` that the module attached. |
| `pending` | `true` when the request changed and the next round must apply it. |
| `moved` | `true` when the display of the window went, and the next round must apply the request again. |
| `dirty` | `true` when an event or a change can change `ideatek:WindowState`. |
| `close_asked` | The value of `close_asked` for the round. |
| `drop_over`, `drop_x` and `drop_y` | The drag over the window, for the fields of the same names of `ideatek:WindowState`. |
| `displays` | The displays that have an entity, `display_count` of them. |
| `displays_dirty` | `true` when an event of a display came, or a display did not fit in a round. |
| `drops` | The `window_half_t` of `drop_bytes`. |
| `pastes` | The `window_half_t` of `clipboard_bytes`. |
| `items` | The entities of `ideatek:Drop` and `ideatek:Paste` of the round, `item_count` of them. |
| `spawned` | The entities that the module spawned in the round. |
| `set` | The number of the last drag. |
| `index` | The place of the next item in the drag. |
| `paste_count` | The pastes that wait for an entity. |
| `taken` | The time of SDL3 in nanoseconds when the module took the events of the last round. 0 before the first round. |

## The messages

Each source writes its messages with `bridge_native_say` of BB-Native.
Thus `bridge_native_say` is the writer of the messages of the code: `docs/contributing/tests.md`, *The messages of the code*.
The readme of the part quotes each message: `docs/extern/platform_abstraction/window_sdl3/readme.md`, *The messages*.

## Functions

The header gives no function of its own. It declares the functions that one source gives and another source calls. The document of each source gives their results.

| Function | Source |
| --- | --- |
| `enable_events` | `window_events.c` |
| `apply_request` | `window_request.c` |
| `get_target` | `window_request.c` |
| `open_window` | `window_open.c` |
| `set_mode` | `window_open.c` |
| `close_window` | `window_open.c` |
| `find_driver` | `window_state.c` |
| `read_handles` | `window_state.c` |
| `attach_state` | `window_state.c` |
| `sync_displays` | `window_displays.c` |
| `find_display` | `window_displays.c` |
| `read_modes` | `window_video_modes.c` |
| `spawn_modes` | `window_video_modes.c` |
| `destroy_modes` | `window_video_modes.c` |
| `find_mode_entity` | `window_video_modes.c` |
| `find_mode` | `window_video_modes.c` |
| `take_drops` | `window_drops.c` |
| `count_pastes` | `window_clipboard.c` |
| `spawn_pastes` | `window_clipboard.c` |
| `start_items` | `window_items.c` |
| `turn_items` | `window_items.c` |
| `put_bytes` | `window_items.c` |
| `spawn_item` | `window_items.c` |
| `cut_text` | `window_text.c` |
| `copy_arena_text` | `window_text.c` |
| `get_cell` | `window_world.c` |
| `spawn_entity` | `window_world.c` |
| `has_room` | `window_world.c` |

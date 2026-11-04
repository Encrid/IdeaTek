# include/platform_abstraction/window/ideatek_window.h

`include/platform_abstraction/window/ideatek_window.h` gives the types of the window and of the displays: their C types, their constants and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes `include/platform_abstraction/world/ideatek_world.h` for `ideatek_entity_t`, which `docs/extern/platform_abstraction/world/ideatek_world.h.md` describes.

## The window and the displays

- The game has 1 window, and the window is an entity.
- Each display is an entity, and each video mode of a display is an entity.
- Each item that the user drops on the window is an entity.
- A place and a size are in units of the desktop. The origin is the top left corner of the primary display, and +Y points down.
- A size in pixels is a size in units of the desktop, times the density of the pixels.

## The roles

- The game writes `ideatek:WindowRequest`.
- The window module reads `ideatek:WindowRequest`. It writes `ideatek:WindowState`, `ideatek:Display`, `ideatek:VideoMode` and `ideatek:Drop`. A deployment has one window module.
- A reader reads the types of this header. Render, input and the UI are readers.

The game spawns an entity, and attaches `ideatek:WindowRequest` to it.
The window module opens the window of the first entity that gets `ideatek:WindowRequest`, and attaches `ideatek:WindowState` to it.
For each other entity that gets `ideatek:WindowRequest`, it writes 1 line in the log, and opens no window.
When the game attaches `ideatek:WindowRequest` again, the window module applies each field that changed.
If the system refuses a change, `ideatek:WindowState` keeps what the window is.
The window module attaches `ideatek:WindowState` again only when a byte of it changed.

## The displays

The window module spawns an entity for each display when it starts, and when the system adds a display.
It attaches `ideatek:Display` to the entity, and spawns an entity with `ideatek:VideoMode` for each video mode of the display.
When the system removes a display, the window module destroys the entity of the display and the entities of its video modes.
Exactly 1 display entity has `is_primary` set to `true`.

If the display of `IDEATEK_DISPLAY_TARGET_CHOSEN` goes, the window module moves the window to the primary display.
It does not move the window back when the display comes back.

## The life cycle

1. A close request comes when the user closes the window. A quit request from the system is a close request on the window.
2. If `close_on_request` is `true`, the window module closes the window, and its system returns `BRIDGE_END`.
3. If `close_on_request` is `false`, `close_asked` is `true` only in the round after the request. The game ends the run when a system of the game returns `BRIDGE_END`. The game cancels the close when it does nothing.
4. The host owns the signals of the system.
5. At the end of the run, Bridge calls `shutdown` of each module. Each module saves its rows there.

The game closes the window when it destroys its entity. The window module then returns `BRIDGE_END`.

## The events of the system

The window module takes the events of the system in the first phase of each round.
It runs after the clock module, and before each other reader of the events.
When the `wait` of `ideatek:Frame` is more than 0, it waits for an event of the system, for at most `wait`.
It waits until `wait` passed since it took the events of the last round, or until an event comes.
`docs/extern/kernel/frame/ideatek_frame.h.md`, *The limit of the rate*, tells when `wait` is more than 0.

## The drops

The user can drag files or a text from another program, and drop them on the window.

- While the user drags over the window, `drop_over` of `ideatek:WindowState` is `true`, and `drop_x` and `drop_y` tell the point.
- For each dropped item, the window module spawns an entity with `ideatek:Drop`. A reader hears it with an observer of `BRIDGE_ON_ENTER`.
- The window module destroys each entity of `ideatek:Drop` in the next round. A reader that keeps the item copies its bytes.

At `init`, the window module allocates `drop_bytes` bytes in its share of the persistent arena.
It uses one half of these bytes in a round, and the other half in the next round.
Thus, the bytes of a drop stay good while its entity lives.
If an item does not fit in the free part of the half, the window module does not spawn its entity.
It then writes 1 line in the log.

The `persistent` of the manifest of the window module must hold `drop_bytes`.
A path names a file of the system, in UTF-8. It can name any file that the user can read.

## The settings of the window module

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `close_on_request` | `true` | `true` or `false` | `true` closes the window on a close request. |
| `icon` | `"none"` | a path or `"none"` | The path of a PNG file, which is the icon of the window. `"none"` keeps the icon of the system. |
| `drop_bytes` | 65536 | 2 to 1073741824, an even number | The bytes for the dropped items. Each round uses at most the half. |

If the window module cannot load `icon`, it writes 1 line in the log, and opens the window with the icon of the system.
The icon does not change while the game runs.

## ideatek:WindowRequest

`ideatek:WindowRequest` is the window that the game asks for.
Its C type `ideatek_window_request_t` takes 160 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `title` | `char[128]` | 0 | The title, in UTF-8, with a NUL after its last byte. |
| `display` | `ideatek_entity_t` | 128 | With `IDEATEK_DISPLAY_TARGET_CHOSEN`: the display entity of the window. |
| `video_mode` | `ideatek_entity_t` | 136 | With `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN`: the video mode entity. 0 is the current mode of the display. |
| `width` | `uint32_t` | 144 | The width. |
| `height` | `uint32_t` | 148 | The height. |
| `mode` | `ideatek_window_mode_t` | 152 | The mode of the window. |
| `target` | `ideatek_display_target_t` | 153 | The display that shows the window. |

The bytes 154 to 159 hold no field, because the size of the type is a multiple of 8.

- No OS bounds a title, so 128 bytes is the size of the contract. The window module cuts a longer title at its last whole code point and writes 1 line in the log.
- If the video mode is dead, or is not a mode of the display, the window module uses `IDEATEK_WINDOW_MODE_FULLSCREEN`. It writes 1 line in the log.
- If the system refuses the video mode, or has no exclusive mode, the window module also uses `IDEATEK_WINDOW_MODE_FULLSCREEN`. It writes 1 line in the log.
- If the display of `IDEATEK_DISPLAY_TARGET_CHOSEN` is dead, the window module uses the primary display.

### ideatek:ideatek_window_mode_t

`ideatek_window_mode_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The mode |
| --- | --- | --- |
| `IDEATEK_WINDOW_MODE_WINDOWED` | 0 | A window with the size of `width` and `height`. |
| `IDEATEK_WINDOW_MODE_FULLSCREEN` | 1 | The window fills all the display, with no border. The display keeps its current mode. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` | 2 | The window fills all the display, and sets the display to `video_mode` while it has the focus. |

### ideatek:ideatek_display_target_t

`ideatek_display_target_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The display |
| --- | --- | --- |
| `IDEATEK_DISPLAY_TARGET_DEFAULT` | 0 | The primary display. |
| `IDEATEK_DISPLAY_TARGET_CHOSEN` | 1 | The display entity of `display`. |

## ideatek:WindowState

`ideatek:WindowState` is the window that the system gives.
Its C type `ideatek_window_state_t` takes 80 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `native_display` | `uint64_t` | 0 | The first handle of the window: see `ideatek_surface_t`. |
| `native_window` | `uint64_t` | 8 | The second handle of the window: see `ideatek_surface_t`. |
| `display` | `ideatek_entity_t` | 16 | The display entity that shows the window. |
| `video_mode` | `ideatek_entity_t` | 24 | With `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN`: the video mode entity that the display uses. Else 0. |
| `width` | `uint32_t` | 32 | The width of the area that render draws into. |
| `height` | `uint32_t` | 36 | The height of the area that render draws into. |
| `pixel_width` | `uint32_t` | 40 | `width`, in pixels. |
| `pixel_height` | `uint32_t` | 44 | `height`, in pixels. |
| `display_scale` | `float` | 48 | The scale that the system asks for the content of the window. |
| `sdr_white_level` | `float` | 52 | The level of the white of SDR content, relative to the white of HDR content. |
| `hdr_headroom` | `float` | 56 | The brightest level that the display shows, relative to `sdr_white_level`. |
| `drop_x` | `float` | 60 | While `drop_over` is `true`: the left edge of the point of the drag, from the left edge of the area. 0 until the system gives the first point. |
| `drop_y` | `float` | 64 | While `drop_over` is `true`: the top edge of the point of the drag, from the top edge of the area. 0 until the system gives the first point. |
| `mode` | `ideatek_window_mode_t` | 68 | The mode of the window. |
| `surface` | `ideatek_surface_t` | 69 | The kind of the handles. |
| `focused` | `bool` | 70 | `true` while the window gets the keys. |
| `minimized` | `bool` | 71 | `true` while the window is hidden in the task bar of the system. |
| `hdr` | `bool` | 72 | `true` while the window can show HDR content. |
| `close_asked` | `bool` | 73 | `true` in the round after a close request, when `close_on_request` is `false`. |
| `drop_over` | `bool` | 74 | `true` while the user drags files or a text over the window. |

The bytes 75 to 79 hold no field, because the size of the type is a multiple of 8.

- The density of the pixels is `pixel_width` divided by `width`.
- When `hdr` is `false`, `sdr_white_level` and `hdr_headroom` are 1.0.
- The render module asks for HDR output. The window module only tells what the system gives.

### ideatek:ideatek_surface_t

`ideatek_surface_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.
A handle that the table does not name is 0.

| Constant | Value | `native_display` | `native_window` |
| --- | --- | --- | --- |
| `IDEATEK_SURFACE_NONE` | 0 | The window module shares no handle. | The window module shares no handle. |
| `IDEATEK_SURFACE_WINDOWS` | 1 | The `HINSTANCE` of the program. | The `HWND` of the window. |
| `IDEATEK_SURFACE_METAL` | 2 | No handle. | The `CAMetalLayer` of the window. |
| `IDEATEK_SURFACE_XLIB` | 3 | The `Display *` of Xlib. | The `Window` of Xlib. |
| `IDEATEK_SURFACE_XCB` | 4 | The `xcb_connection_t *` of XCB. | The `xcb_window_t` of XCB. |
| `IDEATEK_SURFACE_WAYLAND` | 5 | The `wl_display *` of Wayland. | The `wl_surface *` of the window. |
| `IDEATEK_SURFACE_ANDROID` | 6 | No handle. | The `ANativeWindow *` of the window. |
| `IDEATEK_SURFACE_WEB` | 7 | No handle. | No handle. The window is the canvas of the page that runs the program. |

A window module that also renders states `IDEATEK_SURFACE_NONE`.

## ideatek:Display

`ideatek:Display` is a display that the system gives.
Its C type `ideatek_display_t` takes 160 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `char[128]` | 0 | The name of the display, in UTF-8, with a NUL after its last byte. |
| `desktop_mode` | `ideatek_entity_t` | 128 | The video mode entity of the desktop. |
| `current_mode` | `ideatek_entity_t` | 136 | The video mode entity that the display uses now. |
| `width` | `uint32_t` | 144 | The width, in units of the desktop. |
| `height` | `uint32_t` | 148 | The height, in units of the desktop. |
| `content_scale` | `float` | 152 | The scale that the user set for the content of the display. |
| `hdr` | `bool` | 156 | `true` if the system tells that the display shows HDR content. |
| `is_primary` | `bool` | 157 | `true` if the display is the primary display of the system. |

The bytes 158 and 159 hold no field, because the size of the type is a multiple of 8.
The name of a display has no bound in macOS, so its 128 bytes are the size of the contract. If a name does not fit, the window module keeps the code points that fit whole, and it writes 1 line in the log.
Some systems do not tell `hdr` for a display: `hdr` of `ideatek:WindowState` is the value to use.

## ideatek:VideoMode

`ideatek:VideoMode` is a video mode of a display.
Its C type `ideatek_video_mode_t` takes 32 bytes, and its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `display` | `ideatek_entity_t` | 0 | The display entity of the mode. |
| `width` | `uint32_t` | 8 | The width, in units of the desktop. |
| `height` | `uint32_t` | 12 | The height, in units of the desktop. |
| `pixel_density` | `float` | 16 | The number of pixels in 1 unit of the desktop. |
| `refresh_numerator` | `int32_t` | 20 | The numerator of the refresh rate, in Hz. |
| `refresh_denominator` | `int32_t` | 24 | The denominator of the refresh rate. |
| `order` | `uint32_t` | 28 | The place of the mode in the list of the system. The largest mode is 0. |

A refresh rate of 0 Hz tells that the system does not give the refresh rate.

## ideatek:Drop

`ideatek:Drop` is an item that the user dropped on the window: *The drops*.
Its C type `ideatek_drop_t` takes 48 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window entity that got the item. |
| `data` | `bridge_text_t` | 8 | With `IDEATEK_DROP_FILE`, the path of the file. With `IDEATEK_DROP_TEXT`, the text. In UTF-8, with no NUL. |
| `source` | `bridge_text_t` | 16 | The name of the program that sent the item, in UTF-8. Its `len` is 0 if the system does not tell it. |
| `set` | `uint32_t` | 24 | The number of the drag. The items of 1 drag have the same `set`. The first drag is 1. |
| `index` | `uint32_t` | 28 | The place of the item in its drag. The first item is 0. |
| `x` | `float` | 32 | The left edge of the point of the drop, from the left edge of the area of the window. |
| `y` | `float` | 36 | The top edge of the point of the drop, from the top edge of the area of the window. |
| `kind` | `ideatek_drop_kind_t` | 40 | The kind of the item. |

The bytes 41 to 47 hold no field, because the size of the type is a multiple of 8.

### ideatek:ideatek_drop_kind_t

`ideatek_drop_kind_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The item |
| --- | --- | --- |
| `IDEATEK_DROP_FILE` | 0 | A file. |
| `IDEATEK_DROP_TEXT` | 1 | A text. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_window_mode_t` | `ideatek_window_mode_t` | `ideatek_window_mode_t_desc` | `IDEATEK_WINDOW_MODE_VALUES` |
| `ideatek:ideatek_display_target_t` | `ideatek_display_target_t` | `ideatek_display_target_t_desc` | `IDEATEK_DISPLAY_TARGET_VALUES` |
| `ideatek:ideatek_surface_t` | `ideatek_surface_t` | `ideatek_surface_t_desc` | `IDEATEK_SURFACE_VALUES` |
| `ideatek:ideatek_drop_kind_t` | `ideatek_drop_kind_t` | `ideatek_drop_kind_t_desc` | `IDEATEK_DROP_KIND_VALUES` |
| `ideatek:WindowRequest` | `ideatek_window_request_t` | `ideatek_window_request_t_desc` | `IDEATEK_WINDOW_REQUEST_FIELDS` |
| `ideatek:WindowState` | `ideatek_window_state_t` | `ideatek_window_state_t_desc` | `IDEATEK_WINDOW_STATE_FIELDS` |
| `ideatek:Display` | `ideatek_display_t` | `ideatek_display_t_desc` | `IDEATEK_DISPLAY_FIELDS` |
| `ideatek:VideoMode` | `ideatek_video_mode_t` | `ideatek_video_mode_t_desc` | `IDEATEK_VIDEO_MODE_FIELDS` |
| `ideatek:Drop` | `ideatek_drop_t` | `ideatek_drop_t_desc` | `IDEATEK_DROP_FIELDS` |

```text
{ IDEATEK_WINDOW_MODE_WINDOWED = 0, IDEATEK_WINDOW_MODE_FULLSCREEN = 1, IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN = 2 }
{ IDEATEK_DISPLAY_TARGET_DEFAULT = 0, IDEATEK_DISPLAY_TARGET_CHOSEN = 1 }
{ IDEATEK_SURFACE_NONE = 0, IDEATEK_SURFACE_WINDOWS = 1, IDEATEK_SURFACE_METAL = 2, IDEATEK_SURFACE_XLIB = 3, IDEATEK_SURFACE_XCB = 4, IDEATEK_SURFACE_WAYLAND = 5, IDEATEK_SURFACE_ANDROID = 6, IDEATEK_SURFACE_WEB = 7 }
{ IDEATEK_DROP_FILE = 0, IDEATEK_DROP_TEXT = 1 }
{ char title[128]; ideatek_entity_t display; ideatek_entity_t video_mode; uint32_t width; uint32_t height; ideatek_window_mode_t mode; ideatek_display_target_t target; }
{ uint64_t native_display; uint64_t native_window; ideatek_entity_t display; ideatek_entity_t video_mode; uint32_t width; uint32_t height; uint32_t pixel_width; uint32_t pixel_height; float display_scale; float sdr_white_level; float hdr_headroom; float drop_x; float drop_y; ideatek_window_mode_t mode; ideatek_surface_t surface; bool focused; bool minimized; bool hdr; bool close_asked; bool drop_over; }
{ char name[128]; ideatek_entity_t desktop_mode; ideatek_entity_t current_mode; uint32_t width; uint32_t height; float content_scale; bool hdr; bool is_primary; }
{ ideatek_entity_t display; uint32_t width; uint32_t height; float pixel_density; int32_t refresh_numerator; int32_t refresh_denominator; uint32_t order; }
{ ideatek_entity_t window; bridge_text_t data; bridge_text_t source; uint32_t set; uint32_t index; float x; float y; ideatek_drop_kind_t kind; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_window_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_display_target_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_surface_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_drop_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Display` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VideoMode` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Drop` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares each field type before the struct that has a field of the type.
`bridge_text_t` is a type of Bridge, and no module declares it.

## The grants

The field types are `ideatek:ideatek_entity_t` and the four enums.

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:WindowRequest` | The field types |
| The window module | `ideatek:WindowState`, `ideatek:Display`, `ideatek:VideoMode` and `ideatek:Drop` | `ideatek:WindowRequest` and the field types |
| A reader | No type. | The types that it reads, and the field types |

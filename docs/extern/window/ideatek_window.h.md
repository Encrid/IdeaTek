# include/window/ideatek_window.h

`include/window/ideatek_window.h` gives the types of the windows and of the displays: their C types, their constants and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes `include/space/ideatek_space.h` for `ideatek_entity_t`, which `docs/extern/space/ideatek_space.h.md` describes.

## The windows and the displays

- Each window is an entity. A game has 1 window or more.
- `ideatek:MainWindow` marks the main window. At most 1 entity carries it.
- Each display is an entity, and each video mode of a display is an entity.
- `ideatek:PrimaryDisplay` marks the primary display of the system. Exactly 1 display entity carries it.
- A place and a size are in units of the desktop. The origin is the top left corner of the primary display, and +Y points down.
- A size in pixels is a size in units of the desktop, times the density of the pixels.

## The roles

- The game writes `ideatek:WindowRequest` and `ideatek:MainWindow`.
- The window module reads `ideatek:WindowRequest` and `ideatek:MainWindow`. It writes `ideatek:WindowState`, `ideatek:Display`, `ideatek:VideoMode` and `ideatek:PrimaryDisplay`. A deployment has one window module.
- A reader reads the types of this header. Render, input and the UI are readers.

The game spawns an entity, and attaches `ideatek:WindowRequest` to it.
The window module opens a window for each entity that carries `ideatek:WindowRequest`, and attaches `ideatek:WindowState` to it.
When the game attaches `ideatek:WindowRequest` again, the window module applies each field that changed.
If the system refuses a change, `ideatek:WindowState` keeps what the window is.
The window module attaches `ideatek:WindowState` again only when a byte of it changed.
The game closes a window when it destroys its entity.

## The displays

The window module spawns an entity for each display when it starts, and when the system adds a display.
It attaches `ideatek:Display` to the entity, and spawns an entity with `ideatek:VideoMode` for each video mode of the display.
When the system removes a display, the window module destroys the entity of the display and the entities of its video modes.
When the system changes its primary display, the window module moves `ideatek:PrimaryDisplay` to the new primary display.

## The close of a window

A close request comes when the user closes a window. `SDL_EVENT_QUIT` is a close request on each window.

- If `close_on_request` is `true`, the window module closes the window, and destroys its entity.
- If `close_on_request` is `false`, the window module sets `close_asked`, and the window stays open.

The window module ends the run when the condition of `end_on` is true.
Its system then returns `BRIDGE_END`.

## The settings of the window module

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `close_on_request` | `true` | `true` or `false` | `true` closes a window on a close request. |
| `end_on` | `"main"` | `"main"`, `"all"` or `"never"` | `"main"` ends the run when the main window closes. `"all"` ends the run when the last window closes. `"never"` does not end the run. |
| `icon` | `"none"` | a path or `"none"` | The path of a PNG file, which is the icon of each window. `"none"` keeps the icon of the system. |
| `screen_saver` | `false` | `true` or `false` | `true` lets the screen saver of the system start while the game runs. |

If the window module cannot load `icon`, it writes 1 line in the log, and opens the windows with the icon of the system.
The icon is the same for each window, and does not change while the game runs.

## ideatek:WindowRequest

`ideatek:WindowRequest` is the window that the game asks for.
Its C type `ideatek_window_request_t` takes 192 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `title` | `char[128]` | 0 | The title, in UTF-8, with a NUL after its last byte. |
| `display` | `ideatek_entity_t` | 128 | The display entity of the window. 0 is the primary display. |
| `video_mode` | `ideatek_entity_t` | 136 | With `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN`: the video mode entity. 0 is the current mode of `display`. |
| `x` | `int32_t` | 144 | With `IDEATEK_PLACE_AT`: the left edge. |
| `y` | `int32_t` | 148 | With `IDEATEK_PLACE_AT`: the top edge. |
| `width` | `uint32_t` | 152 | The width. |
| `height` | `uint32_t` | 156 | The height. |
| `min_width` | `uint32_t` | 160 | The smallest width. |
| `min_height` | `uint32_t` | 164 | The smallest height. |
| `max_width` | `uint32_t` | 168 | The largest width. 0 has no limit. |
| `max_height` | `uint32_t` | 172 | The largest height. 0 has no limit. |
| `flashes` | `uint32_t` | 176 | A count. Each change of the count applies `flash`. |
| `raises` | `uint32_t` | 180 | A count. Each change of the count raises the window, and gives it the focus. |
| `place` | `ideatek_place_t` | 184 | How the window module places the window. |
| `mode` | `ideatek_window_mode_t` | 185 | The mode of the window. |
| `flash` | `ideatek_flash_t` | 186 | How the window flashes. |
| `resizable` | `bool` | 187 | `true` lets the user change the size. |
| `borderless` | `bool` | 188 | `true` shows no border and no title bar. |
| `visible` | `bool` | 189 | `true` shows the window. |
| `always_on_top` | `bool` | 190 | `true` keeps the window above the other windows. |

The byte 191 holds no field, because the size of the type is a multiple of 8.

- A title that is longer than 127 bytes is cut before its first UTF-8 character that does not fit. The window module writes 1 line in the log.
- The window module keeps the size between the smallest size and the largest size.
- If the video mode is dead, or is not a mode of `display`, the window module uses `IDEATEK_WINDOW_MODE_FULLSCREEN`. It writes 1 line in the log.
- If the system refuses the video mode, the window module also uses `IDEATEK_WINDOW_MODE_FULLSCREEN`.

### ideatek:ideatek_place_t

`ideatek_place_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The place |
| --- | --- | --- |
| `IDEATEK_PLACE_SYSTEM` | 0 | The system chooses the place. |
| `IDEATEK_PLACE_CENTER` | 1 | The center of `display`. |
| `IDEATEK_PLACE_AT` | 2 | `x` and `y`. |

### ideatek:ideatek_window_mode_t

`ideatek_window_mode_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The mode |
| --- | --- | --- |
| `IDEATEK_WINDOW_MODE_WINDOWED` | 0 | A window with the size of `width` and `height`. |
| `IDEATEK_WINDOW_MODE_MINIMIZED` | 1 | The window is hidden in the task bar of the system. |
| `IDEATEK_WINDOW_MODE_MAXIMIZED` | 2 | The window fills the display, except the bars of the system. |
| `IDEATEK_WINDOW_MODE_FULLSCREEN` | 3 | The window fills all the display, with no border. The display keeps its current mode. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` | 4 | The window fills all the display, and sets the display to `video_mode` while it has the focus. |

### ideatek:ideatek_flash_t

`ideatek_flash_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The flash |
| --- | --- | --- |
| `IDEATEK_FLASH_CANCEL` | 0 | Stops the flash. |
| `IDEATEK_FLASH_BRIEFLY` | 1 | Flashes the window 1 time. |
| `IDEATEK_FLASH_UNTIL_FOCUSED` | 2 | Flashes the window until it gets the focus. |

## ideatek:WindowState

`ideatek:WindowState` is the window that the system gives.
Its C type `ideatek_window_state_t` takes 112 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `native_display` | `uint64_t` | 0 | The first handle of the window: see `ideatek_surface_t`. |
| `native_window` | `uint64_t` | 8 | The second handle of the window: see `ideatek_surface_t`. |
| `display` | `ideatek_entity_t` | 16 | The display entity that shows the window. |
| `video_mode` | `ideatek_entity_t` | 24 | With `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN`: the video mode entity that the display uses. Else 0. |
| `x` | `int32_t` | 32 | The left edge. |
| `y` | `int32_t` | 36 | The top edge. |
| `width` | `uint32_t` | 40 | The width of the area that render draws into. |
| `height` | `uint32_t` | 44 | The height of the area that render draws into. |
| `pixel_width` | `uint32_t` | 48 | `width`, in pixels. |
| `pixel_height` | `uint32_t` | 52 | `height`, in pixels. |
| `border_top` | `int32_t` | 56 | The size of the border above the area. |
| `border_left` | `int32_t` | 60 | The size of the border at the left of the area. |
| `border_bottom` | `int32_t` | 64 | The size of the border below the area. |
| `border_right` | `int32_t` | 68 | The size of the border at the right of the area. |
| `safe_x` | `int32_t` | 72 | The left edge of the safe area, from the left edge of the area. |
| `safe_y` | `int32_t` | 76 | The top edge of the safe area, from the top edge of the area. |
| `safe_width` | `uint32_t` | 80 | The width of the safe area. |
| `safe_height` | `uint32_t` | 84 | The height of the safe area. |
| `display_scale` | `float` | 88 | The scale that the system asks for the content of the window. |
| `sdr_white_level` | `float` | 92 | The level of the white of SDR content, relative to the white of HDR content. |
| `hdr_headroom` | `float` | 96 | The brightest level that the display shows, relative to `sdr_white_level`. |
| `mode` | `ideatek_window_mode_t` | 100 | The mode of the window. |
| `surface` | `ideatek_surface_t` | 101 | The kind of the handles. |
| `focused` | `bool` | 102 | `true` while the window gets the keys. |
| `mouse_inside` | `bool` | 103 | `true` while the mouse is in the window. |
| `occluded` | `bool` | 104 | `true` while other windows hide all the window. |
| `visible` | `bool` | 105 | `true` while the window is shown. |
| `resizable` | `bool` | 106 | `true` while the user can change the size. |
| `borderless` | `bool` | 107 | `true` while the window shows no border. |
| `always_on_top` | `bool` | 108 | `true` while the window stays above the other windows. |
| `hdr` | `bool` | 109 | `true` while the window can show HDR content. |
| `close_asked` | `bool` | 110 | `true` after a close request, when `close_on_request` is `false`. |

The byte 111 holds no field, because the size of the type is a multiple of 8.

- The density of the pixels is `pixel_width` divided by `width`.
- The safe area is the part of the area that no notch and no rounded corner hides. Without a notch, it is the full area.
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
Its C type `ideatek_display_t` takes 184 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `char[128]` | 0 | The name of the display, in UTF-8, with a NUL after its last byte. |
| `desktop_mode` | `ideatek_entity_t` | 128 | The video mode entity of the desktop. |
| `current_mode` | `ideatek_entity_t` | 136 | The video mode entity that the display uses now. |
| `x` | `int32_t` | 144 | The left edge. |
| `y` | `int32_t` | 148 | The top edge. |
| `width` | `uint32_t` | 152 | The width. |
| `height` | `uint32_t` | 156 | The height. |
| `usable_x` | `int32_t` | 160 | The left edge of the part that the bars of the system do not cover. |
| `usable_y` | `int32_t` | 164 | The top edge of the part that the bars of the system do not cover. |
| `usable_width` | `uint32_t` | 168 | The width of the part that the bars of the system do not cover. |
| `usable_height` | `uint32_t` | 172 | The height of the part that the bars of the system do not cover. |
| `content_scale` | `float` | 176 | The scale that the user set for the content of the display. |
| `orientation` | `ideatek_orientation_t` | 180 | The orientation of the display now. |
| `natural_orientation` | `ideatek_orientation_t` | 181 | The orientation of the display when it is not turned. |
| `hdr` | `bool` | 182 | `true` if the system tells that the display shows HDR content. |

The byte 183 holds no field, because the size of the type is a multiple of 8.
Some systems do not tell `hdr` for a display: `hdr` of `ideatek:WindowState` is the value to use.

### ideatek:ideatek_orientation_t

`ideatek_orientation_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The orientation |
| --- | --- | --- |
| `IDEATEK_ORIENTATION_UNKNOWN` | 0 | The system does not tell the orientation. |
| `IDEATEK_ORIENTATION_LANDSCAPE` | 1 | Wider than high. |
| `IDEATEK_ORIENTATION_LANDSCAPE_FLIPPED` | 2 | Wider than high, turned by 180 degrees. |
| `IDEATEK_ORIENTATION_PORTRAIT` | 3 | Higher than wide. |
| `IDEATEK_ORIENTATION_PORTRAIT_FLIPPED` | 4 | Higher than wide, turned by 180 degrees. |

## ideatek:VideoMode

`ideatek:VideoMode` is a video mode of a display.
`ideatek_video_mode_t` is its C type: 32 bytes, aligned to 8.

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

## ideatek:MainWindow and ideatek:PrimaryDisplay

`ideatek:MainWindow` and `ideatek:PrimaryDisplay` are tags: they have no bytes, no C type and no descriptor.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_place_t` | `ideatek_place_t` | `ideatek_place_t_desc` | `IDEATEK_PLACE_VALUES` |
| `ideatek:ideatek_window_mode_t` | `ideatek_window_mode_t` | `ideatek_window_mode_t_desc` | `IDEATEK_WINDOW_MODE_VALUES` |
| `ideatek:ideatek_surface_t` | `ideatek_surface_t` | `ideatek_surface_t_desc` | `IDEATEK_SURFACE_VALUES` |
| `ideatek:ideatek_flash_t` | `ideatek_flash_t` | `ideatek_flash_t_desc` | `IDEATEK_FLASH_VALUES` |
| `ideatek:ideatek_orientation_t` | `ideatek_orientation_t` | `ideatek_orientation_t_desc` | `IDEATEK_ORIENTATION_VALUES` |
| `ideatek:WindowRequest` | `ideatek_window_request_t` | `ideatek_window_request_t_desc` | `IDEATEK_WINDOW_REQUEST_FIELDS` |
| `ideatek:WindowState` | `ideatek_window_state_t` | `ideatek_window_state_t_desc` | `IDEATEK_WINDOW_STATE_FIELDS` |
| `ideatek:Display` | `ideatek_display_t` | `ideatek_display_t_desc` | `IDEATEK_DISPLAY_FIELDS` |
| `ideatek:VideoMode` | `ideatek_video_mode_t` | `ideatek_video_mode_t_desc` | `IDEATEK_VIDEO_MODE_FIELDS` |

```text
{ IDEATEK_PLACE_SYSTEM = 0, IDEATEK_PLACE_CENTER = 1, IDEATEK_PLACE_AT = 2 }
{ IDEATEK_WINDOW_MODE_WINDOWED = 0, IDEATEK_WINDOW_MODE_MINIMIZED = 1, IDEATEK_WINDOW_MODE_MAXIMIZED = 2, IDEATEK_WINDOW_MODE_FULLSCREEN = 3, IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN = 4 }
{ IDEATEK_SURFACE_NONE = 0, IDEATEK_SURFACE_WINDOWS = 1, IDEATEK_SURFACE_METAL = 2, IDEATEK_SURFACE_XLIB = 3, IDEATEK_SURFACE_XCB = 4, IDEATEK_SURFACE_WAYLAND = 5, IDEATEK_SURFACE_ANDROID = 6, IDEATEK_SURFACE_WEB = 7 }
{ IDEATEK_FLASH_CANCEL = 0, IDEATEK_FLASH_BRIEFLY = 1, IDEATEK_FLASH_UNTIL_FOCUSED = 2 }
{ IDEATEK_ORIENTATION_UNKNOWN = 0, IDEATEK_ORIENTATION_LANDSCAPE = 1, IDEATEK_ORIENTATION_LANDSCAPE_FLIPPED = 2, IDEATEK_ORIENTATION_PORTRAIT = 3, IDEATEK_ORIENTATION_PORTRAIT_FLIPPED = 4 }
{ char title[128]; ideatek_entity_t display; ideatek_entity_t video_mode; int32_t x; int32_t y; uint32_t width; uint32_t height; uint32_t min_width; uint32_t min_height; uint32_t max_width; uint32_t max_height; uint32_t flashes; uint32_t raises; ideatek_place_t place; ideatek_window_mode_t mode; ideatek_flash_t flash; bool resizable; bool borderless; bool visible; bool always_on_top; }
{ uint64_t native_display; uint64_t native_window; ideatek_entity_t display; ideatek_entity_t video_mode; int32_t x; int32_t y; uint32_t width; uint32_t height; uint32_t pixel_width; uint32_t pixel_height; int32_t border_top; int32_t border_left; int32_t border_bottom; int32_t border_right; int32_t safe_x; int32_t safe_y; uint32_t safe_width; uint32_t safe_height; float display_scale; float sdr_white_level; float hdr_headroom; ideatek_window_mode_t mode; ideatek_surface_t surface; bool focused; bool mouse_inside; bool occluded; bool visible; bool resizable; bool borderless; bool always_on_top; bool hdr; bool close_asked; }
{ char name[128]; ideatek_entity_t desktop_mode; ideatek_entity_t current_mode; int32_t x; int32_t y; uint32_t width; uint32_t height; int32_t usable_x; int32_t usable_y; uint32_t usable_width; uint32_t usable_height; float content_scale; ideatek_orientation_t orientation; ideatek_orientation_t natural_orientation; bool hdr; }
{ ideatek_entity_t display; uint32_t width; uint32_t height; float pixel_density; int32_t refresh_numerator; int32_t refresh_denominator; uint32_t order; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_place_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_window_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_surface_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_flash_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_orientation_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Display` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VideoMode` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MainWindow` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PrimaryDisplay` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares each field type before the struct that has a field of the type.

## The grants

The field types are `ideatek:ideatek_entity_t` and the five enums.

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:WindowRequest` and `ideatek:MainWindow` | The field types |
| The window module | `ideatek:WindowState`, `ideatek:Display`, `ideatek:VideoMode` and `ideatek:PrimaryDisplay` | `ideatek:WindowRequest`, `ideatek:MainWindow` and the field types |
| A reader | No type. | The types that it reads, and the field types |

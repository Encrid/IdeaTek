# include/window/ideatek_window.h

`include/window/ideatek_window.h` gives the types of the windows and of the displays: their C types, their constants and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes `include/world/ideatek_world.h` for `ideatek_entity_t`, which `docs/extern/world/ideatek_world.h.md` describes.

## The windows and the displays

- Each window is an entity. A game has 1 window or more.
- `ideatek:MainWindow` marks the main window. At most 1 entity carries it.
- Each display is an entity, and each video mode of a display is an entity.
- `ideatek:PrimaryDisplay` marks the primary display of the system. Exactly 1 display entity carries it.
- Each region of a window is an entity, and each item that the user drops on a window is an entity.
- A place and a size are in units of the desktop. The origin is the top left corner of the primary display, and +Y points down.
- A size in pixels is a size in units of the desktop, times the density of the pixels.

## The roles

- The game writes `ideatek:WindowRequest`, `ideatek:MainWindow` and `ideatek:WindowRegion`.
- The window module reads `ideatek:WindowRequest`, `ideatek:MainWindow` and `ideatek:WindowRegion`. It writes `ideatek:WindowState`, `ideatek:Display`, `ideatek:VideoMode`, `ideatek:PrimaryDisplay` and `ideatek:Drop`. A deployment has one window module.
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

## The events of the system

The window module takes the events of the system from the queue of SDL, in the first phase of each round.
It runs after the clock module, and before each other reader of the events.
When the `wait` of `ideatek:Frame` is more than 0, it waits for an event (`SDL_WaitEventTimeout`).
It waits until `wait` passed since it took the events of the last round, or until an event comes.
`docs/extern/frame/ideatek_frame.h.md`, *The limit of the rate*, tells when `wait` is more than 0.

## The regions of a window

A window without border has no title bar of the system. The game can draw its own title bar, and mark its parts with regions.
A region is a rectangle of a window, with a role. The role tells what the system does when the user presses the mouse in the region.

- With `IDEATEK_REGION_DRAG`, the user moves the window.
- With a resize role, the user changes the size of the window from that edge or corner.
- With `IDEATEK_REGION_NORMAL`, the game gets the mouse as at each other point.

The system asks for the role of a point at any time, not only in a phase.
Thus, in each round, the window module copies the regions of each window, and gives the role of a point from that copy.
If 2 regions hold the point, the region with the larger `order` gives the role.
A point that no region holds is `IDEATEK_REGION_NORMAL`.
In a drag or a resize region, the system takes the mouse, and the game can get no event of the mouse button.
If the system cannot ask for the roles, `hit_test` of `ideatek:WindowState` is `false`, and the regions have no effect.

## The drops

The user can drag files or a text from another program, and drop them on a window.

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
| `close_on_request` | `true` | `true` or `false` | `true` closes a window on a close request. |
| `end_on` | `"main"` | `"main"`, `"all"` or `"never"` | `"main"` ends the run when the main window closes. `"all"` ends the run when the last window closes. `"never"` does not end the run. |
| `icon` | `"none"` | a path or `"none"` | The path of a PNG file, which is the icon of each window. `"none"` keeps the icon of the system. |
| `screen_saver` | `false` | `true` or `false` | `true` lets the screen saver of the system start while the game runs. |
| `drop_bytes` | 65536 | 2 to 1073741824, an even number | The bytes for the dropped items. Each round uses at most the half. |

If the window module cannot load `icon`, it writes 1 line in the log, and opens the windows with the icon of the system.
The icon is the same for each window, and does not change while the game runs.

## ideatek:WindowRequest

`ideatek:WindowRequest` is the window that the game asks for.
Its C type `ideatek_window_request_t` takes 216 bytes, with an alignment of 8.

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
| `menus` | `uint32_t` | 184 | A count. Each change of the count shows the menu of the system for the window, at `menu_x` and `menu_y`. On a system with no such menu, a change has no effect. |
| `menu_x` | `int32_t` | 188 | The left edge of the menu, from the left edge of the area of the window. |
| `menu_y` | `int32_t` | 192 | The top edge of the menu, from the top edge of the area of the window. |
| `opacity` | `float` | 196 | The opacity of the window, from 0 to 1. 1 is opaque. |
| `progress` | `float` | 200 | The part of a task that is done, from 0 to 1. The task bar of the system shows it on the button of the window. |
| `place` | `ideatek_place_t` | 204 | How the window module places the window. |
| `mode` | `ideatek_window_mode_t` | 205 | The mode of the window. |
| `flash` | `ideatek_flash_t` | 206 | How the window flashes. |
| `progress_state` | `ideatek_progress_t` | 207 | How the task bar shows `progress`. |
| `resizable` | `bool` | 208 | `true` lets the user change the size. |
| `borderless` | `bool` | 209 | `true` shows no border and no title bar. |
| `visible` | `bool` | 210 | `true` shows the window. |
| `always_on_top` | `bool` | 211 | `true` keeps the window above the other windows. |
| `transparent` | `bool` | 212 | `true` lets the alpha of the pixels that render draws show what is behind the window. |
| `focusable` | `bool` | 213 | `true` lets the window get the focus. |

The bytes 214 and 215 hold no field, because the size of the type is a multiple of 8.

- A title that is longer than 127 bytes is cut before its first UTF-8 character that does not fit. The window module writes 1 line in the log.
- The window module keeps the size between the smallest size and the largest size.
- If the video mode is dead, or is not a mode of `display`, the window module uses `IDEATEK_WINDOW_MODE_FULLSCREEN`. It writes 1 line in the log.
- If the system refuses the video mode, the window module also uses `IDEATEK_WINDOW_MODE_FULLSCREEN`.
- The window module reads `transparent` only when it opens the window, because the system sets it only then. A later change writes 1 line in the log.
- The window module refuses an `opacity` or a `progress` that is not from 0 to 1. It keeps the last value, and writes 1 line in the log.

### ideatek:ideatek_progress_t

`ideatek_progress_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The task bar |
| --- | --- | --- |
| `IDEATEK_PROGRESS_NONE` | 0 | Shows no progress. |
| `IDEATEK_PROGRESS_INDETERMINATE` | 1 | Shows a task that runs, with no amount. |
| `IDEATEK_PROGRESS_NORMAL` | 2 | Shows `progress`. |
| `IDEATEK_PROGRESS_PAUSED` | 3 | Shows `progress`, as a task that is paused. |
| `IDEATEK_PROGRESS_ERROR` | 4 | Shows `progress`, as a task that failed. |

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
Its C type `ideatek_window_state_t` takes 128 bytes, with an alignment of 8.

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
| `opacity` | `float` | 100 | The opacity of the window, from 0 to 1. |
| `drop_x` | `float` | 104 | While `drop_over` is `true`: the left edge of the point of the drag, from the left edge of the area. 0 until the system gives the first point. |
| `drop_y` | `float` | 108 | While `drop_over` is `true`: the top edge of the point of the drag, from the top edge of the area. 0 until the system gives the first point. |
| `mode` | `ideatek_window_mode_t` | 112 | The mode of the window. |
| `surface` | `ideatek_surface_t` | 113 | The kind of the handles. |
| `focused` | `bool` | 114 | `true` while the window gets the keys. |
| `mouse_inside` | `bool` | 115 | `true` while the mouse is in the window. |
| `occluded` | `bool` | 116 | `true` while other windows hide all the window. |
| `visible` | `bool` | 117 | `true` while the window is shown. |
| `resizable` | `bool` | 118 | `true` while the user can change the size. |
| `borderless` | `bool` | 119 | `true` while the window shows no border. |
| `always_on_top` | `bool` | 120 | `true` while the window stays above the other windows. |
| `hdr` | `bool` | 121 | `true` while the window can show HDR content. |
| `close_asked` | `bool` | 122 | `true` after a close request, when `close_on_request` is `false`. |
| `transparent` | `bool` | 123 | `true` if the window shows the alpha of its pixels. |
| `focusable` | `bool` | 124 | `true` while the window can get the focus. |
| `progress_shown` | `bool` | 125 | `true` if the task bar of the system shows the progress. |
| `hit_test` | `bool` | 126 | `true` if the system asks the window module for the roles of the regions. |
| `drop_over` | `bool` | 127 | `true` while the user drags files or a text over the window. |

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

## ideatek:WindowRegion

`ideatek:WindowRegion` is a region of a window: *The regions of a window*.
Its C type `ideatek_window_region_t` has an alignment of 8 and takes 32 bytes.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `window` | `ideatek_entity_t` | 0 | The window entity of the region. |
| `x` | `int32_t` | 8 | The left edge, from the left edge of the area of the window. |
| `y` | `int32_t` | 12 | The top edge, from the top edge of the area of the window. |
| `width` | `uint32_t` | 16 | The width. |
| `height` | `uint32_t` | 20 | The height. |
| `order` | `uint32_t` | 24 | Of 2 regions that hold a point, the region with the larger `order` gives the role. |
| `role` | `ideatek_region_t` | 28 | What the system does when the user presses the mouse in the region. |

The bytes 29 to 31 hold no field, because the size of the type is a multiple of 8.
A region of a dead window has no effect. If 2 regions that hold a point have the same `order`, the role is one of their roles.

### ideatek:ideatek_region_t

`ideatek_region_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The user presses the mouse in the region, and |
| --- | --- | --- |
| `IDEATEK_REGION_NORMAL` | 0 | the game gets the mouse. |
| `IDEATEK_REGION_DRAG` | 1 | moves the window. |
| `IDEATEK_REGION_RESIZE_TOP_LEFT` | 2 | changes the size from the top left corner. |
| `IDEATEK_REGION_RESIZE_TOP` | 3 | changes the size from the top edge. |
| `IDEATEK_REGION_RESIZE_TOP_RIGHT` | 4 | changes the size from the top right corner. |
| `IDEATEK_REGION_RESIZE_RIGHT` | 5 | changes the size from the right edge. |
| `IDEATEK_REGION_RESIZE_BOTTOM_RIGHT` | 6 | changes the size from the bottom right corner. |
| `IDEATEK_REGION_RESIZE_BOTTOM` | 7 | changes the size from the bottom edge. |
| `IDEATEK_REGION_RESIZE_BOTTOM_LEFT` | 8 | changes the size from the bottom left corner. |
| `IDEATEK_REGION_RESIZE_LEFT` | 9 | changes the size from the left edge. |

## ideatek:Drop

`ideatek:Drop` is an item that the user dropped on a window: *The drops*.
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
| `ideatek:ideatek_progress_t` | `ideatek_progress_t` | `ideatek_progress_t_desc` | `IDEATEK_PROGRESS_VALUES` |
| `ideatek:ideatek_region_t` | `ideatek_region_t` | `ideatek_region_t_desc` | `IDEATEK_REGION_VALUES` |
| `ideatek:ideatek_drop_kind_t` | `ideatek_drop_kind_t` | `ideatek_drop_kind_t_desc` | `IDEATEK_DROP_KIND_VALUES` |
| `ideatek:WindowRequest` | `ideatek_window_request_t` | `ideatek_window_request_t_desc` | `IDEATEK_WINDOW_REQUEST_FIELDS` |
| `ideatek:WindowState` | `ideatek_window_state_t` | `ideatek_window_state_t_desc` | `IDEATEK_WINDOW_STATE_FIELDS` |
| `ideatek:Display` | `ideatek_display_t` | `ideatek_display_t_desc` | `IDEATEK_DISPLAY_FIELDS` |
| `ideatek:VideoMode` | `ideatek_video_mode_t` | `ideatek_video_mode_t_desc` | `IDEATEK_VIDEO_MODE_FIELDS` |
| `ideatek:WindowRegion` | `ideatek_window_region_t` | `ideatek_window_region_t_desc` | `IDEATEK_WINDOW_REGION_FIELDS` |
| `ideatek:Drop` | `ideatek_drop_t` | `ideatek_drop_t_desc` | `IDEATEK_DROP_FIELDS` |

```text
{ IDEATEK_PLACE_SYSTEM = 0, IDEATEK_PLACE_CENTER = 1, IDEATEK_PLACE_AT = 2 }
{ IDEATEK_WINDOW_MODE_WINDOWED = 0, IDEATEK_WINDOW_MODE_MINIMIZED = 1, IDEATEK_WINDOW_MODE_MAXIMIZED = 2, IDEATEK_WINDOW_MODE_FULLSCREEN = 3, IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN = 4 }
{ IDEATEK_SURFACE_NONE = 0, IDEATEK_SURFACE_WINDOWS = 1, IDEATEK_SURFACE_METAL = 2, IDEATEK_SURFACE_XLIB = 3, IDEATEK_SURFACE_XCB = 4, IDEATEK_SURFACE_WAYLAND = 5, IDEATEK_SURFACE_ANDROID = 6, IDEATEK_SURFACE_WEB = 7 }
{ IDEATEK_FLASH_CANCEL = 0, IDEATEK_FLASH_BRIEFLY = 1, IDEATEK_FLASH_UNTIL_FOCUSED = 2 }
{ IDEATEK_ORIENTATION_UNKNOWN = 0, IDEATEK_ORIENTATION_LANDSCAPE = 1, IDEATEK_ORIENTATION_LANDSCAPE_FLIPPED = 2, IDEATEK_ORIENTATION_PORTRAIT = 3, IDEATEK_ORIENTATION_PORTRAIT_FLIPPED = 4 }
{ IDEATEK_PROGRESS_NONE = 0, IDEATEK_PROGRESS_INDETERMINATE = 1, IDEATEK_PROGRESS_NORMAL = 2, IDEATEK_PROGRESS_PAUSED = 3, IDEATEK_PROGRESS_ERROR = 4 }
{ IDEATEK_REGION_NORMAL = 0, IDEATEK_REGION_DRAG = 1, IDEATEK_REGION_RESIZE_TOP_LEFT = 2, IDEATEK_REGION_RESIZE_TOP = 3, IDEATEK_REGION_RESIZE_TOP_RIGHT = 4, IDEATEK_REGION_RESIZE_RIGHT = 5, IDEATEK_REGION_RESIZE_BOTTOM_RIGHT = 6, IDEATEK_REGION_RESIZE_BOTTOM = 7, IDEATEK_REGION_RESIZE_BOTTOM_LEFT = 8, IDEATEK_REGION_RESIZE_LEFT = 9 }
{ IDEATEK_DROP_FILE = 0, IDEATEK_DROP_TEXT = 1 }
{ char title[128]; ideatek_entity_t display; ideatek_entity_t video_mode; int32_t x; int32_t y; uint32_t width; uint32_t height; uint32_t min_width; uint32_t min_height; uint32_t max_width; uint32_t max_height; uint32_t flashes; uint32_t raises; uint32_t menus; int32_t menu_x; int32_t menu_y; float opacity; float progress; ideatek_place_t place; ideatek_window_mode_t mode; ideatek_flash_t flash; ideatek_progress_t progress_state; bool resizable; bool borderless; bool visible; bool always_on_top; bool transparent; bool focusable; }
{ uint64_t native_display; uint64_t native_window; ideatek_entity_t display; ideatek_entity_t video_mode; int32_t x; int32_t y; uint32_t width; uint32_t height; uint32_t pixel_width; uint32_t pixel_height; int32_t border_top; int32_t border_left; int32_t border_bottom; int32_t border_right; int32_t safe_x; int32_t safe_y; uint32_t safe_width; uint32_t safe_height; float display_scale; float sdr_white_level; float hdr_headroom; float opacity; float drop_x; float drop_y; ideatek_window_mode_t mode; ideatek_surface_t surface; bool focused; bool mouse_inside; bool occluded; bool visible; bool resizable; bool borderless; bool always_on_top; bool hdr; bool close_asked; bool transparent; bool focusable; bool progress_shown; bool hit_test; bool drop_over; }
{ char name[128]; ideatek_entity_t desktop_mode; ideatek_entity_t current_mode; int32_t x; int32_t y; uint32_t width; uint32_t height; int32_t usable_x; int32_t usable_y; uint32_t usable_width; uint32_t usable_height; float content_scale; ideatek_orientation_t orientation; ideatek_orientation_t natural_orientation; bool hdr; }
{ ideatek_entity_t display; uint32_t width; uint32_t height; float pixel_density; int32_t refresh_numerator; int32_t refresh_denominator; uint32_t order; }
{ ideatek_entity_t window; int32_t x; int32_t y; uint32_t width; uint32_t height; uint32_t order; ideatek_region_t role; }
{ ideatek_entity_t window; bridge_text_t data; bridge_text_t source; uint32_t set; uint32_t index; float x; float y; ideatek_drop_kind_t kind; }
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
| `ideatek:ideatek_progress_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_region_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_drop_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Display` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VideoMode` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowRegion` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Drop` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MainWindow` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PrimaryDisplay` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares each field type before the struct that has a field of the type.
`bridge_text_t` is a type of Bridge, and no module declares it.

## The grants

The field types are `ideatek:ideatek_entity_t` and the eight enums.

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:WindowRequest`, `ideatek:MainWindow` and `ideatek:WindowRegion` | The field types |
| The window module | `ideatek:WindowState`, `ideatek:Display`, `ideatek:VideoMode`, `ideatek:PrimaryDisplay` and `ideatek:Drop` | `ideatek:WindowRequest`, `ideatek:MainWindow`, `ideatek:WindowRegion` and the field types |
| A reader | No type. | The types that it reads, and the field types |

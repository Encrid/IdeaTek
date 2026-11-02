# include/window/ideatek_window.h

`include/window/ideatek_window.h` gives the types of the windows of the game: their C types, their constants and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.

## The windows

- Each window is an entity. A game has 1 window or more.
- `ideatek:MainWindow` marks the main window. At most 1 entity carries it.
- A size or a place in units of the screen does not change with the scale of the screen. A size in pixels does.
- The scale of a screen is the number of pixels in 1 unit of the screen.

## The roles

- The game writes `ideatek:WindowRequest` and `ideatek:MainWindow`.
- The window module reads `ideatek:WindowRequest`, and writes `ideatek:WindowState`. A deployment has one window module.
- A reader reads `ideatek:WindowState`. Render and input are readers.

The game spawns an entity, and attaches `ideatek:WindowRequest` to it.
The window module opens a window for each entity that carries `ideatek:WindowRequest`, and attaches `ideatek:WindowState` to it, 1 barrier later.
When the game changes `ideatek:WindowRequest`, the window module changes the window.
If the system refuses a change, `ideatek:WindowState` keeps what the window is.
When the user closes the window, the window module sets `close_asked`, and the window stays open.
The game closes a window when it destroys its entity.

## ideatek:WindowRequest

`ideatek:WindowRequest` is the window that the game asks for.
Its C type `ideatek_window_request_t` has 152 bytes, and an alignment of 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `title` | `char[128]` | 0 | The title, in UTF-8, with a NUL after its last byte. |
| `width` | `uint32_t` | 128 | The width, in units of the screen. |
| `height` | `uint32_t` | 132 | The height, in units of the screen. |
| `x` | `int32_t` | 136 | With `IDEATEK_PLACE_AT`: the left edge, in units of the screen, from the left edge of `screen`. |
| `y` | `int32_t` | 140 | With `IDEATEK_PLACE_AT`: the top edge, in units of the screen, from the top edge of `screen`. |
| `screen` | `uint32_t` | 144 | The number of the screen. The primary screen is 0. |
| `place` | `ideatek_place_t` | 148 | How the window module places the window. |
| `mode` | `ideatek_window_mode_t` | 149 | The mode of the window. |
| `resizable` | `bool` | 150 | `true` lets the user change the size. |
| `borderless` | `bool` | 151 | `true` shows no border and no title bar. |

A title that is longer than 127 bytes is cut before its first UTF-8 character that does not fit.

### ideatek:ideatek_place_t

`ideatek_place_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The place |
| --- | --- | --- |
| `IDEATEK_PLACE_SYSTEM` | 0 | The system chooses the place. |
| `IDEATEK_PLACE_CENTER` | 1 | The center of `screen`. |
| `IDEATEK_PLACE_AT` | 2 | `x` and `y`. |

### ideatek:ideatek_window_mode_t

`ideatek_window_mode_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.

| Constant | Value | The mode |
| --- | --- | --- |
| `IDEATEK_WINDOW_MODE_WINDOWED` | 0 | A window with the size of `width` and `height`. |
| `IDEATEK_WINDOW_MODE_MINIMIZED` | 1 | The window is hidden in the task bar of the system. |
| `IDEATEK_WINDOW_MODE_MAXIMIZED` | 2 | The window fills the screen, except the bars of the system. |
| `IDEATEK_WINDOW_MODE_FULLSCREEN` | 3 | The window fills all the screen, with no border. The system keeps the mode of the screen. |
| `IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN` | 4 | The window fills all the screen, and owns the screen while it has the focus. |

## ideatek:WindowState

`ideatek:WindowState` is the window that the system gives.
Its C type `ideatek_window_state_t` has 48 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `display` | `uint64_t` | 0 | The first handle of the window: see `ideatek_surface_t`. |
| `window` | `uint64_t` | 8 | The second handle of the window: see `ideatek_surface_t`. |
| `width` | `uint32_t` | 16 | The width of the area that render draws into, in pixels. |
| `height` | `uint32_t` | 20 | The height of the area that render draws into, in pixels. |
| `scale` | `float` | 24 | The scale of the screen that shows the window. |
| `x` | `int32_t` | 28 | The left edge, in units of the screen, from the left edge of `screen`. |
| `y` | `int32_t` | 32 | The top edge, in units of the screen, from the top edge of `screen`. |
| `screen` | `uint32_t` | 36 | The number of the screen that shows the window. |
| `mode` | `ideatek_window_mode_t` | 40 | The mode of the window. |
| `focused` | `bool` | 41 | `true` while the window gets the keys. |
| `close_asked` | `bool` | 42 | `true` after the user asked to close the window. |
| `surface` | `ideatek_surface_t` | 43 | The kind of the handles. |

The bytes 44 to 47 hold no field, because the size of the type is a multiple of 8.

### ideatek:ideatek_surface_t

`ideatek_surface_t` is an enum of `uint8_t`: 1 byte, with an alignment of 1.
A handle that the table does not name is 0.

| Constant | Value | `display` | `window` |
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

## ideatek:MainWindow

`ideatek:MainWindow` is a tag: it has no bytes, no C type and no descriptor.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_place_t` | `ideatek_place_t` | `ideatek_place_t_desc` | `IDEATEK_PLACE_VALUES` |
| `ideatek:ideatek_window_mode_t` | `ideatek_window_mode_t` | `ideatek_window_mode_t_desc` | `IDEATEK_WINDOW_MODE_VALUES` |
| `ideatek:ideatek_surface_t` | `ideatek_surface_t` | `ideatek_surface_t_desc` | `IDEATEK_SURFACE_VALUES` |
| `ideatek:WindowRequest` | `ideatek_window_request_t` | `ideatek_window_request_t_desc` | `IDEATEK_WINDOW_REQUEST_FIELDS` |
| `ideatek:WindowState` | `ideatek_window_state_t` | `ideatek_window_state_t_desc` | `IDEATEK_WINDOW_STATE_FIELDS` |

```text
uint8_t { IDEATEK_PLACE_SYSTEM = 0, IDEATEK_PLACE_CENTER = 1, IDEATEK_PLACE_AT = 2 }
uint8_t { IDEATEK_WINDOW_MODE_WINDOWED = 0, IDEATEK_WINDOW_MODE_MINIMIZED = 1, IDEATEK_WINDOW_MODE_MAXIMIZED = 2, IDEATEK_WINDOW_MODE_FULLSCREEN = 3, IDEATEK_WINDOW_MODE_EXCLUSIVE_FULLSCREEN = 4 }
uint8_t { IDEATEK_SURFACE_NONE = 0, IDEATEK_SURFACE_WINDOWS = 1, IDEATEK_SURFACE_METAL = 2, IDEATEK_SURFACE_XLIB = 3, IDEATEK_SURFACE_XCB = 4, IDEATEK_SURFACE_WAYLAND = 5, IDEATEK_SURFACE_ANDROID = 6, IDEATEK_SURFACE_WEB = 7 }
{ char title[128]; uint32_t width; uint32_t height; int32_t x; int32_t y; uint32_t screen; ideatek_place_t place; ideatek_window_mode_t mode; bool resizable; bool borderless; }
{ uint64_t display; uint64_t window; uint32_t width; uint32_t height; float scale; int32_t x; int32_t y; uint32_t screen; ideatek_window_mode_t mode; bool focused; bool close_asked; ideatek_surface_t surface; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_place_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_window_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_surface_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WindowState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MainWindow` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares each enum before the struct that has a field of the enum.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:WindowRequest` and `ideatek:MainWindow` | `ideatek:ideatek_place_t` and `ideatek:ideatek_window_mode_t` |
| The window module | `ideatek:WindowState` | `ideatek:WindowRequest`, `ideatek:MainWindow` and the three enums |
| A reader | No type. | `ideatek:WindowState`, `ideatek:ideatek_window_mode_t` and `ideatek:ideatek_surface_t` |

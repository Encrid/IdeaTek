# src/platform_abstraction/window_sdl3/window_driver.c

This file finds the video driver of SDL3.

## The driver

`find_driver` compares the name that `SDL_GetCurrentVideoDriver` gives with the rows of this table, and gives the first row that matches.
`bridge_init` keeps the row in `driver`, after the start of the video. Thus `SDL_GetCurrentVideoDriver` always gives a name.

| `name` | `surface` | `flags` | `paste` | Key of `native_display` | Key of `native_window` |
| --- | --- | --- | --- | --- | --- |
| `"windows"` | `IDEATEK_SURFACE_WINDOWS` | 0 | `SDL_KMOD_CTRL` | `SDL_PROP_WINDOW_WIN32_INSTANCE_POINTER` | `SDL_PROP_WINDOW_WIN32_HWND_POINTER` |
| `"cocoa"` | `IDEATEK_SURFACE_METAL` | `SDL_WINDOW_METAL` | `SDL_KMOD_GUI` | None. | None: `docs/intern/platform_abstraction/window_sdl3/window_handles.c.md`, *The handles* |
| `"x11"` | `IDEATEK_SURFACE_XLIB` | 0 | `SDL_KMOD_CTRL` | `SDL_PROP_WINDOW_X11_DISPLAY_POINTER` | `SDL_PROP_WINDOW_X11_WINDOW_NUMBER` |
| `"wayland"` | `IDEATEK_SURFACE_WAYLAND` | 0 | `SDL_KMOD_CTRL` | `SDL_PROP_WINDOW_WAYLAND_DISPLAY_POINTER` | `SDL_PROP_WINDOW_WAYLAND_SURFACE_POINTER` |
| Each other name | `IDEATEK_SURFACE_NONE` | 0 | `SDL_KMOD_CTRL` | None. | None. |

The last row matches each name. It serves a driver with no window of the system, for example `"offscreen"` or `"dummy"`.
SDL3 gives no connection of XCB, so the module states `IDEATEK_SURFACE_XLIB` on X11, and never `IDEATEK_SURFACE_XCB`.
The targets of the module have no Android and no web, so it never states `IDEATEK_SURFACE_ANDROID` or `IDEATEK_SURFACE_WEB`.

## The branches

| Case | Result |
| --- | --- |
| Each row of *The driver*. | `surface`, `native_display` and `native_window` of that row. |
| A driver that the table does not name. | `IDEATEK_SURFACE_NONE` and 2 handles of 0. |

## Functions

| Function | Result |
| --- | --- |
| `find_driver` | Gives the row of the video driver of SDL3. |

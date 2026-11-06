# src/platform_abstraction/input_sdl3/input_cursor.c

This file applies `ideatek:MouseRequest`: the mode of the mouse and the cursor: `docs/extern/platform_abstraction/ideatek_input_mouse.h.md`, *ideatek:MouseRequest*.

## The request

The observer `mouse` hears `BRIDGE_ON_ENTER` and `BRIDGE_ON_ATTACH` of `ideatek:MouseRequest`.
Only a trigger of the input entity counts. The module reads its request with `bridge_native_find_value`.
The window is the first window of `SDL_GetWindows`. With no window, a request changes nothing, and the next attach applies it.

## The mode

`apply_mode` calls SDL3 for each mode:

| Mode | Relative mode | Grab | Cursor |
| --- | --- | --- | --- |
| `IDEATEK_MOUSE_MODE_NORMAL` | `false` | `false` | Shows |
| `IDEATEK_MOUSE_MODE_HIDDEN` | `false` | `false` | Hides |
| `IDEATEK_MOUSE_MODE_RELATIVE` | `true` | `false` | Hides, by the relative mode |
| `IDEATEK_MOUSE_MODE_CONFINED` | `false` | `true` | Shows |

The relative mode comes from `SDL_SetWindowRelativeMouseMode`, the grab from `SDL_SetWindowMouseGrab`, and the cursor from `SDL_ShowCursor` and `SDL_HideCursor`.
If SDL3 refuses a call, `apply_mode` writes `no mouse mode <mode>: <reason>`, and the mouse keeps the mode that SDL3 has.
In the relative mode, SDL3 gives only motions, and `x` and `y` of `ideatek:Mouse` stay: https://wiki.libsdl.org/SDL3/SDL_SetWindowRelativeMouseMode.

## The cursor

`apply_cursor` sets the cursor of the request with `SDL_SetCursor`:

| `ideatek_cursor_t` | Cursor of SDL3 |
| --- | --- |
| `IDEATEK_CURSOR_DEFAULT` | `SDL_GetDefaultCursor` |
| `IDEATEK_CURSOR_ARROW` | `SDL_SYSTEM_CURSOR_DEFAULT` |
| `IDEATEK_CURSOR_IBEAM` | `SDL_SYSTEM_CURSOR_TEXT` |
| `IDEATEK_CURSOR_CROSSHAIR` | `SDL_SYSTEM_CURSOR_CROSSHAIR` |
| `IDEATEK_CURSOR_POINTING_HAND` | `SDL_SYSTEM_CURSOR_POINTER` |
| `IDEATEK_CURSOR_RESIZE_EW` | `SDL_SYSTEM_CURSOR_EW_RESIZE` |
| `IDEATEK_CURSOR_RESIZE_NS` | `SDL_SYSTEM_CURSOR_NS_RESIZE` |
| `IDEATEK_CURSOR_RESIZE_NWSE` | `SDL_SYSTEM_CURSOR_NWSE_RESIZE` |
| `IDEATEK_CURSOR_RESIZE_NESW` | `SDL_SYSTEM_CURSOR_NESW_RESIZE` |
| `IDEATEK_CURSOR_RESIZE_ALL` | `SDL_SYSTEM_CURSOR_MOVE` |
| `IDEATEK_CURSOR_NOT_ALLOWED` | `SDL_SYSTEM_CURSOR_NOT_ALLOWED` |

The module makes each cursor of the system 1 time, with `SDL_CreateSystemCursor`, and keeps it in `cursors` until its end.
If SDL3 makes no cursor, `apply_cursor` writes `no cursor <cursor>: <reason>`, and the cursor of the system stays.
A value of `cursor` outside the enum takes `IDEATEK_CURSOR_DEFAULT`, and a value of `mode` outside the enum takes `IDEATEK_MOUSE_MODE_NORMAL`. A request comes from the game, so the module checks it.

## The branches

| Case | Result |
| --- | --- |
| Each mode of the table. | The calls of its row. |
| A mode that SDL3 refuses. | The line of the mode. |
| A mode or a cursor outside its enum. | The default. |
| A cursor of the system, the first time. | SDL3 makes it, and it shows. |
| The same cursor again. | The module uses the cursor that it made. |
| A cursor that SDL3 does not make. | The line of the cursor. |
| `IDEATEK_CURSOR_DEFAULT`. | The default cursor of SDL3. |
| A request on another entity, or with no window. | Nothing. |
| SDL3 refuses the grab, or the hide of the cursor. | The line of the mode. |
| SDL3 refuses to set a cursor that it made. | The line of the cursor. |

## Functions

| Function | Result |
| --- | --- |
| `first_window` | Gives the first window of SDL3, or null. |
| `apply_mode` | Sets the mode of the mouse, or writes the line of the mode. |
| `system_cursor` | Gives the cursor of SDL3 of a value of the enum, and makes it the first time. |
| `apply_cursor` | Sets the cursor, or writes the line of the cursor. |
| `bridge_mouse` | Applies the request of each trigger of the input entity. |

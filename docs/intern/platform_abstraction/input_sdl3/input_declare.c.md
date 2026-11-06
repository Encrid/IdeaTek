# src/platform_abstraction/input_sdl3/input_declare.c

This file declares the 26 types of the input module, in the order of their slots: `docs/intern/platform_abstraction/input_sdl3/input_inside.h.md`, *The slots*.

## The declarations

Each type has the declaration of its header: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The declarations*, and the same section of each input header.
The field types come first, because a struct can only name a type that is declared.
`ideatek:WindowState` and its 2 enums have the declaration of `docs/extern/platform_abstraction/ideatek_window.h.md`, *The declarations*.
A type that the window module declared first counts once: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge.h.md#bridge_load.
`ideatek_input_sdl3:Settings` is a `BRIDGE_SINGLETON`, with `BRIDGE_STRIP`.

| Function | Slots | Types |
| --- | --- | --- |
| `declare_fields` | 0 to 8 | `ideatek:ideatek_entity_t`, `ideatek:ideatek_input_event_kind_t`, `ideatek:ideatek_modifier_t`, `ideatek:ideatek_mouse_mode_t`, `ideatek:ideatek_cursor_t`, `ideatek:ideatek_gamepad_button_t`, `ideatek:ideatek_finger_event_kind_t`, `ideatek:ideatek_window_mode_t` and `ideatek:ideatek_surface_t` |
| `declare_keys` | 9 to 13 | `ideatek:InputEvent`, `ideatek:Keyboard`, `ideatek:KeyEvent`, `ideatek:TextRequest` and `ideatek:TextEvent` |
| `declare_mouse` | 14 to 18 | `ideatek:Mouse`, `ideatek:MouseMotion`, `ideatek:MouseButton`, `ideatek:MouseWheel` and `ideatek:MouseRequest` |
| `declare_pads` | 19 to 25 | `ideatek:Gamepad`, `ideatek:GamepadState`, `ideatek:GamepadEvent`, `ideatek:GamepadRequest`, `ideatek:FingerEvent`, `ideatek:WindowState` and the settings |

## Functions

| Function | Result |
| --- | --- |
| `declare_fields` | Declares the 9 field types of the slots 0 to 8. |
| `declare_keys` | Declares the types of the focus, of the keyboard and of the text input. |
| `declare_mouse` | Declares the types of the mouse. |
| `declare_pads` | Declares the types of the gamepads and of the fingers, `ideatek:WindowState` and the settings. |
| `declare_types` | Declares the 26 types, in the order of their slots. |

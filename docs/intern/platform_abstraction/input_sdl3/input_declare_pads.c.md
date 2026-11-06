# src/platform_abstraction/input_sdl3/input_declare_pads.c

This file declares the types of the slots 19 to 25, after the types of `docs/intern/platform_abstraction/input_sdl3/input_declare.c.md`.

| Slots | Types |
| --- | --- |
| 19 to 23 | `ideatek:Gamepad`, `ideatek:GamepadState`, `ideatek:GamepadEvent`, `ideatek:GamepadRequest` and `ideatek:FingerEvent` |
| 24 | `ideatek:WindowState`, which the observer `window` reads |
| 25 | `ideatek_input_sdl3:Settings`, a `BRIDGE_SINGLETON` |

## Functions

| Function | Result |
| --- | --- |
| `declare_pads` | Declares the types of the gamepads and of the fingers, `ideatek:WindowState` and the settings. |

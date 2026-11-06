# src/platform_abstraction/input_sdl3/input_inside.h

Each source of the part `input_sdl3` includes this header.
It includes SDL3, BB-Native, the 6 headers of `include/platform_abstraction/ideatek_input*.h`, `include/platform_abstraction/ideatek_window.h`, and the header of the part `sdl3_start`.
It holds the slots, the limits, the types, and the functions that the sources share.

## The module

The part fills the role of the input module: `docs/extern/platform_abstraction/input_sdl3/readme.md`.
Its identity is `ideatek_input_sdl3:input@0.1.0`, and its package is `BM-IdeaTek-Input-SDL3`.
It is pinned, because SDL3 runs its loop of events on the main thread of macOS.
Bridge cannot reload it: the gamepads and the cursors of SDL3 live outside the storage of Bridge.

## The slots

Bridge numbers the slots from 0, as the module declares its types: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge_abi.h.md#bridge_entries_t.
`declare_types` follows this table: `docs/intern/platform_abstraction/input_sdl3/input_declare.c.md`. Each slot is a `constexpr`.

| Slot | Name | Type |
| --- | --- | --- |
| 0 to 8 | No name | The field types: `ideatek:ideatek_entity_t`, the 6 enums and bitmasks of the input headers, and `ideatek:ideatek_window_mode_t` and `ideatek:ideatek_surface_t` of `ideatek:WindowState`. |
| 9 | `INPUT_EVENT` | `ideatek:InputEvent` |
| 10 | `KEYBOARD` | `ideatek:Keyboard` |
| 11 | `KEY_EVENT` | `ideatek:KeyEvent` |
| 12 | `TEXT_REQUEST` | `ideatek:TextRequest` |
| 13 | `TEXT_EVENT` | `ideatek:TextEvent` |
| 14 | `MOUSE` | `ideatek:Mouse` |
| 15 | `MOUSE_MOTION` | `ideatek:MouseMotion` |
| 16 | `MOUSE_BUTTON` | `ideatek:MouseButton` |
| 17 | `MOUSE_WHEEL` | `ideatek:MouseWheel` |
| 18 | `MOUSE_REQUEST` | `ideatek:MouseRequest` |
| 19 | `GAMEPAD` | `ideatek:Gamepad` |
| 20 | `GAMEPAD_STATE` | `ideatek:GamepadState` |
| 21 | `GAMEPAD_EVENT` | `ideatek:GamepadEvent` |
| 22 | `GAMEPAD_REQUEST` | `ideatek:GamepadRequest` |
| 23 | `FINGER_EVENT` | `ideatek:FingerEvent` |
| 24 | `WINDOW_STATE` | `ideatek:WindowState` |
| 25 | `SETTINGS` | `ideatek_input_sdl3:Settings` |

## The limits

| Constant | Value | Limit |
| --- | --- | --- |
| `INPUT_GAMEPADS` | 16 | The most gamepads that get an entity. |
| `INPUT_EVENTS` | 65535 | The most entities of 1 round: the top of the range of `round_entities`. |
| `INPUT_FINGERS` | 16 | The most fingers that the module keeps down at a time, for the release at a loss of the focus. |
| `INPUT_REASON` | 256 | The bytes of the text of a failure of SDL3. |

`INPUT_EVENTS` sizes the list of the entities of the events of a round, so the module destroys them in the next round.

## The settings

`ideatek_input_sdl3:Settings` is a singleton of `input_settings_t`:

| Field | C type | Setting |
| --- | --- | --- |
| `mappings` | `bridge_text_t` | `mappings`: a path, or `"none"`. |
| `round_entities` | `uint32_t` | `round_entities`. |
| `merge_motion` | `bool` | `merge_motion`. |
| `touch_mouse_events` | `bool` | `touch_mouse_events`. |

`start_input` reads them: `docs/intern/platform_abstraction/input_sdl3/input_start.c.md`.

## The state of the module

The module keeps its state between 2 calls in 1 variable, `input_module`, of the type `input_module_t`.
No argument of a call of Bridge carries that state.
`.toolchain/project.toml` states the exception of the rule G4 for it.

| Field | Content |
| --- | --- |
| `entity` | The input entity, with `ideatek:Keyboard` and `ideatek:Mouse`. |
| `keyboard`, `mouse` | The last values of `ideatek:Keyboard` and `ideatek:Mouse` that the module attached. |
| `next_keyboard`, `next_mouse` | The values of the round. The round attaches them when their bytes differ from the last values. |
| `pads` | The gamepads: their id of SDL3, their `SDL_Gamepad`, their entity, and their last and next states. |
| `fingers` | The fingers that are down: their id and their last place. |
| `cursors` | The cursors of SDL3 that the module made, 1 for each `ideatek_cursor_t`. |
| `items`, `item_count` | The entities of the events of the round. |
| `order`, `spawned` | The next rank, and the count of the entities of the round. |
| `run` | The run of motions that `merge_motion` joins: its slot, its value, its key, its entity, and `true` while it is pending. |
| `window_list`, `text_list`, `mouse_list`, `pad_list` | The indexes of the lists of the 4 observers. |
| `focused`, `was_focused` | The focus that the observer `window` read, and the focus of the last round. |
| `started`, `text_on` | `true` after the first round, and `true` while the text input runs. |

## Functions

`input_inside.h` declares the functions that 1 source gives to the others.
Each document of a source gives its functions.

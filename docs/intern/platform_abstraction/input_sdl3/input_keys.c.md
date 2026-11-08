# src/platform_abstraction/input_sdl3/input_keys.c

This file turns each key event of SDL3 into an `ideatek:KeyEvent`, and keeps the keys that are down in `ideatek:Keyboard`: `docs/extern/platform_abstraction/ideatek_input_keyboard.h.md`.

## A key

`take_key` does these steps for `SDL_EVENT_KEY_DOWN` and `SDL_EVENT_KEY_UP`:

1. `usage_of_scancode` gives `code`. A key with a `code` of 0 has no event: `docs/intern/platform_abstraction/input_sdl3/input_usage.c.md`.
2. It fills the event: `code`, `key` from `usage_of_keycode`, `character` from `character_of`, `modifiers` from `modifiers_of`, `down`, `repeat`, and `order` from `next_order`.
3. It spawns the event with `spawn_event`: `docs/intern/platform_abstraction/input_sdl3/input_items.c.md`.
4. For a key of the page 0x07, it sets or clears the bit of the id in `keys` of `next_keyboard`.
5. It sets `modifiers` of `next_keyboard` to the modifiers of the event.

A key event needs 1 spawn and 1 attach of `ideatek:KeyEvent`, whose bytes `CHANGE_BYTES` gives.
A repeat of a key that is down keeps its bit at 1.

## The modifiers

`modifiers_of` gives `ideatek_modifier_t` from `SDL_Keymod`:

| `SDL_Keymod` | `ideatek_modifier_t` |
| --- | --- |
| `SDL_KMOD_LCTRL` | `IDEATEK_MODIFIER_LCTRL` |
| `SDL_KMOD_LSHIFT` | `IDEATEK_MODIFIER_LSHIFT` |
| `SDL_KMOD_LALT` | `IDEATEK_MODIFIER_LALT` |
| `SDL_KMOD_LGUI` | `IDEATEK_MODIFIER_LGUI` |
| `SDL_KMOD_RCTRL` | `IDEATEK_MODIFIER_RCTRL` |
| `SDL_KMOD_RSHIFT` | `IDEATEK_MODIFIER_RSHIFT` |
| `SDL_KMOD_RALT` | `IDEATEK_MODIFIER_RALT` |
| `SDL_KMOD_RGUI` | `IDEATEK_MODIFIER_RGUI` |
| `SDL_KMOD_NUM` | `IDEATEK_MODIFIER_NUM` |
| `SDL_KMOD_CAPS` | `IDEATEK_MODIFIER_CAPS` |
| `SDL_KMOD_SCROLL` | `IDEATEK_MODIFIER_SCROLL` |

`SDL_KMOD_LEVEL5` and `SDL_KMOD_MODE` have no bit, because the contract has none: the character carries the level of the layout.

## The release

`release_keys` spawns a release event for each key whose bit in `keys` of `next_keyboard` is 1, then clears the bit.
The event has the `code` and the `key` of the usage of the bit, a `character` of 0, and the modifiers of `next_keyboard`.
A key of the pages 0x0C and 0x01 has no bit, so it has no release.

## The branches

| Case | Result |
| --- | --- |
| A key of the page 0x07 goes down. | 1 event with `down` at `true`, and its bit at 1. |
| The key goes up. | 1 event with `down` at `false`, and its bit at 0. |
| A repeat of a key that is down. | 1 event with `repeat` at `true`. The bit stays at 1. |
| A key of the page 0x0C. | 1 event, and no bit. |
| A key with no usage. | No event. |
| A key with Shift. | `character` of the shifted character, and `modifiers` with `IDEATEK_MODIFIER_LSHIFT`. |
| A loss of the focus with 2 keys down. | 2 events with `down` at `false`, and no bit at 1. |

## Functions

| Function | Result |
| --- | --- |
| `modifiers_of` | Gives the modifiers of the contract from the modifiers of SDL3. |
| `set_bit` | Sets or clears the bit of an id of the page 0x07 in `keys`. |
| `take_key` | Spawns the event of a key, and keeps its bit. |
| `release_keys` | Spawns the release of each key that is down. |

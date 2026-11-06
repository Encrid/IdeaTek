# src/platform_abstraction/window_sdl3/window_clipboard.c

This file holds the observer `clipboard`, which sets the text of the clipboard, and the pastes of the user.
The rules of the clipboard are in `docs/extern/platform_abstraction/ideatek_window_clipboard.h.md`, *The clipboard*.

## The request

`bridge_clipboard` reads the list of its triggers, and calls `set_clipboard` for each entity, in the order of the list.
Thus the last request of a barrier is the text that stays on the clipboard. The observer gives 0.

`set_clipboard` does these steps:

1. `get_cell` gives where the request of the entity is. With no request, it stops.
2. `copy_arena_text` copies the text into a C string: `docs/intern/platform_abstraction/window_sdl3/window_text.c.md`.
3. If the copy fails, it stops. `copy_arena_text` wrote the line.
4. If SDL3 refuses `SDL_SetClipboardText`, it writes the line of no clipboard.
5. It frees the C string.

The text of the request is in the share of the game, and the game can write a wrong `at` or `len`.
Thus step 2 checks that the text is in the persistent arena before it reads a byte.

## A paste

`count_pastes` reads the key presses of the round with `SDL_PeepEvents` and `SDL_PEEKEVENT`.
A peek takes no event, so the input module still gets the key presses.
It reads at most `WINDOW_PEEKS` key presses. With more presses in 1 round, it does not see a paste after the first 64.

`is_paste` tells if a key press is a paste:

```pseudo
FUNCTION is_paste(key : SDL_KeyboardEvent) RETURNS BOOLEAN
   DECLARE modifier : U16
   modifier <- window_module.driver.paste
   RETURN key.key = SDLK_V AND has_modifier(key.mod, modifier) AND NOT has_modifier(key.mod, SDL_KMOD_ALT)
ENDFUNCTION
```

`has_modifier` is the test of the bits of the modifier in `mod`. It is an operator of C, and no function of the file.
The modifier is `SDL_KMOD_GUI` on macOS and `SDL_KMOD_CTRL` on each other system: `docs/intern/platform_abstraction/window_sdl3/window_driver.c.md`, *The driver*.
`SDLK_V` is the key of the letter V in the layout of the user, so a paste follows the layout of the keyboard.
A key press with Alt is no paste, because Ctrl and Alt together are the key AltGr on Windows.
A key that the system repeats is a paste too, as in a text field of the system.

## The entities of the pastes

`spawn_pastes` spawns an entity of `ideatek:Paste` for each paste of `paste_count`:

```pseudo
PROCEDURE spawn_pastes()
   DECLARE text : STRING
   DECLARE paste : ideatek_paste_t
   IF window_module.paste_count > 0 AND has_room(1, 32) THEN
      text <- SDL_GetClipboardText()
      IF LENGTH(text) <= window_module.pastes.size - window_module.pastes.used THEN
         paste.text <- put_bytes(window_module.pastes, text)
         WHILE window_module.paste_count > 0 AND has_room(1, 32)
            CALL spawn_item(PASTE, paste)
            window_module.paste_count <- window_module.paste_count - 1
         ENDWHILE
      ELSE
         CALL bridge_native_say(BRIDGE_LOG_WARN, "the paste needs <count> bytes, and the half has <count> free bytes")
         window_module.paste_count <- 0
      ENDIF
      CALL SDL_free(text)
   ENDIF
ENDPROCEDURE
```

The pastes of 1 round share 1 copy of the text, because the clipboard does not change during a round.
A paste takes 1 spawn and 32 bytes of `changes`. Without the room, the next round spawns the rest of the pastes.
`SDL_GetClipboardText` gives an empty text when the clipboard holds no text. Then the paste has a `text` of length 0.

## The branches

| Case | Result |
| --- | --- |
| The game attaches `ideatek:ClipboardRequest`. | The clipboard holds the text. |
| A text outside of the persistent arena. | The line of the text, and the clipboard does not change. |
| No memory for the copy. | The line of the memory, and the clipboard does not change. |
| SDL3 refuses the text. | The line of no clipboard. |
| Ctrl+V, or Cmd+V on macOS. | 1 entity of `ideatek:Paste` with the text of the clipboard. |
| V with no modifier, or with Alt. | No paste. The key press stays in the queue. |
| 2 pastes in 1 round. | 2 entities that share 1 copy of the text. |
| A text that does not fit in the free bytes of the half. | The line of the paste, and no entity. |
| A clipboard with no text. | 1 entity with a `text` of length 0. |
| More than 64 key presses in 1 round. | A paste after the 64th press gets no entity. |

## Functions

| Function | Result |
| --- | --- |
| `bridge_clipboard` | Calls `set_clipboard` for each trigger. Gives 0. |
| `set_clipboard` | Puts the text of a request on the clipboard, or writes the line of the failure. |
| `count_pastes` | Adds the pastes of the key presses of the round to `paste_count`. |
| `is_paste` | Tells if a key press is a paste. |
| `spawn_pastes` | Spawns the entities of the pastes of the round, while the round has room. |

# src/platform_abstraction/window_sdl3/window_items.c

This file keeps the items of a round: the entities of `ideatek:Drop` and `ideatek:Paste`, and their bytes.
An item lives for 1 round. Its bytes stay good while its entity lives: `docs/extern/platform_abstraction/ideatek_window.h.md`, *The drops*.

## The first round

Bridge writes the settings after `init`, so the first round of `events` reads them with `start_items`:

1. If `drop_bytes` is less than 2, more than 1073741824, or odd, it writes the line of the range.
2. It does the same check for `clipboard_bytes`.
3. After a line of step 1 or 2, it gives `false`, and `events` gives 1.
4. Else `alloc` takes `drop_bytes` and then `clipboard_bytes` from the share of the module in the persistent arena.
5. `size` of each `window_half_t` is the half of its bytes, and it gives `true`.

The line and the failure follow `readme.md`, *The contracts*. The range in the line is `2 to 1073741824, an even number`.
A setting out of its range fails each round, and the line comes in each round, until Bridge stops the module or the world.
The share must hold `drop_bytes`, `clipboard_bytes` and the text of `icon`. Bridge rounds each one up to a multiple of 8.
If the share is too small, Bridge refuses `alloc`, and its record names the reason: https://github.com/Encrid/Bridge/blob/v0.4.0/docs/extern/bridge_abi.h.md#alloc.

## A turn

`turn_items` starts each round, before the events:

```pseudo
PROCEDURE turn_items()
   FOR i <- 0 TO window_module.item_count - 1
      CALL destroy(window_module.items[i])
   NEXT i
   window_module.item_count <- 0
   window_module.spawned <- 0
   window_module.drops.side <- 1 - window_module.drops.side
   window_module.drops.used <- 0
   window_module.pastes.side <- 1 - window_module.pastes.side
   window_module.pastes.used <- 0
ENDPROCEDURE
```

The items of the last round go at the barrier of this phase, and their bytes are in the other half.
Thus a reader in a phase before that barrier still reads good bytes.
The module spawns at most `WINDOW_SPAWNS` items in a round, so the destroys take at most 6144 bytes of `changes`.
They come first in the round, so they always fit in a deployment that keeps the default of `changes`.

## The bytes

`put_bytes` puts a text after the used bytes of the half:

```pseudo
FUNCTION put_bytes(BYREF half : window_half_t, bytes : STRING) RETURNS bridge_text_t
   DECLARE text : bridge_text_t
   text.at <- half.at + half.side * half.size + half.used
   text.len <- LENGTH(bytes)
   CALL memcpy(shared_base + text.at, bytes, text.len)
   half.used <- half.used + text.len
   RETURN text
ENDFUNCTION
```

The caller checks that the text fits in the free bytes of the half before the call.
A text of a `bridge_text_t` has no zero at its end, so the copy takes no zero.

## An item

`spawn_item` spawns an entity with `spawn_entity`, attaches the value of `slot` to it, and adds the entity to `items`.
The caller checks the room of the round before the call: `docs/intern/platform_abstraction/window_sdl3/window_world.c.md`, *The room of a round*.

## The branches

| Case | Result |
| --- | --- |
| The first round, with the defaults. | 2 areas: 32768 and 8192 bytes in each half. |
| `drop_bytes` or `clipboard_bytes` of 0, of 1073741826, or odd. | The line of the setting, and the system fails. |
| A share too small for the settings. | Bridge refuses `alloc`, and the system fails. |
| Items in a round. | They go at the barrier of the next round. |
| 2 rounds in a row. | The 2 rounds use the 2 halves. |

## Functions

| Function | Result |
| --- | --- |
| `start_items` | Checks the 2 settings of the bytes, and takes their areas. Gives `true`, or `false` after a line. |
| `turn_items` | Destroys the items of the last round, and turns the halves. |
| `put_bytes` | Copies a text into the half of the round, and gives its `bridge_text_t`. |
| `spawn_item` | Spawns an item, attaches its value, and keeps its entity. |

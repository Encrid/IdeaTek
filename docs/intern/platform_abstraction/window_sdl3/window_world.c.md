# src/platform_abstraction/window_sdl3/window_world.c

This file holds 2 calls on the storage of Bridge that the sources share.

## The room of a round

Bridge refuses a spawn after the block of `spawns`, and a change after the room of `changes`.
A refused call fails the system: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge_abi.h.md#bridge_entries_t.
Thus each source checks the room with `has_room` before it spawns or attaches:

```pseudo
FUNCTION has_room(spawns : U32, bytes : U32) RETURNS BOOLEAN
   RETURN window_module.spawned + spawns <= WINDOW_SPAWNS AND changes_left >= bytes
ENDFUNCTION
```

`changes_left` is the field of the header that Bridge writes after each change.
Bridge tells no count of the spawns that are left. Thus `spawn_entity` counts the spawns of the round in `spawned`, and `turn_items` sets it to 0.

| Change | Spawns | Bytes of `changes` |
| --- | --- | --- |
| A destroy | 0 | `CHANGE_HEAD` |
| An attach of `ideatek:WindowState` | 0 | `CHANGE_BYTES(ideatek_window_state_t)` |
| A new display | 1 | `CHANGE_BYTES(ideatek_display_t)` |
| A new video mode | 1 | `CHANGE_BYTES(ideatek_video_mode_t)` |
| A new drop | 1 | `CHANGE_BYTES(ideatek_drop_t)` |
| A new paste | 1 | `CHANGE_BYTES(ideatek_paste_t)` |

The manifest states `spawns` at 256, which is `WINDOW_SPAWNS`, and `changes` at 65536.
A deployment that states less `spawns` makes the spawns fail. A deployment that states less `changes` makes the rounds do less work.
A display with 255 modes takes 14464 bytes. Thus each display fits in 1 round of the default.

## The branches

| Case | Result |
| --- | --- |
| A live entity with the component. | Where the value is. |
| A live entity without the component. | 0. |
| 256 spawns in a round. | No room for 1 more spawn in the round. |
| Less room in `changes` than the change needs. | No room. |

## Functions

| Function | Result |
| --- | --- |
| `spawn_entity` | Spawns an entity, and counts it in `spawned`. |
| `has_room` | Tells if the round has room for `spawns` more spawns and `bytes` more bytes of changes. |

# src/platform_abstraction/window_sdl3/window_world.c

This file holds 3 calls on the storage of Bridge that the sources share.

## The cell of a component

`get_cell` gives where the component `slot` of the entity `id` is, in the shared memory, or 0:

```pseudo
FUNCTION get_cell(id : U64, slot : U32) RETURNS U32
   DECLARE record : bridge_entity_t
   DECLARE base : U32
   record <- entity_index[id MOD 4294967296]
   IF record.generation <> id DIV 4294967296 THEN
      RETURN 0
   ENDIF
   base <- column_base[record.table * types + slot]
   IF base = 0 THEN
      RETURN 0
   ENDIF
   RETURN base + record.row * stride[slot]
ENDFUNCTION
```

It follows the steps of https://github.com/Encrid/Bridge/blob/v0.4.0/docs/extern/bridge_abi.h.md#bridge_entity_t.
It gives 0 for a dead entity, and for an entity that does not carry the component.
The module calls it only with an id that Bridge gave in a trigger, so the index of the id is in the entity index.

## The room of a round

Bridge refuses a spawn after the block of `spawns`, and a change after the room of `changes`.
A refused call fails the system: https://github.com/Encrid/Bridge/blob/v0.4.0/docs/extern/bridge_abi.h.md#bridge_entries_t.
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
| A destroy | 0 | 24 |
| An attach of `ideatek:WindowState` | 0 | 104 |
| A new display | 1 | 184 |
| A new video mode | 1 | 56 |
| A new drop | 1 | 72 |
| A new paste | 1 | 32 |

The manifest states `spawns` at 256, which is `WINDOW_SPAWNS`, and `changes` at 65536.
A deployment that states less `spawns` makes the spawns fail. A deployment that states less `changes` makes the rounds do less work.
A display with 255 modes takes 14464 bytes. Thus each display fits in 1 round of the default.

## The branches

| Case | Result |
| --- | --- |
| A live entity with the component. | Where the value is. |
| A live entity without the component. | 0. |
| A dead entity. | 0. |
| 256 spawns in a round. | No room for 1 more spawn in the round. |
| Less room in `changes` than the change needs. | No room. |

## Functions

| Function | Result |
| --- | --- |
| `get_cell` | Gives where the component of an entity is, or 0. |
| `spawn_entity` | Spawns an entity, and counts it in `spawned`. |
| `has_room` | Tells if the round has room for `spawns` more spawns and `bytes` more bytes of changes. |

# src/platform_abstraction/input_sdl3/input_items.c

This file spawns the entity of each event, gives it its rank, and destroys it in the next round: `docs/extern/platform_abstraction/ideatek_input.h.md`, *The events*.

## The room

`has_room` tells if the round can spawn `spawns` more entities and attach `bytes` more bytes:

- The entities of the round plus `spawns` are at most `round_entities`.
- `changes_left` of the header of Bridge is at least `bytes`, plus the bytes of the attach of the pending run: `docs/intern/platform_abstraction/input_sdl3/input_runs.c.md`.

An event takes 1 spawn, and 24 bytes for the attach plus the size of its value.
A run attaches its value at its end, so the room keeps its bytes until then.
The module counts its own spawns, because Bridge does not tell how many spawns remain: https://github.com/Encrid/Bridge/blob/v0.5.0/docs/extern/bridge_abi.h.md#spawn.
The deployment states `spawns` of at least `round_entities`, so a spawn of the module never fails.

## An event

`spawn_event` spawns an entity, attaches the value of the event, and keeps the entity in `items`:

```pseudo
PROCEDURE spawn_event(slot : U32, value : BYTES, size : U32)
   DECLARE id : U64
   id <- spawn()
   input_module.spawned <- input_module.spawned + 1
   CALL attach(id, slot, lend(value, size))
   input_module.items[input_module.item_count] <- id
   input_module.item_count <- input_module.item_count + 1
ENDPROCEDURE
```

`next_order` gives the rank of the next event, and adds 1 to `order`. Each source calls it when it fills the value of an event.
A caller checks the room with `has_room` before it fills the value, so the rank goes only to an event that gets its entity.

## The next round

`turn_items` destroys each entity of `items`, and sets `item_count`, `spawned` and `order` to 0.
A reader hears each event at the barrier of its round. The destroy of the next round removes it.

## The branches

| Case | Result |
| --- | --- |
| Events in a round. | They get the ranks 0, 1, 2 and so on, in their order. |
| 2 rounds in a row. | The events of the first round go in the second round, and the ranks start at 0 again. |
| `round_entities` events in a round. | No room for 1 more event in the round. |
| Less room in `changes` than an event needs. | No room. |

## Functions

| Function | Result |
| --- | --- |
| `has_room` | Tells if the round has room for more entities and bytes. |
| `next_order` | Gives the rank of the next event of the round. |
| `spawn_event` | Spawns the entity of an event, and keeps it for the next round. |
| `turn_items` | Destroys the entities of the events of the last round. |
| `spawn_entity` | Spawns an entity that the module keeps, such as a gamepad, and counts it. |

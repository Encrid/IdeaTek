# src/kernel/resources_files/resources_hash.c

This file is the hash map of the module. It is pure: the program `resources_hash` of the tests calls it on a span of its own.

## The map

The map is open addressing with linear probing. The home slot of a key is `key % slots`. A search walks from the home slot until it finds the key or a free slot.

- `map_find` gives the value of a key, or `RES_NONE` when the key is not in the map.
- `map_put` writes the value of a key. It takes the first removed or free slot of the walk when the key is new. It gives `false` when the map is full.
- `map_drop` frees the slot of a key. Then it moves down each later slot of the walk whose home is before the hole, so the walks of the other keys stay whole.
- `map_clear` frees each slot.

A key of 0 is not allowed: an entity id is never 0, and a hash of 0 becomes 1.

```pseudo
FUNCTION map_find(map : U64, key : U64) RETURNS U32
   i <- key MOD map.slots
   WHILE map.slot[i].key <> 0
      IF map.slot[i].key = key THEN
         RETURN map.slot[i].value
      ENDIF
      i <- (i + 1) MOD map.slots
   ENDWHILE
   RETURN RES_NONE
ENDFUNCTION
```

## The keys

`docs/intern/kernel/resources_files/resources_key.c.md` hashes the keys. A collision of 2 keys is possible, so the caller compares the texts after a find.

## Functions

| Function | Result |
| --- | --- |
| `map_find` | The value of a key, or `RES_NONE`. |
| `map_put` | Writes a value. `false` when the map is full. |
| `moves_down` | `true` when a slot moves into the hole of a removed key. |
| `map_drop` | Removes a key, and moves the slots that its hole would cut. |
| `map_clear` | Frees each slot. |

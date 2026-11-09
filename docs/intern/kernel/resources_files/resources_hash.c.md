# src/kernel/resources_files/resources_hash.c

This file is the hash map of the module. It is pure: the program `resources_hash` of the tests calls it on a span of its own.

## The map

The map is open addressing with linear probing. The home slot of a key is `key % slots`. A search walks from the home slot until it finds the key or a free slot.

- `map_find` gives the value of a key, or `RES_NONE` when the key is not in the map.
- `map_put` writes the value of a key. It takes the first removed or free slot of the walk when the key is new. It gives `false` when the map is full.
- `map_drop` marks the slot of a key as removed. A removed slot keeps the walks of the other keys whole.
- `map_clear` frees each slot.

A key of 0 is not allowed: an entity id is never 0, and a hash of 0 becomes 1.

```pseudo
FUNCTION map_find(map : U64, key : U64) RETURNS U32
   i <- key MOD map.slots
   WHILE map.slot[i].key <> 0
      IF map.slot[i].key = key AND map.slot[i].value <> RES_GONE THEN
         RETURN map.slot[i].value
      ENDIF
      i <- (i + 1) MOD map.slots
   ENDWHILE
   RETURN RES_NONE
ENDFUNCTION
```

## The hash of a text

`res_hash_text` folds each byte of a text into the 64-bit FNV-1a hash, then turns a result of 0 into 1.
The key of a resource is the hash of its path, a zero, and its type. A collision of 2 keys is possible, so the caller compares the texts after a find.

## Functions

| Function | Result |
| --- | --- |
| `res_hash_text` | The FNV-1a hash of a text, never 0. |
| `map_find` | The value of a key, or `RES_NONE`. |
| `map_put` | Writes a value. `false` when the map is full. |
| `map_drop` | Removes a key. |
| `map_clear` | Frees each slot. |

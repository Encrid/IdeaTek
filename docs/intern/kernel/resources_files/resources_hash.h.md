# src/kernel/resources_files/resources_hash.h

This header gives the hash map of the module: 64-bit keys to 32-bit indexes, in a span of memory of the caller.

## The types

`res_map_t` is the map: where its slots are, how many slots it has, and how many are used. Each slot is a `res_slot_t`: a key and a value.
A key of 0 marks a free slot.
The caller gives a span of twice as many slots as the entries that the map must hold, so a search ends fast.
`res_hash_more` and `res_hash_key` give the 64-bit FNV-1a hash of a text and of the key of a resource.

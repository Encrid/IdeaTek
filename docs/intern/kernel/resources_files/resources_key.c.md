# src/kernel/resources_files/resources_key.c

This file compares 2 texts, and hashes the key of a resource. It is pure, and the hash map and the table call it.

## The key

`res_hash_more` folds each byte of a text into a 64-bit FNV-1a hash, from a hash that the caller gives.
`res_hash_key` hashes the path of a resource, a zero byte, and its type, from `RES_HASH_START`. A result of 0 becomes 1, because 0 marks a free slot of the map.
`same_text` tells that 2 texts have the same bytes.

## Functions

| Function | Result |
| --- | --- |
| `same_text` | `true` when 2 texts are equal. |
| `res_hash_more` | The hash of a text, folded after a hash. |
| `res_hash_key` | The key of a path and a type, never 0. |

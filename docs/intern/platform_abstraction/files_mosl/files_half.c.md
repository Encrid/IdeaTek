# src/platform_abstraction/files_mosl/files_half.c

This file takes the 2 halves, turns the half of the round, writes bytes into it, and tells the room of `changes`.

## The round

`take_halves` takes `read_bytes` bytes of the persistent arena, and gives each half the half of them.
`turn_results` destroys the result entities of the round before, then swaps the sides of the half and empties the side of the round.
The destroys always fit in `changes`: a destroy costs less than the attach of its result in the round before.
`put_bytes` copies bytes into the side of the round, and gives their text.
`has_room` tells if `changes` has room for a change of the given bytes.

## Functions

| Function | Result |
| --- | --- |
| `has_room` | Gives `true` when `changes` has the bytes left. |
| `take_halves` | Takes the bytes of the 2 halves from the persistent arena. |
| `turn_results` | Destroys the result entities of the round before, and swaps the sides of the half. |
| `put_bytes` | Copies bytes into the side of the round, and gives their text. |

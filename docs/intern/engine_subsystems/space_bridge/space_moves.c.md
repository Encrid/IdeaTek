# src/engine_subsystems/space_bridge/space_moves.c

This file keeps the ring of `ideatek:WorldMoves`: `docs/extern/engine_subsystems/ideatek_space.h.md`, *ideatek:WorldMoves*.

## The start

Each observer and the system run their work through `run_started`. It starts the ring at the first call of the module:

1. It reads `moves` of the settings. A value above 16777216 writes `moves: <value> is outside 0 to 16777216`, and the module is broken.
2. With a `moves` above 0, it takes `moves` times 8 bytes of the persistent arena for the ring.
   A `persistent` that has no room stops the module: Bridge writes its reason.
3. It attaches `ideatek:WorldMoves` with a `total` of 0. No move comes before the start, so the value is exact.

While the module is broken, `run_started` runs no work and gives 1, so the entry gives 1. Else it runs the work and gives 0.
So the check of a broken module is in 1 place for the 6 entries.

## The moves

`write_move` takes each write in place of a place, from `write_world`: `docs/intern/engine_subsystems/space_bridge/space_one.c.md`.
It writes the entity at `at` of the ring, moves `at` to the next value, and adds 1 to `total`.
`at` goes back to 0 after the last value, so `at` is always `total` modulo `capacity`, with no division.
With no ring, it only adds 1 to `total`.

`publish_moves` attaches `ideatek:WorldMoves` with the new `total`, when the call wrote moves. `place_all` and `relink_all` call it at their end.
So 1 call costs 1 attach, whatever its count of moves. A reader sees the new `total` after the barrier.

## Functions

| Function | Result |
| --- | --- |
| `take_moves` | Checks `moves`, takes the ring and attaches the singleton. With `moves` outside its range, it writes the line and marks the module broken. |
| `run_started` | Runs `take_moves` at the first call, then the work of the entry. Gives 1 while the module is broken, else 0. |
| `write_move` | Puts 1 entity in the ring and counts it. |
| `publish_moves` | Attaches the singleton with the new `total`, when the call wrote moves. |

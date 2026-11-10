# src/engine_subsystems/space_bridge/space_place.c

This file runs the observers `place` and `relink`.

1. The function puts each entity of the list in a set: `docs/intern/engine_subsystems/space_bridge/space_set.c.md`.
2. It walks the parents of each entity: `docs/intern/engine_subsystems/space_bridge/space_ancestor.c.md`.
3. An entity in a cycle loses its place, and the log gets 1 line. A root is walked: `docs/intern/engine_subsystems/space_bridge/space_walk.c.md`.

When the set cannot hold the list, the entities out of the set skip the test, so an entity can be placed 2 times. The log gets 1 line for the call.

## Functions

| Function | Result |
| --- | --- |
| `fill_set` | Puts the entities of the list in the set. Gives how many did not fit. |
| `place_entity` | Places 1 entity of the list, or writes the line of a cycle. |
| `place_list` | Places the entities of the list of 1 observer. |
| `place` | The observer of `ideatek:Transform`. Gives 0. |
| `relink` | The observer of `ideatek:Sibling`. Gives 0. |

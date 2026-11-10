# src/engine_subsystems/space_bridge/space_place.c

This file runs the observers `place` and `relink`: it places the entities whose transform or list changed.

## The call

1. The function puts each entity of the list in a set of the scratch arena: `docs/intern/engine_subsystems/space_bridge/space_set.c.md`.
2. For each entity, it walks up its parents: `docs/intern/engine_subsystems/space_bridge/space_ancestor.c.md`.
3. An entity with an ancestor in the set waits for the walk of that ancestor. A root or an entity whose ancestors stayed is walked: `docs/intern/engine_subsystems/space_bridge/space_walk.c.md`.

A list with more entities than the set holds puts the rest in no set. Their walk skips the test, so an entity can be placed 2 times, and the log gets 1 line.

## The branches

| Case | Result |
| --- | --- |
| The entity has an ancestor in the set. | Nothing in this step. |
| The entity has no changed ancestor. | The walk. |
| The entity is in a cycle. | 1 line, and no place. |
| The set is full. | The walk with no test, and 1 line for the call. |

## Functions

| Function | Result |
| --- | --- |
| `fill_set` | Puts the entities of the list in the set. Gives how many did not fit. |
| `place_list` | Places the entities of 1 list. |
| `place` | The observer of `ideatek:Transform`. Gives 0. |
| `relink` | The observer of `ideatek:Sibling`. Gives 0. |

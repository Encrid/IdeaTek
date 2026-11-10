# src/engine_subsystems/space_bridge/space_keep.c

This file keeps the place in the world of a child whose parent changes or goes.

## The keep

`keep_world` reads the `ideatek:WorldTransform` of the child, and the place of its new parent. It attaches the `ideatek:Transform` that gives the same place: `docs/intern/engine_subsystems/space_bridge/space_math.c.md`.
A child with no place keeps its transform. A parent with no inverse keeps the transform too, and the log gets 1 line.

`orphan_keep` applies `IDEATEK_ORPHAN_KEEP_WORLD`: it detaches `ideatek:Parent` and `ideatek:Sibling`, and attaches the place in the world as the transform of a root.

## Functions

| Function | Result |
| --- | --- |
| `keep_world` | Attaches the transform of the child under its new parent. |
| `orphan_keep` | Makes the child a root at its place. |

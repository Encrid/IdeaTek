# src/kernel/resources_files/resources_tags.c

This file detaches the tags of 1 round.

## The tags

`turn_tags` runs first in each round. `drop_tags` detaches the tags that the round before attached to a resource: `ideatek:ResourceReady`, `ideatek:ResourceFailed`, `ideatek:ResourceModified` and `ideatek:RawBytes`.
With `ideatek:RawBytes`, the block of the bytes is freed too: the owner copied them in the round before.

## Functions

| Function | Result |
| --- | --- |
| `drop_tags` | Detaches the tags of 1 resource. |
| `turn_tags` | Detaches the tags of the round before. |

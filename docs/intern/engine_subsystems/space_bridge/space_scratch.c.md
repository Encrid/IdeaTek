# src/engine_subsystems/space_bridge/space_scratch.c

This file gives the module its reads of values and its buffers in the scratch arena.

`take_scratch` counts its bytes in `scratch_used`, so a later log line or a later attach puts its bytes above them. Each barrier frees them.
`take_stack` takes at most half of the free scratch arena, and at most `STACK_ROOM` items, so the rest stays for the log and the attaches.

## Functions

| Function | Result |
| --- | --- |
| `space_find` | The cell of a type on an entity, or `nullptr`. `SPACE_FIND` reads it, and `SPACE_EDIT` writes it. |
| `take_scratch` | A buffer of the scratch arena, or `nullptr`. |
| `take_set` | A set of a room. `false` when the scratch arena cannot hold it. |
| `get_parent` | The parent of an entity, or 0. |
| `take_stack` | A stack for the walks of 1 call. |

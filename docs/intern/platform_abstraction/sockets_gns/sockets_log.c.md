# src/platform_abstraction/sockets_gns/sockets_log.c

This file takes the lines of GameNetworkingSockets, and writes them into the log of the module.

## The output

The first round gives GameNetworkingSockets `on_output` and the level of `debug_output`: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.
GameNetworkingSockets calls `on_output` from any thread, with a level and a text. Under `lock`, `on_output` writes the text into the ring `lines`, cut at `SOCKETS_LINE` bytes. A line that does not fit in the ring is lost.

A level of bug or error is an error of the log. A level of important or warning is a warning. Each other level is information.

`say_lines` runs once a round. It empties the ring under `lock`, and writes `gns: <text>` for each record, with its kind. The kind goes in the ring before the text, as 1 byte.

## The branches

| Case | Result |
| --- | --- |
| A line below the level. | GameNetworkingSockets does not call. |
| A line that fits in the ring. | `gns: <text>` in the next round. |
| A line that does not fit. | Nothing. |

## Functions

| Function | Result |
| --- | --- |
| `kind_of` | Gives the kind of the log for a level of GameNetworkingSockets. |
| `on_output` | Writes a line into the ring under the lock. |
| `say_lines` | Writes each line of the ring into the log. |

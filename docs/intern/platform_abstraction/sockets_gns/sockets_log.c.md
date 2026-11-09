# src/platform_abstraction/sockets_gns/sockets_log.c

This file takes the lines of GameNetworkingSockets, writes them into the log of the module, and writes the lines of the requests.

## The output

The first round gives GameNetworkingSockets `on_output` and the level of `debug_output`: `docs/intern/platform_abstraction/sockets_gns/sockets_start.c.md`.
GameNetworkingSockets calls `on_output` with a level and a text, from a call of the module: `docs/intern/platform_abstraction/sockets_gns/sockets_round.c.md`, *The manual poll mode*. A callback cannot write the log, so `on_output` writes the text into the ring `lines`, cut at `SOCKETS_LINE` bytes: `docs/intern/platform_abstraction/sockets_gns/sockets_ring.c.md`. A line that does not fit in the ring is lost.

A level of bug, error, important or warning is a warning of the log. Each other level is information. An error of the log would end the run of bridge-host, and a line of GameNetworkingSockets is not an error of the module.

`say_lines` runs once a round. It empties the ring, and writes `gns: <text>` for each record, with its kind. The kind goes in the ring before the text, as 1 byte.

## The requests

`say_request` writes the line of a wrong request: the entity, the field with its value, and the range. A request on the wrong entity gets the same line.
`say_bound` writes `more than <max> <what>: the request of <id> fails` for a request above a bound of the settings.

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
| `on_output` | Writes a line into the ring. |
| `say_lines` | Writes each line of the ring into the log. |
| `say_request` | Writes the line of a wrong request. |
| `say_bound` | Writes the line of a request above a bound. |

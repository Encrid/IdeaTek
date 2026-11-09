# src/platform_abstraction/sockets_gns/sockets_lanes.c

This file runs the observer of `ideatek:LanesRequest`: it configures the lanes of a connection.

## The request

The observer `lanes` gets each write of `ideatek:LanesRequest` on a connection entity:

1. A request on an entity with no connection, or on a session, writes `the request of <id>: lanes is outside a connection`, and nothing changes.
2. `judge_lanes` checks the shape of the batch. A record is 6 bytes: a priority of 4 bytes and a weight of 2 bytes. A wrong shape writes its line with the range of the lanes, and nothing changes. The shape is wrong with 0 or over 255 records, leftover bytes, or fewer records than the lanes.
3. `apply_lanes` reads the records with `read_lanes`. A weight of 0 writes `the request of <id>: lanes <count> is outside weights of 1 or more`, and nothing changes.
4. GameNetworkingSockets configures the lanes. A refusal writes `the request of <id>: lanes <count> is outside the lanes that GameNetworkingSockets takes: refused`.
5. The slot keeps the count of the lanes.

The lane states of each round are in `docs/intern/platform_abstraction/sockets_gns/sockets_lane_states.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A request on an entity with no connection, or a session. | Its line, and nothing. |
| A batch with a wrong shape, or too few records. | Its line, and nothing. |
| A weight of 0. | Its line, and nothing. |
| GameNetworkingSockets refuses the lanes. | Its line, and nothing. |
| Good lanes. | The connection has them, and the slot keeps their count. |

## Functions

| Function | Result |
| --- | --- |
| `read_lanes` | Reads the priorities and the weights of a batch. Gives `false` for a weight of 0. |
| `judge_lanes` | Checks the shape of a batch against the lanes of the connection. Gives `false` with its line. |
| `apply_lanes` | Reads the lanes and gives them to GameNetworkingSockets, or writes the line. |
| `lanes` | Finds the connection of each trigger of the observer, and sets its lanes. |

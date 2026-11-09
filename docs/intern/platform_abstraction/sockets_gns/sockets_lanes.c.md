# src/platform_abstraction/sockets_gns/sockets_lanes.c

This file runs the observer of `ideatek:LanesRequest`, and attaches `ideatek:LaneStates` in each round.

## The request

The observer `lanes` gets each write of `ideatek:LanesRequest` on a connection entity:

1. `read_lanes` reads the records of the batch, up to 255. A record is 6 bytes: a priority of 4 bytes and a weight of 2 bytes.
2. A wrong batch writes `the request of <id>: lanes <count> is outside <range>`, and nothing changes. A batch is wrong with 0 or over 255 records, leftover bytes, a weight of 0, or too few records.
3. A request on an entity with no connection, or on a session, writes `the request of <id>: lanes is outside a connection`, and nothing changes.
4. GameNetworkingSockets configures the lanes. A refusal writes the same line as a wrong count, with its reason.
5. The slot keeps the count of the lanes.

## The round

`attach_lanes` runs once a round, for the connections with more than 1 lane. It asks GameNetworkingSockets for the real time status of each lane, and writes 1 record of 20 bytes for each lane into the half. A record holds `queue_time`, `pending_unreliable`, `pending_reliable` and `sent_unacked_reliable`.
Then it attaches `ideatek:LaneStates` to the entity. A batch with no room in the half or in `changes` waits for the next round.

## The branches

| Case | Result |
| --- | --- |
| A batch outside its range. | Its line, and nothing. |
| A request on an entity with no connection, or a session. | Its line, and nothing. |
| GameNetworkingSockets refuses the lanes. | Its line, and nothing. |
| A connection with 1 lane. | No `ideatek:LaneStates`. |
| No room for the lane states. | They wait for the next round. |

## Functions

| Function | Result |
| --- | --- |
| `read_lanes` | Reads the priorities and the weights of a batch. Gives the count, or 0 for a wrong batch. |
| `set_lanes` | Configures the lanes of a connection, and keeps their count. |
| `lanes` | Reads the batch of each trigger of the observer, and sets the lanes. |
| `write_lanes` | Writes the records of the lanes of a connection into the half. |
| `attach_lanes` | Attaches `ideatek:LaneStates` to each connection with more than 1 lane. |

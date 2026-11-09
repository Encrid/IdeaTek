# src/platform_abstraction/sockets_gns/sockets_lane_states.c

This file attaches `ideatek:LaneStates` in each round, for the connections with more than 1 lane.

## The round

`attach_lanes` runs once a round. It looks at each live connection with more than 1 lane, with room in the half for its records. For each one, `write_lanes` asks GameNetworkingSockets for the real time status of each lane.
It writes 1 record of 20 bytes for each lane into the half: `queue_time`, `pending_unreliable`, `pending_reliable` and `sent_unacked_reliable`. Then `attach_lanes` attaches `ideatek:LaneStates` to the entity.

The records of a connection whose status GameNetworkingSockets does not give hold zeros. The lanes are zeroed before the call, and the call is no branch.
A batch that does not fit in the half comes in the next round.

## The branches

| Case | Result |
| --- | --- |
| A connection with 1 lane. | No `ideatek:LaneStates`. |
| A closed or forgotten connection. | No `ideatek:LaneStates`. |
| No room in the half for the lane states. | They wait for the next round. |
| A connection with 2 lanes or more, and room. | A batch with 1 record for each lane. |

## Functions

| Function | Result |
| --- | --- |
| `write_lanes` | Writes the records of the lanes of a connection into the half, and gives their text. |
| `attach_lanes` | Attaches `ideatek:LaneStates` to each connection with more than 1 lane. |

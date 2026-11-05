# include/engine_subsystems/ideatek_networking.h

`include/engine_subsystems/ideatek_networking.h` gives the types of the networking of a game: net sessions, peers, replication, relevance, commands and the network clock.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the world part, because its fields name entities.

## The roles

- The game writes the net session, the join data and the answers to the joins.
- The game also writes the replicated entities, the bytes of its modules, the viewers, the relevance and the commands.
- The net module runs the handshake and the transport over the connections of the sockets part. It writes the peers, the events, the network ids and the state of the net session. It also writes the bytes and the commands that come in.
- The sockets part of layer 2 gives the connections, the encryption, the messages and the statistics. The net module reads them.

## The net sessions and the peers

The role of the net session tells what the module does. A server listens, a client connects, and a host does both and has a local player.
The client sends `protocol` and the bytes of `ideatek:NetJoin`. A protocol that differs is refused.
Else the server spawns `ideatek:NetJoinEvent`, and the game of the server answers with `ideatek:NetAdmit` on the peer.

Peer 1 is the server. On a host, the local peer is READY with a connection of 0.
When the connection closes, the state of the peer is LEFT, and the module destroys the peer 1 round later.
Solo is a HOST session with a `listen` of 0, and it takes no join. When the game writes a listen socket, the session takes joins, as Open to LAN does.
So the same code runs in solo and with other players.

## The replication

The game of the server writes `ideatek:Replicated` on each entity that the clients see. The module gives it `ideatek:NetEntity`, and spawns the same prefab on each client.
Each module serializes its own rows, field by field, into `ideatek:NetOut`. The bytes are records of `{net_id uint64_t, length uint32_t, bytes}`, little-endian.

A STATE goes as a delta from the last state that the peer acked. It is unreliable, and the latest wins. An EVENT goes reliable and in order, once.
On a client, the module writes 1 `ideatek:NetIn` for each tag in a round. The module that owns the tag reads it and writes its rows.

An EVENT from a client to the server is a request. The server does not trust it.

## The relevance

An entity with no `ideatek:NetRelevance` is relevant to each peer. DISTANCE means within `distance` of a viewer of the peer. OWNER means only the owner.
GROUP means the peers whose groups share a bit with `groups`. A peer with no `ideatek:NetPeerGroups` has the groups 1.

VISIBLE means the peers with a viewer that sees the entity. The module tests the line of sight with the physics, or with its own sets of visibility.
An entity sends in 1 step of each `steps`. A `steps` of 0 takes the setting `snapshot_steps`.
`condition` of `ideatek:NetOut` limits the peers of a tag: only the owner, each peer but the owner, or only the first send.

When the bytes of a peer pass its budget, the module sends by `priority` times the time since the last send. The rest waits.
When an entity leaves the relevance of a peer, the module despawns it on that peer. When it comes back, the module spawns it with its full state.

## The authority

`authority` of `ideatek:Replicated` tells who computes the state of the entity. `owner` tells whose commands drive it.
- SERVER: the server simulates the entity, as for a shooter.
- OWNER: the owner sends the state of the entity. The server applies it and relays it, as for a co-op game.
- HOST: the peer `host` simulates it and sends its state, for example a host of the physics of a zone.
- DETERMINISTIC: each peer of `group` computes it from the inputs: *The deterministic groups*.

When the server writes a new `authority` or `host`, the module spawns `ideatek:NetAuthorityChanged`. Each entity has its own authority, so 1 game can mix the 4.
When a host peer leaves, the module chooses the READY peer of the lowest id as the new host, and spawns `ideatek:NetHostChanged`.

## The deterministic groups

The inputs of the peers of a group go to each peer of that group. `confirmed` is the last tick with all the inputs of the group of the local peer.
A peer predicts each input that has not come. When a remote input differs from the prediction, the module spawns `ideatek:NetRollback` with its tick.
The game then restores a snapshot of the physics at that tick and replays the steps with the true inputs. `input_delay` adds steps before a command applies, so fewer rollbacks occur.

A group fits a duel, a vehicle or a mini-game. A whole world of many players does not fit, because each peer of a group needs each input of the group.

## The predicted spawns

A client can show a new entity at once, for example a projectile. It spawns the entity with `ideatek:NetPredictedSpawn` and sends the key in its command.
The game of the server writes the same key on its replicated entity. The module of the client then joins the 2: the entity of the server takes the place of the entity of the client.

## The commands and the prediction

In each step, the game of a client writes `ideatek:NetCommand`. Its bytes are the command of the game, for example from its action states.
The module sends each command that the server did not ack yet, unreliable. The server gets them in the order of the ticks, 1 for each step. A missing command repeats the last one.

Each STATE tells `ack`: the last command that the server applied. The prediction is code of the game: it applies the state, then replays the commands after `ack`.
`view_tick` is the playback tick of the client. The server copies it into `tick` of a query of the physics, for the lag compensation.

## The playback and the clock

The tick of the server is the number of its fixed steps since the start of the net session. Each STATE carries it.
On a client, the module plays the states at `playback`: the newest tick minus `interp_ratio` intervals of the snapshots. So each module applies the same tick.
The records of the entities that the client owns come at once, with the kind PREDICTED. A module that writes a place from the network attaches `ideatek:Interpolated` of the space part.

`command_tick` is the estimate plus half the round trip plus 1 step. The filter of the estimate is a choice of the module.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `interp_ratio` | 2 | 1 or more | The intervals of the snapshots between the newest tick and the playback. 2 survives 1 lost snapshot. |
| `snapshot_steps` | 1 | 1 or more | The server sends a state each this many steps. |
| `input_delay` | 0 | 0 or more | The steps between a command and its tick in a deterministic group. The game sets it. |

## ideatek:ideatek_net_role_t

`ideatek_net_role_t` tells the role of a net session.
`ideatek_net_role_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NET_ROLE_SERVER` | A server with no local player. |
| 1 | `IDEATEK_NET_ROLE_CLIENT` | A client. |
| 2 | `IDEATEK_NET_ROLE_HOST` | A server with a local player. |

## ideatek:NetSession

`ideatek:NetSession` starts a net session on connections of the sockets part.
`ideatek_net_session_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `connection` | `ideatek_entity_t` | 0 | The connection of a client. |
| `listen` | `ideatek_entity_t` | 8 | The listen socket of a server or a host. |
| `protocol` | `uint32_t` | 16 | The version of the protocol of the game. |
| `role` | `ideatek_net_role_t` | 20 | The role. |

Bytes 21 to 23 hold no field.

## ideatek:ideatek_peer_state_t

`ideatek_peer_state_t` tells the state of a peer.
`ideatek_peer_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PEER_STATE_JOINING` | The handshake goes on. |
| 1 | `IDEATEK_PEER_STATE_READY` | The peer plays. |
| 2 | `IDEATEK_PEER_STATE_REFUSED` | The peer was refused. |
| 3 | `IDEATEK_PEER_STATE_LEFT` | The connection closed. |

## ideatek:NetPeer

`ideatek:NetPeer` is 1 peer of a net session. The net module writes it.
`ideatek_net_peer_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `session` | `ideatek_entity_t` | 0 | The net session. |
| `connection` | `ideatek_entity_t` | 8 | The connection. 0 means the local peer of a host. |
| `id` | `uint32_t` | 16 | The id of the peer. 1 is the server. |
| `state` | `ideatek_peer_state_t` | 20 | The state. |

Bytes 21 to 23 hold no field.

## ideatek:NetJoin

`ideatek:NetJoin` gives the bytes that a client sends with its join, on its net session entity.
`ideatek_net_join_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | The bytes of the game, for example a name or a ticket. |

## ideatek:NetJoinEvent

`ideatek:NetJoinEvent` tells the server that a peer asks to join.
`ideatek_net_join_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `peer` | `ideatek_entity_t` | 0 | The peer that joins. |
| `data` | `bridge_text_t` | 8 | Its bytes of `ideatek:NetJoin`. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:ideatek_admit_t

`ideatek_admit_t` tells the answer to a join.
`ideatek_admit_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ADMIT_ACCEPT` | The peer joins. |
| 1 | `IDEATEK_ADMIT_REFUSE` | The peer is refused. |

## ideatek:NetAdmit

`ideatek:NetAdmit` answers a join, on the peer entity.
`ideatek_net_admit_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `reason` | `int32_t` | 0 | A reason that the game defines. |
| `action` | `ideatek_admit_t` | 4 | The answer. |

Bytes 5 to 7 hold no field.

## ideatek:ideatek_net_authority_t

`ideatek_net_authority_t` tells who computes the state of an entity: *The authority*.
`ideatek_net_authority_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NET_AUTHORITY_SERVER` | The server computes the state. |
| 1 | `IDEATEK_NET_AUTHORITY_OWNER` | The owner computes the state. |
| 2 | `IDEATEK_NET_AUTHORITY_HOST` | The peer `host` computes the state. |
| 3 | `IDEATEK_NET_AUTHORITY_DETERMINISTIC` | Each peer of `group` computes the state from the inputs. |

## ideatek:Replicated

`ideatek:Replicated` makes the clients see an entity of the server.
`ideatek_replicated_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `prefab` | `uint64_t` | 0 | No default | The hash of the name of the prefab. |
| `group` | `uint64_t` | 8 | 0 | The peer groups of DETERMINISTIC. |
| `owner` | `uint32_t` | 16 | 0 | The id of the owner peer, whose commands drive it. 0 means the server. |
| `host` | `uint32_t` | 20 | 0 | The peer of HOST. |
| `authority` | `ideatek_net_authority_t` | 24 | `IDEATEK_NET_AUTHORITY_SERVER` | Who computes the state. |

Bytes 25 to 31 hold no field.

## ideatek:NetEntity

`ideatek:NetEntity` gives an entity its id in the net session. The net module writes it.
`ideatek_net_entity_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `net_id` | `uint64_t` | 0 | The id of the entity in the net session. |

## ideatek:ideatek_net_kind_t

`ideatek_net_kind_t` tells the kind of the bytes of a module.
`ideatek_net_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NET_KIND_STATE` | A state, as a delta. The latest wins. |
| 1 | `IDEATEK_NET_KIND_EVENT` | An event, reliable and in order. |
| 2 | `IDEATEK_NET_KIND_PREDICTED` | The newest state of the entities that the client owns. |

## ideatek:ideatek_net_condition_t

`ideatek_net_condition_t` limits the peers of the bytes of a tag, as the conditions of Unreal.
`ideatek_net_condition_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NET_CONDITION_ALL` | Each peer of the relevance. |
| 1 | `IDEATEK_NET_CONDITION_OWNER_ONLY` | Only the owner. |
| 2 | `IDEATEK_NET_CONDITION_SKIP_OWNER` | Each peer but the owner. |
| 3 | `IDEATEK_NET_CONDITION_INITIAL_ONLY` | Only in the first send to a peer. |

## ideatek:NetOut

`ideatek:NetOut` holds the bytes that a module sends in a step.
`ideatek_net_out_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `tag` | `uint64_t` | 0 | No default | The hash of the name of the component. |
| `data` | `bridge_text_t` | 8 | No default | The records: *The replication*. |
| `peer` | `uint32_t` | 16 | 0 | The peer. 0 means each peer. |
| `kind` | `ideatek_net_kind_t` | 20 | No default | The kind. |
| `condition` | `ideatek_net_condition_t` | 21 | `IDEATEK_NET_CONDITION_ALL` | The peers that get it. |

Bytes 22 to 23 hold no field.

## ideatek:NetIn

`ideatek:NetIn` holds the bytes that came for a tag. The net module writes it.
`ideatek_net_in_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `tag` | `uint64_t` | 0 | The hash of the name of the component. |
| `data` | `bridge_text_t` | 8 | The records. |
| `peer` | `uint32_t` | 16 | The peer that sent them. |
| `kind` | `ideatek_net_kind_t` | 20 | The kind. |

Bytes 21 to 23 hold no field.

## ideatek:NetViewer

`ideatek:NetViewer` makes the place of its entity a point of view of a peer.
`ideatek_net_viewer_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `peer` | `uint32_t` | 0 | The peer. |

## ideatek:ideatek_relevance_mode_t

`ideatek_relevance_mode_t` tells which peers get an entity.
`ideatek_relevance_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_RELEVANCE_MODE_DISTANCE` | Within a distance of a viewer. |
| 1 | `IDEATEK_RELEVANCE_MODE_ALWAYS` | To each peer. |
| 2 | `IDEATEK_RELEVANCE_MODE_OWNER` | To the owner only. |
| 3 | `IDEATEK_RELEVANCE_MODE_NONE` | To no peer. |
| 4 | `IDEATEK_RELEVANCE_MODE_GROUP` | To the peers of `groups`. |
| 5 | `IDEATEK_RELEVANCE_MODE_VISIBLE` | To the peers that see it. |

## ideatek:NetRelevance

`ideatek:NetRelevance` tells which peers get a replicated entity: *The relevance*.
`ideatek_net_relevance_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | All bits | The peer groups of GROUP. |
| `distance` | `float` | 8 | 0 | The distance of DISTANCE. 0 means no limit. |
| `priority` | `float` | 12 | 1 | The factor of the order of the sends. |
| `steps` | `uint32_t` | 16 | 0 | The steps between 2 sends. 0 means the setting. |
| `mode` | `ideatek_relevance_mode_t` | 20 | `IDEATEK_RELEVANCE_MODE_DISTANCE` | The peers. |

Bytes 21 to 23 hold no field.

Only 1 reference gives a cull distance, so the game chooses it.

## ideatek:NetCommand

`ideatek:NetCommand` is the command of a client for 1 step.
`ideatek_net_command_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | The bytes of the command. |
| `tick` | `uint32_t` | 8 | The tick of the command: `command_tick`. |
| `view_tick` | `uint32_t` | 12 | The playback tick of the client. |

## ideatek:NetCommandIn

`ideatek:NetCommandIn` is the command of a peer for the step of the server. The net module writes it.
`ideatek_net_command_in_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | The bytes of the command. |
| `peer` | `uint32_t` | 8 | The peer. |
| `tick` | `uint32_t` | 12 | The tick of the command. |
| `view_tick` | `uint32_t` | 16 | The playback tick of the peer. |

## ideatek:NetSessionState

`ideatek:NetSessionState` is the clock of a client. The net module writes it.
`ideatek_net_session_state_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `tick` | `uint32_t` | 0 | The estimate of the tick of the server now. |
| `command_tick` | `uint32_t` | 4 | The tick of a new command. |
| `playback` | `uint32_t` | 8 | The tick that the client plays. |
| `ack` | `uint32_t` | 12 | The last command that the server applied. |
| `confirmed` | `uint32_t` | 16 | The last tick with all the inputs of the deterministic group. |
| `host` | `uint32_t` | 20 | The peer that hosts the session now. |

## ideatek:NetPeerGroups

`ideatek:NetPeerGroups` gives a peer its groups, on the peer entity: *The relevance*.
`ideatek_net_peer_groups_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | 1 | The groups of the peer. |

## ideatek:NetPredictedSpawn

`ideatek:NetPredictedSpawn` marks an entity that a client spawned before the server: *The predicted spawns*.
`ideatek_net_predicted_spawn_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `key` | `uint64_t` | 0 | The key that joins the entity of a client to the entity of the server. |

## ideatek:NetRollback

`ideatek:NetRollback` tells that a remote input differs from the prediction. It is an event of 1 round.
`ideatek_net_rollback_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `group` | `uint64_t` | 0 | The deterministic group. |
| `tick` | `uint32_t` | 8 | The first tick to compute again. |
| `order` | `uint32_t` | 12 | The rank in the round. |

## ideatek:NetAuthorityChanged

`ideatek:NetAuthorityChanged` tells that the authority of an entity changed. It is an event of 1 round.
`ideatek_net_authority_changed_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `entity` | `ideatek_entity_t` | 0 | The replicated entity. |
| `old_host` | `uint32_t` | 8 | The host before. |
| `new_host` | `uint32_t` | 12 | The host now. |
| `order` | `uint32_t` | 16 | The rank in the round. |
| `old_authority` | `ideatek_net_authority_t` | 20 | The authority before. |
| `new_authority` | `ideatek_net_authority_t` | 21 | The authority now. |

Bytes 22 to 23 hold no field.

## ideatek:NetHostChanged

`ideatek:NetHostChanged` tells that a new peer hosts the session. It is an event of 1 round.
`ideatek_net_host_changed_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `old_host` | `uint32_t` | 0 | The host that left. |
| `new_host` | `uint32_t` | 4 | The new host. |
| `order` | `uint32_t` | 8 | The rank in the round. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_net_role_t` | `ideatek_net_role_t` | `ideatek_net_role_t_desc` | `IDEATEK_NET_ROLE_VALUES` |
| `ideatek:NetSession` | `ideatek_net_session_t` | `ideatek_net_session_t_desc` | `IDEATEK_NET_SESSION_FIELDS` |
| `ideatek:ideatek_peer_state_t` | `ideatek_peer_state_t` | `ideatek_peer_state_t_desc` | `IDEATEK_PEER_STATE_VALUES` |
| `ideatek:NetPeer` | `ideatek_net_peer_t` | `ideatek_net_peer_t_desc` | `IDEATEK_NET_PEER_FIELDS` |
| `ideatek:NetJoin` | `ideatek_net_join_t` | `ideatek_net_join_t_desc` | `IDEATEK_NET_JOIN_FIELDS` |
| `ideatek:NetJoinEvent` | `ideatek_net_join_event_t` | `ideatek_net_join_event_t_desc` | `IDEATEK_NET_JOIN_EVENT_FIELDS` |
| `ideatek:ideatek_admit_t` | `ideatek_admit_t` | `ideatek_admit_t_desc` | `IDEATEK_ADMIT_VALUES` |
| `ideatek:NetAdmit` | `ideatek_net_admit_t` | `ideatek_net_admit_t_desc` | `IDEATEK_NET_ADMIT_FIELDS` |
| `ideatek:ideatek_net_authority_t` | `ideatek_net_authority_t` | `ideatek_net_authority_t_desc` | `IDEATEK_NET_AUTHORITY_VALUES` |
| `ideatek:Replicated` | `ideatek_replicated_t` | `ideatek_replicated_t_desc` | `IDEATEK_REPLICATED_FIELDS` |
| `ideatek:NetEntity` | `ideatek_net_entity_t` | `ideatek_net_entity_t_desc` | `IDEATEK_NET_ENTITY_FIELDS` |
| `ideatek:ideatek_net_kind_t` | `ideatek_net_kind_t` | `ideatek_net_kind_t_desc` | `IDEATEK_NET_KIND_VALUES` |
| `ideatek:ideatek_net_condition_t` | `ideatek_net_condition_t` | `ideatek_net_condition_t_desc` | `IDEATEK_NET_CONDITION_VALUES` |
| `ideatek:NetOut` | `ideatek_net_out_t` | `ideatek_net_out_t_desc` | `IDEATEK_NET_OUT_FIELDS` |
| `ideatek:NetIn` | `ideatek_net_in_t` | `ideatek_net_in_t_desc` | `IDEATEK_NET_IN_FIELDS` |
| `ideatek:NetViewer` | `ideatek_net_viewer_t` | `ideatek_net_viewer_t_desc` | `IDEATEK_NET_VIEWER_FIELDS` |
| `ideatek:ideatek_relevance_mode_t` | `ideatek_relevance_mode_t` | `ideatek_relevance_mode_t_desc` | `IDEATEK_RELEVANCE_MODE_VALUES` |
| `ideatek:NetRelevance` | `ideatek_net_relevance_t` | `ideatek_net_relevance_t_desc` | `IDEATEK_NET_RELEVANCE_FIELDS` |
| `ideatek:NetCommand` | `ideatek_net_command_t` | `ideatek_net_command_t_desc` | `IDEATEK_NET_COMMAND_FIELDS` |
| `ideatek:NetCommandIn` | `ideatek_net_command_in_t` | `ideatek_net_command_in_t_desc` | `IDEATEK_NET_COMMAND_IN_FIELDS` |
| `ideatek:NetSessionState` | `ideatek_net_session_state_t` | `ideatek_net_session_state_t_desc` | `IDEATEK_NET_SESSION_STATE_FIELDS` |
| `ideatek:NetPeerGroups` | `ideatek_net_peer_groups_t` | `ideatek_net_peer_groups_t_desc` | `IDEATEK_NET_PEER_GROUPS_FIELDS` |
| `ideatek:NetPredictedSpawn` | `ideatek_net_predicted_spawn_t` | `ideatek_net_predicted_spawn_t_desc` | `IDEATEK_NET_PREDICTED_SPAWN_FIELDS` |
| `ideatek:NetRollback` | `ideatek_net_rollback_t` | `ideatek_net_rollback_t_desc` | `IDEATEK_NET_ROLLBACK_FIELDS` |
| `ideatek:NetAuthorityChanged` | `ideatek_net_authority_changed_t` | `ideatek_net_authority_changed_t_desc` | `IDEATEK_NET_AUTHORITY_CHANGED_FIELDS` |
| `ideatek:NetHostChanged` | `ideatek_net_host_changed_t` | `ideatek_net_host_changed_t_desc` | `IDEATEK_NET_HOST_CHANGED_FIELDS` |

```text
{ IDEATEK_NET_ROLE_SERVER = 0, IDEATEK_NET_ROLE_CLIENT = 1, IDEATEK_NET_ROLE_HOST = 2 }
{ ideatek_entity_t connection; ideatek_entity_t listen; uint32_t protocol; ideatek_net_role_t role; }
{ IDEATEK_PEER_STATE_JOINING = 0, IDEATEK_PEER_STATE_READY = 1, IDEATEK_PEER_STATE_REFUSED = 2, IDEATEK_PEER_STATE_LEFT = 3 }
{ ideatek_entity_t session; ideatek_entity_t connection; uint32_t id; ideatek_peer_state_t state; }
{ bridge_text_t data; }
{ ideatek_entity_t peer; bridge_text_t data; uint32_t order; }
{ IDEATEK_ADMIT_ACCEPT = 0, IDEATEK_ADMIT_REFUSE = 1 }
{ int32_t reason; ideatek_admit_t action; }
{ IDEATEK_NET_AUTHORITY_SERVER = 0, IDEATEK_NET_AUTHORITY_OWNER = 1, IDEATEK_NET_AUTHORITY_HOST = 2, IDEATEK_NET_AUTHORITY_DETERMINISTIC = 3 }
{ uint64_t prefab; uint64_t group; uint32_t owner; uint32_t host; ideatek_net_authority_t authority; }
{ uint64_t net_id; }
{ IDEATEK_NET_KIND_STATE = 0, IDEATEK_NET_KIND_EVENT = 1, IDEATEK_NET_KIND_PREDICTED = 2 }
{ IDEATEK_NET_CONDITION_ALL = 0, IDEATEK_NET_CONDITION_OWNER_ONLY = 1, IDEATEK_NET_CONDITION_SKIP_OWNER = 2, IDEATEK_NET_CONDITION_INITIAL_ONLY = 3 }
{ uint64_t tag; bridge_text_t data; uint32_t peer; ideatek_net_kind_t kind; ideatek_net_condition_t condition; }
{ uint64_t tag; bridge_text_t data; uint32_t peer; ideatek_net_kind_t kind; }
{ uint32_t peer; }
{ IDEATEK_RELEVANCE_MODE_DISTANCE = 0, IDEATEK_RELEVANCE_MODE_ALWAYS = 1, IDEATEK_RELEVANCE_MODE_OWNER = 2, IDEATEK_RELEVANCE_MODE_NONE = 3, IDEATEK_RELEVANCE_MODE_GROUP = 4, IDEATEK_RELEVANCE_MODE_VISIBLE = 5 }
{ uint64_t groups; float distance; float priority; uint32_t steps; ideatek_relevance_mode_t mode; }
{ bridge_text_t data; uint32_t tick; uint32_t view_tick; }
{ bridge_text_t data; uint32_t peer; uint32_t tick; uint32_t view_tick; }
{ uint32_t tick; uint32_t command_tick; uint32_t playback; uint32_t ack; uint32_t confirmed; uint32_t host; }
{ uint64_t groups; }
{ uint64_t key; }
{ uint64_t group; uint32_t tick; uint32_t order; }
{ ideatek_entity_t entity; uint32_t old_host; uint32_t new_host; uint32_t order; ideatek_net_authority_t old_authority; ideatek_net_authority_t new_authority; }
{ uint32_t old_host; uint32_t new_host; uint32_t order; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_net_role_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_peer_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_admit_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_net_authority_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_net_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_net_condition_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_relevance_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetSession` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetPeer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetJoin` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetJoinEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetAdmit` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Replicated` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetEntity` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetOut` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetIn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetViewer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetRelevance` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetCommand` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetCommandIn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetSessionState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetPeerGroups` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetPredictedSpawn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetRollback` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetAuthorityChanged` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NetHostChanged` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The net session, the join, the answers, the replicated entities, the bytes out, the viewers, the relevance and the commands | The types of the fields |
| The net module | `ideatek:NetPeer`, `ideatek:NetJoinEvent`, `ideatek:NetEntity`, `ideatek:NetIn`, `ideatek:NetCommandIn`, `ideatek:NetSessionState`, `ideatek:NetRollback`, `ideatek:NetAuthorityChanged`, `ideatek:NetHostChanged` | `ideatek:Tick`, what the game writes, and the field types |

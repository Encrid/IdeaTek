# include/platform_abstraction/ideatek_sockets.h

`include/platform_abstraction/ideatek_sockets.h` gives the types of the sockets part: the identity, the connections over IP and P2P, the lanes, the sessions, the signals and the messages.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the world part, for `ideatek_entity_t`.
The sockets module uses the open-source GameNetworkingSockets, so the words of this document are the words of GameNetworkingSockets.

## The roles

- The game writes the requests, `ideatek:MessagesOut`, `ideatek:SessionMessagesOut` and `ideatek:SignalsIn`.
- The sockets module writes the states, `ideatek:Inbound`, `ideatek:Session`, the batches in and `ideatek:SignalsOut`. A deployment has one sockets module.
- Each of them reads the types that the other writes.

Out: the parts of GameNetworkingSockets that need Steam. These are the relays of Steam, FakeIP, the hosted servers and the request of a certificate from Steam.

## The sockets entity

At `init`, the sockets module spawns 1 entity, the sockets entity.
It attaches `ideatek:IdentityState`, `ideatek:MessagesIn`, `ideatek:SessionMessagesIn` and `ideatek:SignalsOut` to it.
At `init`, the game spawns 1 entity, and attaches `ideatek:MessagesOut`, `ideatek:SessionMessagesOut` and `ideatek:SignalsIn` to it.
The game takes the blocks of its batches at `init`, in its share, and uses them again in each round.

## The identity

An identity names a program on the network, in the text form of GameNetworkingSockets. Examples are `str:player-1`, `gen:00ff` and `ip:192.168.0.1:27015`.
The game attaches `ideatek:IdentityRequest` to the sockets entity.
The sockets module then stops GameNetworkingSockets and starts it again with the identity. This closes each connection and each listen socket.

Before the first request, the identity is `ip:::1`, the local host. P2P and the sessions need an identity that is not an IP address.

With a `certificate` of 0 bytes, each connection makes its own certificate. Then the connection is encrypted, but the peer is not authenticated.
With a certificate, the peer can check the identity. The certificate comes from the server of the game, which signs `certificate_request`.
The sockets module trusts only the key of the certificate authority that its build holds. GameNetworkingSockets sets the key at compile time.

## The security

Without a certificate, a peer can claim any identity. `IDEATEK_CONNECTION_FLAG_UNAUTHENTICATED` of `flags` then tells it.
An identity of the kind `ip:` is never authenticated.
Do not send passwords or payment data on a connection that is not authenticated.

## The addresses

An address is an IPv6 address and a port. An IPv4 address is in the IPv6 form `::ffff:a.b.c.d`.
The bytes of an address are in the order of the network. The port is in the order of the machine.

The game spawns an entity and attaches `ideatek:AddressRequest` to it, with a name in UTF-8 in its share.
In a later round, the sockets module attaches `ideatek:AddressResult` to the same entity.
The OS finds the addresses of the name.
In the next round, the sockets module destroys the entity.

## The listen sockets

The game spawns an entity and attaches `ideatek:ListenRequest` to it.
With `IDEATEK_LISTEN_KIND_IP`, the sockets module opens a listen socket on `port`. With `IDEATEK_LISTEN_KIND_P2P`, it listens on `virtual_port` for P2P.
It attaches `ideatek:ListenState` to the same entity.
To close the listen socket, the game destroys the entity. The sockets module observes `BRIDGE_ON_LEAVE` of `ideatek:ListenRequest`.

For each connection that comes to a listen socket, the sockets module spawns 1 entity.
It attaches `ideatek:Inbound` and `ideatek:ConnectionState` to the entity.
The game answers with `ideatek:ConnectionRequest` on the entity: `IDEATEK_CONNECTION_ACTION_ACCEPT` or `IDEATEK_CONNECTION_ACTION_CLOSE`.

## The connections

The game spawns an entity and attaches `ideatek:ConnectionRequest` to it.
`IDEATEK_CONNECTION_ACTION_CONNECT` connects to `address`. `IDEATEK_CONNECTION_ACTION_CONNECT_P2P` connects to `remote_identity` on `virtual_port`, through the signals.
`IDEATEK_CONNECTION_ACTION_PAIR` makes 2 connections in the same program, with no network. A host uses a pair to play on its own server.
For a pair, the sockets module spawns a second entity, and attaches `ideatek:Inbound` with `listen` at 0 to it.

The sockets module attaches `ideatek:ConnectionState` to each new connection entity in the same round.
In each round, it attaches `ideatek:ConnectionState` again when a byte of it changed.

With `symmetric` at `true`, 2 peers can connect to each other at the same time, and get 1 connection.
To end a connection, the game attaches `ideatek:ConnectionRequest` again, with `IDEATEK_CONNECTION_ACTION_CLOSE`.
The game does this also after `IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER` and after `IDEATEK_CONNECTION_STATUS_PROBLEM`.
One round after the close, the sockets module destroys the entity.
After `IDEATEK_CONNECTION_STATUS_PROBLEM`, the sockets module writes the detailed status of GameNetworkingSockets in its log.

## The lanes

A connection has 1 lane at first. A lane is a queue of messages with its own order.
A lost message of 1 lane does not stop the messages of another lane.
To set the lanes, the game attaches `ideatek:LanesRequest` to the connection entity. Its batch has 1 record for each lane.

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `priority` | `int32_t` | 0 | The lanes with the lowest priority send first. |
| `weight` | `uint16_t` | 4 | Among lanes of the same priority, the part of the bandwidth. 1 or more. |

A connection has 1 to 255 lanes, and the number of lanes does not go down.
With more than 1 lane, the sockets module attaches `ideatek:LaneStates` to the connection entity in each round. Its batch has 1 record for each lane.

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `queue_time` | `int64_t` | 0 | The microseconds that a new message of the lane waits before it goes. |
| `pending_unreliable` | `int32_t` | 8 | The bytes of unreliable messages in the queue of the lane. |
| `pending_reliable` | `int32_t` | 12 | The bytes of reliable messages in the queue of the lane. |
| `sent_unacked_reliable` | `int32_t` | 16 | The bytes of reliable messages that went, with no acknowledgement yet. |

## The messages

In each round that sends messages, the game attaches `ideatek:MessagesOut` again. The sockets module sends each batch once.
In each round that gets messages, the sockets module attaches `ideatek:MessagesIn` again.
A reader observes `BRIDGE_ON_ATTACH` of `ideatek:MessagesIn`.

A batch is a sequence of records, with no bytes between them. Its records are for all the connections.
The fields of a record have no alignment, so a reader copies each field with `memcpy`.
The numbers are in the order of the machine. A message has at most 524288 bytes, the limit of GameNetworkingSockets.

A record of `ideatek:MessagesOut`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `connection` | `uint64_t` | 0 | The `id` of the entity of the connection. |
| `length` | `uint32_t` | 8 | The number of bytes of the message. |
| `lane` | `uint16_t` | 12 | The lane, from 0. |
| `flags` | `ideatek_send_t` | 14 | How to send the message. |
| The bytes | `uint8_t[length]` | 15 | The message. |

A record of `ideatek:MessagesIn`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `connection` | `uint64_t` | 0 | The `id` of the entity of the connection. |
| `number` | `int64_t` | 8 | The number of the message in its lane. The sender gives it, from 1. |
| `length` | `uint32_t` | 16 | The number of bytes of the message. |
| `lane` | `uint16_t` | 20 | The lane, from 0. |
| `flags` | `ideatek_send_t` | 22 | Only `IDEATEK_SEND_RELIABLE` is valid. |
| The bytes | `uint8_t[length]` | 23 | The message. |

If GameNetworkingSockets refuses a message, the sockets module adds 1 to `failed_sends` of the connection.

## The sessions

A session sends messages to an identity with no request for a connection. GameNetworkingSockets opens a symmetric P2P connection under it, through the signals.
A session is an entity with `ideatek:Session` and `ideatek:ConnectionState`.
The game spawns an entity and attaches `ideatek:SessionRequest` with `IDEATEK_SESSION_ACTION_OPEN` to it.
When a peer opens a session, the sockets module spawns an entity with `ideatek:Session`. The game answers with `IDEATEK_SESSION_ACTION_ACCEPT` or `IDEATEK_SESSION_ACTION_CLOSE`.

`IDEATEK_SESSION_ACTION_CLOSE_CHANNEL` closes 1 channel. The last channel closes the session.
A session with no messages for 3 minutes closes. The sockets module then destroys its entity in the next round.

The records of `ideatek:SessionMessagesOut` and `ideatek:SessionMessagesIn`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `session` | `uint64_t` | 0 | The `id` of the entity of the session. |
| `channel` | `int32_t` | 8 | The channel of the message. |
| `length` | `uint32_t` | 12 | The number of bytes of the message. |
| `flags` | `ideatek_send_t` | 16 | How to send the message. In `ideatek:SessionMessagesIn`, only `IDEATEK_SEND_RELIABLE` is valid. |
| The bytes | `uint8_t[length]` | 17 | The message. |

`IDEATEK_SEND_AUTO_RESTART` opens the session again if it broke.

## The signals

A P2P connection starts with signals: small messages through a channel of the game, before the peers find a route.
The game carries the signals, for example through its own server or a platform module.
A channel of signals can lose, repeat or swap signals.

In each round with signals, the sockets module attaches `ideatek:SignalsOut` again. A record of `ideatek:SignalsOut`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `connection` | `uint64_t` | 0 | The `id` of the entity of the connection. 0 for a rejection. |
| `peer` | `char[128]` | 8 | The identity of the peer that gets the signal, with a NUL after its last byte. |
| `length` | `uint32_t` | 136 | The number of bytes of the signal. |
| The bytes | `uint8_t[length]` | 140 | The signal. |

The game sends each signal to its peer. The peer game writes it into its `ideatek:SignalsIn`. A record of `ideatek:SignalsIn`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `length` | `uint32_t` | 0 | The number of bytes of the signal. |
| The bytes | `uint8_t[length]` | 4 | The signal. |

A signal that comes for no listen socket of its virtual port gets a rejection.

## The bytes of the batches in

At `init`, the sockets module takes `message_bytes` bytes of its persistent arena.
Each round writes its batches in, its lane states, its signals and its addresses into the half that the round before did not use.
So the bytes of a batch in stay good for 1 round.
A message that finds no room in that half stays in the queue of GameNetworkingSockets until the next round.
The bytes of `certificate_request` are in a separate part of the arena, of 512 bytes. They stay good until the next `ideatek:IdentityState`.

The `persistent` of its manifest includes `message_bytes` and 512.

## The network tests

The settings that start with `fake_` make the network worse on purpose, to test the game. Each one is 0 and off by default.
They act on all the connections of the program. The delay comes first, then the jitter, then the change of the order.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `message_bytes` | 2097152 | 1048576 to 1073741824, an even number | The bytes for the batches in. Each round uses at most the half. |
| `timeout_initial` | 10000 | 1 to 2147483647 | The milliseconds that a new connection waits for the peer. |
| `timeout_connected` | 10000 | 1 to 2147483647 | The milliseconds that a connection waits for a silent peer, before `IDEATEK_CONNECTION_STATUS_PROBLEM`. |
| `send_buffer` | 524288 | 4096 to 268435456 | The bytes that wait to go on 1 connection. A message that does not fit is a failed send. |
| `recv_buffer` | 1048576 | 4096 to 268435456, more than `recv_max_message` | The bytes that wait for the game on 1 connection. |
| `recv_buffer_messages` | 1000 | 2 to 268435456 | The messages that wait for the game on 1 connection. |
| `recv_max_message` | 524288 | 64 to 268435456 | The largest message that comes in. A larger message closes the connection. |
| `nagle_time` | 5000 | 0 to 20000 | The microseconds that a message waits for more messages, without `IDEATEK_SEND_NO_NAGLE`. |
| `mtu` | 1300 | 200 to 1300 | The largest UDP payload of a packet, in bytes. |
| `allow_without_auth` | `"allow"` | `"refuse"`, `"warn"` or `"allow"` | What a connection over IP does with a peer that is not authenticated. |
| `allow_localhost_without_auth` | `"allow"` | `"refuse"`, `"warn"` or `"allow"` | The same, for a peer on the local host. The more open of the two applies. |
| `unencrypted` | `"never"` | `"never"`, `"allowed"`, `"preferred"` or `"required"` | A connection with no encryption. The peer must allow it too. |
| `stun_servers` | `[]` | a list of texts | The STUN servers that P2P uses to find the public address, as `host:port`. |
| `turn_servers` | `[]` | a list of tables with `url`, `user` and `password` | The TURN servers that carry P2P when no direct route works. |
| `ice_transports` | `["relay", "private", "public"]` | a list of `"relay"`, `"private"` and `"public"` | The kinds of route that P2P tries: TURN, the local network, the public addresses. |
| `ice_penalty` | 0 | 0 to 2147483647 | The milliseconds that P2P adds to the score of a route through ICE. |
| `ice_implementation` | `"auto"` | `"auto"`, `"native"` or `"webrtc"` | The ICE client. `"auto"` takes WebRTC if the build has it, else the native client. |
| `fake_loss_send` | 0 | 0 to 100 | The percent of the packets that go out that the sockets module drops. |
| `fake_loss_recv` | 0 | 0 to 100 | The percent of the packets that come in that the sockets module drops. |
| `fake_lag_send` | 0 | 0 to 5000 | The milliseconds of delay of each packet that goes out. |
| `fake_lag_recv` | 0 | 0 to 5000 | The milliseconds of delay of each packet that comes in. |
| `fake_reorder_send` | 0 | 0 to 100 | The percent of the packets that go out that come later than the next ones. |
| `fake_reorder_recv` | 0 | 0 to 100 | The percent of the packets that come in that come later than the next ones. |
| `fake_reorder_time` | 15 | 0 to 5000 | The milliseconds that a packet of a change of order waits. |
| `fake_duplicate_send` | 0 | 0 to 100 | The percent of the packets that go out 2 times. |
| `fake_duplicate_recv` | 0 | 0 to 100 | The percent of the packets that come in 2 times. |
| `fake_duplicate_time_max` | 10 | 0 to 5000 | The most milliseconds between a packet and its copy. |
| `fake_jitter_send_avg` | 0 | 0 to 2000 | The mean milliseconds of random delay of a packet that goes out. |
| `fake_jitter_recv_avg` | 0 | 0 to 2000 | The mean milliseconds of random delay of a packet that comes in. |
| `fake_jitter_send_max` | 100 | 0 to 5000 | The most milliseconds of random delay of a packet that goes out. |
| `fake_jitter_recv_max` | 100 | 0 to 5000 | The most milliseconds of random delay of a packet that comes in. |
| `fake_jitter_send_pct` | 75 | 0 to 100 | The percent of the packets that go out that get a random delay. |
| `fake_jitter_recv_pct` | 75 | 0 to 100 | The percent of the packets that come in that get a random delay. |
| `fake_rate_send` | 0 | 0 to 1073741824 | The bytes per second that go out. 0 is no limit. |
| `fake_rate_recv` | 0 | 0 to 1073741824 | The bytes per second that come in. 0 is no limit. |
| `fake_burst_send` | 16384 | 0 to 1048576 | The bytes that go out above `fake_rate_send` in a burst. |
| `fake_burst_recv` | 16384 | 0 to 1048576 | The bytes that come in above `fake_rate_recv` in a burst. |

## ideatek:ideatek_address_t

`ideatek_address_t` is an address and a port: *The addresses*.
`ideatek_address_t` takes 18 bytes. Its alignment is 2.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `uint8_t[16]` | 0 | The IPv6 address. |
| `port` | `uint16_t` | 16 | The port. |

## ideatek:ideatek_identity_t

`ideatek_identity_t` is an identity: *The identity*.
`ideatek_identity_t` takes 128 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `text` | `char[128]` | 0 | The identity as UTF-8 text that ends with a NUL. |

128 bytes is `k_cchMaxString` of GameNetworkingSockets, the size of the text form of an identity.

## ideatek:ideatek_send_t

`ideatek_send_t` tells how to send a message. With no flag, the message can be lost, and it can come out of order.
`ideatek_send_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SEND_RELIABLE` | Send the message again until the peer gets it, in order. |
| 0x02 | `IDEATEK_SEND_NO_NAGLE` | Send the message without a wait for more messages. |
| 0x04 | `IDEATEK_SEND_NO_DELAY` | Drop the message if it cannot go now. Only for an unreliable message. |
| 0x08 | `IDEATEK_SEND_AUTO_RESTART` | Open a broken session again. Only for a session. |

## ideatek:ideatek_connection_flag_t

`ideatek_connection_flag_t` tells facts of a connection. The values are the values of GameNetworkingSockets.
`ideatek_connection_flag_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_CONNECTION_FLAG_UNAUTHENTICATED` | The identity of the peer is not checked. |
| 0x02 | `IDEATEK_CONNECTION_FLAG_UNENCRYPTED` | The connection has no encryption. |
| 0x04 | `IDEATEK_CONNECTION_FLAG_LOOPBACK` | The connection is a pair in the same program. |
| 0x08 | `IDEATEK_CONNECTION_FLAG_FAST` | The connection is on a fast local network. |
| 0x10 | `IDEATEK_CONNECTION_FLAG_RELAYED` | The connection goes through a TURN server. |

## ideatek:ideatek_listen_kind_t

`ideatek_listen_kind_t` tells the kind of a listen socket.
`ideatek_listen_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LISTEN_KIND_IP` | A listen socket on a port of UDP. |
| 1 | `IDEATEK_LISTEN_KIND_P2P` | A listen socket on a virtual port, for P2P. |

## ideatek:ideatek_listen_status_t

`ideatek_listen_status_t` tells the status of a listen socket.
`ideatek_listen_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LISTEN_STATUS_LISTENING` | The listen socket waits for connections. |
| 1 | `IDEATEK_LISTEN_STATUS_FAILED` | GameNetworkingSockets did not open the listen socket. |

## ideatek:ideatek_connection_action_t

`ideatek_connection_action_t` tells the action of a connection request.
`ideatek_connection_action_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CONNECTION_ACTION_CONNECT` | Open a connection to the address. |
| 1 | `IDEATEK_CONNECTION_ACTION_CONNECT_P2P` | Open a P2P connection to the identity. |
| 2 | `IDEATEK_CONNECTION_ACTION_ACCEPT` | Accept a connection that came to a listen socket. |
| 3 | `IDEATEK_CONNECTION_ACTION_CLOSE` | End the connection. |
| 4 | `IDEATEK_CONNECTION_ACTION_PAIR` | Make a pair of connections in the same program. |

## ideatek:ideatek_connection_status_t

`ideatek_connection_status_t` tells the status of a connection, in the order of GameNetworkingSockets.
`ideatek_connection_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CONNECTION_STATUS_CONNECTING` | The connection is not open yet. |
| 1 | `IDEATEK_CONNECTION_STATUS_FINDING_ROUTE` | A P2P connection looks for a route. |
| 2 | `IDEATEK_CONNECTION_STATUS_CONNECTED` | The connection is open. |
| 3 | `IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER` | The peer ended the connection. |
| 4 | `IDEATEK_CONNECTION_STATUS_PROBLEM` | The connection ended because of a problem here or on the network. |

## ideatek:ideatek_auth_status_t

`ideatek_auth_status_t` tells the status of the certificate.
`ideatek_auth_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_AUTH_STATUS_NONE` | No certificate: the connections are not authenticated. |
| 1 | `IDEATEK_AUTH_STATUS_CURRENT` | The certificate is good. |
| 2 | `IDEATEK_AUTH_STATUS_FAILED` | GameNetworkingSockets refused the certificate. |

## ideatek:ideatek_session_action_t

`ideatek_session_action_t` tells the action of a session request.
`ideatek_session_action_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SESSION_ACTION_OPEN` | Open a session with the identity. |
| 1 | `IDEATEK_SESSION_ACTION_ACCEPT` | Accept a session that a peer opened. |
| 2 | `IDEATEK_SESSION_ACTION_CLOSE` | Close the session. |
| 3 | `IDEATEK_SESSION_ACTION_CLOSE_CHANNEL` | Close the channel `channel` of the session. |

## ideatek:ideatek_address_status_t

`ideatek_address_status_t` tells the status of a search for addresses.
`ideatek_address_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ADDRESS_STATUS_OK` | The OS found 1 address or more. |
| 1 | `IDEATEK_ADDRESS_STATUS_NOT_FOUND` | The name has no address. |
| 2 | `IDEATEK_ADDRESS_STATUS_FAILED` | The OS did not find the addresses. |

## ideatek:IdentityRequest

`ideatek:IdentityRequest` sets the identity: *The identity*.
`ideatek_identity_request_t` takes 136 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `certificate` | `bridge_text_t` | 0 | The certificate that the server of the game signed. 0 bytes for no certificate. |
| `identity` | `ideatek_identity_t` | 8 | The identity of the program. |

## ideatek:IdentityState

`ideatek:IdentityState` gives the identity that GameNetworkingSockets uses.
`ideatek_identity_state_t` takes 140 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `certificate_request` | `bridge_text_t` | 0 | The request of a certificate, for the server of the game to sign. |
| `identity` | `ideatek_identity_t` | 8 | The identity of the program. |
| `status` | `ideatek_auth_status_t` | 136 | The status of the certificate. |

Bytes 137 to 139 hold no field.

## ideatek:ListenRequest

`ideatek:ListenRequest` asks for a listen socket.
`ideatek_listen_request_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `virtual_port` | `int32_t` | 0 | With `IDEATEK_LISTEN_KIND_P2P`: the virtual port, 0 to 65535. |
| `port` | `uint16_t` | 4 | With `IDEATEK_LISTEN_KIND_IP`: the port. With 0, the OS chooses a free port. |
| `kind` | `ideatek_listen_kind_t` | 6 | The kind of the listen socket. |

Byte 7 holds no field.

## ideatek:ListenState

`ideatek:ListenState` gives the state of a listen socket.
`ideatek_listen_state_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `virtual_port` | `int32_t` | 0 | With `IDEATEK_LISTEN_KIND_P2P`: the virtual port. |
| `port` | `uint16_t` | 4 | With `IDEATEK_LISTEN_KIND_IP`: the port of the listen socket. |
| `kind` | `ideatek_listen_kind_t` | 6 | The kind of the listen socket. |
| `status` | `ideatek_listen_status_t` | 7 | The status. |

## ideatek:ConnectionRequest

`ideatek:ConnectionRequest` asks for an action on a connection.
`ideatek_connection_request_t` takes 168 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `send_rate_min` | `uint32_t` | 0 | 262144 | The lowest bytes per second that the connection sends. |
| `send_rate_max` | `uint32_t` | 4 | 262144 | The highest bytes per second that the connection sends. |
| `reason` | `int32_t` | 8 | No default | With `IDEATEK_CONNECTION_ACTION_CLOSE`: the reason for the peer, 1000 to 2999. |
| `virtual_port` | `int32_t` | 12 | No default | With `IDEATEK_CONNECTION_ACTION_CONNECT_P2P`: the virtual port of the peer. |
| `remote_identity` | `ideatek_identity_t` | 16 | No default | With `IDEATEK_CONNECTION_ACTION_CONNECT_P2P`: the identity of the peer. |
| `address` | `ideatek_address_t` | 144 | No default | With `IDEATEK_CONNECTION_ACTION_CONNECT`: the address of the peer. |
| `action` | `ideatek_connection_action_t` | 162 | No default | The action. |
| `linger` | `bool` | 163 | No default | With `IDEATEK_CONNECTION_ACTION_CLOSE`: `true` sends the reliable messages in the queue before the end. |
| `symmetric` | `bool` | 164 | No default | With a connect: `true` makes 1 connection when 2 peers connect to each other. |

Bytes 165 to 167 hold no field.

A send rate is from 1024 to 268435456, the bounds of GameNetworkingSockets. GameNetworkingSockets adapts the rate between the 2 values.
A symmetric connection cannot linger.

## ideatek:Inbound

`ideatek:Inbound` marks a connection that came to a listen socket.
`ideatek_inbound_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `listen` | `ideatek_entity_t` | 0 | The entity of the listen socket. 0 for the second connection of a pair. |

## ideatek:ConnectionState

`ideatek:ConnectionState` gives the state of a connection or of a session.
`ideatek_connection_state_t` takes 344 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `queue_time` | `int64_t` | 0 | The microseconds that a new message waits in the queue before it goes. |
| `end_reason` | `int32_t` | 8 | After the end: the reason. 1000 to 2999 is a reason of the peer game. Else 0. |
| `ping` | `int32_t` | 12 | The time of a round trip, in milliseconds. |
| `quality_local` | `float` | 16 | The part of the packets of the peer that come here, 0 to 1. -1 when it is not known. |
| `quality_remote` | `float` | 20 | The part of the packets from here that come to the peer, 0 to 1. -1 when it is not known. |
| `in_bytes` | `float` | 24 | The bytes per second that come in. |
| `out_bytes` | `float` | 28 | The bytes per second that go out. |
| `in_packets` | `float` | 32 | The packets per second that come in. |
| `out_packets` | `float` | 36 | The packets per second that go out. |
| `send_rate` | `int32_t` | 40 | The bytes per second that the connection can send now. |
| `max_jitter` | `int32_t` | 44 | The largest jitter since the last state, in microseconds. -1 when it is not known. |
| `pending_reliable` | `int32_t` | 48 | The bytes of reliable messages in the queue. |
| `pending_unreliable` | `int32_t` | 52 | The bytes of unreliable messages in the queue. |
| `sent_unacked_reliable` | `int32_t` | 56 | The bytes of reliable messages that went, with no acknowledgement yet. |
| `failed_sends` | `uint32_t` | 60 | The messages that GameNetworkingSockets refused in the last round. |
| `remote_identity` | `ideatek_identity_t` | 64 | The identity of the peer. |
| `end_text` | `char[128]` | 192 | After the end: the text of the reason, with a NUL after its last byte. Else an empty text. |
| `remote` | `ideatek_address_t` | 320 | The address of the peer. |
| `status` | `ideatek_connection_status_t` | 338 | The status. |
| `flags` | `ideatek_connection_flag_t` | 339 | The facts of the connection. |

Bytes 340 to 343 hold no field.

128 bytes of `end_text` is the limit of GameNetworkingSockets for the text of a reason.

## ideatek:LanesRequest

`ideatek:LanesRequest` sets the lanes of a connection: *The lanes*. Its C type `ideatek_batch_t` holds a batch of records.
`ideatek_batch_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | The records of the batch. |

## ideatek:LaneStates

`ideatek:LaneStates` gives the state of each lane of a connection: *The lanes*. Its C type is `ideatek_batch_t`.

## ideatek:Session

`ideatek:Session` marks a session: *The sessions*.
`ideatek_session_t` takes 128 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `remote` | `ideatek_identity_t` | 0 | The identity of the peer. |

## ideatek:SessionRequest

`ideatek:SessionRequest` asks for an action on a session.
`ideatek_session_request_t` takes 136 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `channel` | `int32_t` | 0 | With `IDEATEK_SESSION_ACTION_CLOSE_CHANNEL`: the channel. |
| `remote` | `ideatek_identity_t` | 4 | With `IDEATEK_SESSION_ACTION_OPEN`: the identity of the peer. |
| `action` | `ideatek_session_action_t` | 132 | The action. |

Bytes 133 to 135 hold no field.

## ideatek:MessagesOut

`ideatek:MessagesOut` holds the messages that the game sends: *The messages*. Its C type is `ideatek_batch_t`.

## ideatek:MessagesIn

`ideatek:MessagesIn` holds the messages that came in: *The messages*. Its C type is `ideatek_batch_t`.

## ideatek:SessionMessagesOut

`ideatek:SessionMessagesOut` holds the messages that the game sends to sessions: *The sessions*. Its C type is `ideatek_batch_t`.

## ideatek:SessionMessagesIn

`ideatek:SessionMessagesIn` holds the messages of sessions that came in: *The sessions*. Its C type is `ideatek_batch_t`.

## ideatek:SignalsOut

`ideatek:SignalsOut` holds the signals that the game carries to the peers: *The signals*. Its C type is `ideatek_batch_t`.

## ideatek:SignalsIn

`ideatek:SignalsIn` holds the signals that came from the peers: *The signals*. Its C type is `ideatek_batch_t`.

## ideatek:AddressRequest

`ideatek:AddressRequest` asks for the addresses of a name.
`ideatek_address_request_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | The name, for example `example.com`. |

## ideatek:AddressResult

`ideatek:AddressResult` gives the addresses of a name.
`ideatek_address_result_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `addresses` | `bridge_text_t` | 0 | The addresses, 18 bytes each in the form of `ideatek_address_t`, with the port 0. |
| `status` | `ideatek_address_status_t` | 8 | The status. |

Bytes 9 to 11 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_address_t` | `ideatek_address_t` | `ideatek_address_t_desc` | `IDEATEK_ADDRESS_FIELDS` |
| `ideatek:ideatek_identity_t` | `ideatek_identity_t` | `ideatek_identity_t_desc` | `IDEATEK_IDENTITY_FIELDS` |
| `ideatek:ideatek_send_t` | `ideatek_send_t` | `ideatek_send_t_desc` | `IDEATEK_SEND_VALUES` |
| `ideatek:ideatek_connection_flag_t` | `ideatek_connection_flag_t` | `ideatek_connection_flag_t_desc` | `IDEATEK_CONNECTION_FLAG_VALUES` |
| `ideatek:ideatek_listen_kind_t` | `ideatek_listen_kind_t` | `ideatek_listen_kind_t_desc` | `IDEATEK_LISTEN_KIND_VALUES` |
| `ideatek:ideatek_listen_status_t` | `ideatek_listen_status_t` | `ideatek_listen_status_t_desc` | `IDEATEK_LISTEN_STATUS_VALUES` |
| `ideatek:ideatek_connection_action_t` | `ideatek_connection_action_t` | `ideatek_connection_action_t_desc` | `IDEATEK_CONNECTION_ACTION_VALUES` |
| `ideatek:ideatek_connection_status_t` | `ideatek_connection_status_t` | `ideatek_connection_status_t_desc` | `IDEATEK_CONNECTION_STATUS_VALUES` |
| `ideatek:ideatek_auth_status_t` | `ideatek_auth_status_t` | `ideatek_auth_status_t_desc` | `IDEATEK_AUTH_STATUS_VALUES` |
| `ideatek:ideatek_session_action_t` | `ideatek_session_action_t` | `ideatek_session_action_t_desc` | `IDEATEK_SESSION_ACTION_VALUES` |
| `ideatek:ideatek_address_status_t` | `ideatek_address_status_t` | `ideatek_address_status_t_desc` | `IDEATEK_ADDRESS_STATUS_VALUES` |
| `ideatek:IdentityRequest` | `ideatek_identity_request_t` | `ideatek_identity_request_t_desc` | `IDEATEK_IDENTITY_REQUEST_FIELDS` |
| `ideatek:IdentityState` | `ideatek_identity_state_t` | `ideatek_identity_state_t_desc` | `IDEATEK_IDENTITY_STATE_FIELDS` |
| `ideatek:ListenRequest` | `ideatek_listen_request_t` | `ideatek_listen_request_t_desc` | `IDEATEK_LISTEN_REQUEST_FIELDS` |
| `ideatek:ListenState` | `ideatek_listen_state_t` | `ideatek_listen_state_t_desc` | `IDEATEK_LISTEN_STATE_FIELDS` |
| `ideatek:ConnectionRequest` | `ideatek_connection_request_t` | `ideatek_connection_request_t_desc` | `IDEATEK_CONNECTION_REQUEST_FIELDS` |
| `ideatek:Inbound` | `ideatek_inbound_t` | `ideatek_inbound_t_desc` | `IDEATEK_INBOUND_FIELDS` |
| `ideatek:ConnectionState` | `ideatek_connection_state_t` | `ideatek_connection_state_t_desc` | `IDEATEK_CONNECTION_STATE_FIELDS` |
| `ideatek:LanesRequest` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:LaneStates` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:Session` | `ideatek_session_t` | `ideatek_session_t_desc` | `IDEATEK_SESSION_FIELDS` |
| `ideatek:SessionRequest` | `ideatek_session_request_t` | `ideatek_session_request_t_desc` | `IDEATEK_SESSION_REQUEST_FIELDS` |
| `ideatek:MessagesOut` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:MessagesIn` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:SessionMessagesOut` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:SessionMessagesIn` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:SignalsOut` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:SignalsIn` | `ideatek_batch_t` | `ideatek_batch_t_desc` | `IDEATEK_BATCH_FIELDS` |
| `ideatek:AddressRequest` | `ideatek_address_request_t` | `ideatek_address_request_t_desc` | `IDEATEK_ADDRESS_REQUEST_FIELDS` |
| `ideatek:AddressResult` | `ideatek_address_result_t` | `ideatek_address_result_t_desc` | `IDEATEK_ADDRESS_RESULT_FIELDS` |

```text
{ uint8_t bytes[16]; uint16_t port; }
{ char text[128]; }
{ IDEATEK_SEND_RELIABLE = 0x01, IDEATEK_SEND_NO_NAGLE = 0x02, IDEATEK_SEND_NO_DELAY = 0x04, IDEATEK_SEND_AUTO_RESTART = 0x08 }
{ IDEATEK_CONNECTION_FLAG_UNAUTHENTICATED = 0x01, IDEATEK_CONNECTION_FLAG_UNENCRYPTED = 0x02, IDEATEK_CONNECTION_FLAG_LOOPBACK = 0x04, IDEATEK_CONNECTION_FLAG_FAST = 0x08, IDEATEK_CONNECTION_FLAG_RELAYED = 0x10 }
{ IDEATEK_LISTEN_KIND_IP = 0, IDEATEK_LISTEN_KIND_P2P = 1 }
{ IDEATEK_LISTEN_STATUS_LISTENING = 0, IDEATEK_LISTEN_STATUS_FAILED = 1 }
{ IDEATEK_CONNECTION_ACTION_CONNECT = 0, IDEATEK_CONNECTION_ACTION_CONNECT_P2P = 1, IDEATEK_CONNECTION_ACTION_ACCEPT = 2, IDEATEK_CONNECTION_ACTION_CLOSE = 3, IDEATEK_CONNECTION_ACTION_PAIR = 4 }
{ IDEATEK_CONNECTION_STATUS_CONNECTING = 0, IDEATEK_CONNECTION_STATUS_FINDING_ROUTE = 1, IDEATEK_CONNECTION_STATUS_CONNECTED = 2, IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER = 3, IDEATEK_CONNECTION_STATUS_PROBLEM = 4 }
{ IDEATEK_AUTH_STATUS_NONE = 0, IDEATEK_AUTH_STATUS_CURRENT = 1, IDEATEK_AUTH_STATUS_FAILED = 2 }
{ IDEATEK_SESSION_ACTION_OPEN = 0, IDEATEK_SESSION_ACTION_ACCEPT = 1, IDEATEK_SESSION_ACTION_CLOSE = 2, IDEATEK_SESSION_ACTION_CLOSE_CHANNEL = 3 }
{ IDEATEK_ADDRESS_STATUS_OK = 0, IDEATEK_ADDRESS_STATUS_NOT_FOUND = 1, IDEATEK_ADDRESS_STATUS_FAILED = 2 }
{ bridge_text_t certificate; ideatek_identity_t identity; }
{ bridge_text_t certificate_request; ideatek_identity_t identity; ideatek_auth_status_t status; }
{ int32_t virtual_port; uint16_t port; ideatek_listen_kind_t kind; }
{ int32_t virtual_port; uint16_t port; ideatek_listen_kind_t kind; ideatek_listen_status_t status; }
{ uint32_t send_rate_min; uint32_t send_rate_max; int32_t reason; int32_t virtual_port; ideatek_identity_t remote_identity; ideatek_address_t address; ideatek_connection_action_t action; bool linger; bool symmetric; }
{ ideatek_entity_t listen; }
{ int64_t queue_time; int32_t end_reason; int32_t ping; float quality_local; float quality_remote; float in_bytes; float out_bytes; float in_packets; float out_packets; int32_t send_rate; int32_t max_jitter; int32_t pending_reliable; int32_t pending_unreliable; int32_t sent_unacked_reliable; uint32_t failed_sends; ideatek_identity_t remote_identity; char end_text[128]; ideatek_address_t remote; ideatek_connection_status_t status; ideatek_connection_flag_t flags; }
{ bridge_text_t data; }
{ ideatek_identity_t remote; }
{ int32_t channel; ideatek_identity_t remote; ideatek_session_action_t action; }
{ bridge_text_t name; }
{ bridge_text_t addresses; ideatek_address_status_t status; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_address_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_identity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_send_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_connection_flag_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_listen_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_listen_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_connection_action_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_connection_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_auth_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_session_action_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_address_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:IdentityRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:IdentityState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ListenRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ListenState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ConnectionRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Inbound` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ConnectionState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LanesRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LaneStates` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Session` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SessionRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MessagesOut` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MessagesIn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SessionMessagesOut` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SessionMessagesIn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SignalsOut` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SignalsIn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AddressRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AddressResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:IdentityRequest`, `ideatek:ListenRequest`, `ideatek:ConnectionRequest`, `ideatek:LanesRequest`, `ideatek:SessionRequest`, `ideatek:MessagesOut`, `ideatek:SessionMessagesOut`, `ideatek:SignalsIn`, `ideatek:AddressRequest` | `ideatek:IdentityState`, `ideatek:ListenState`, `ideatek:Inbound`, `ideatek:ConnectionState`, `ideatek:LaneStates`, `ideatek:Session`, `ideatek:MessagesIn`, `ideatek:SessionMessagesIn`, `ideatek:SignalsOut`, `ideatek:AddressResult`, and the types of the fields |
| The sockets module | `ideatek:IdentityState`, `ideatek:ListenState`, `ideatek:Inbound`, `ideatek:ConnectionState`, `ideatek:LaneStates`, `ideatek:Session`, `ideatek:MessagesIn`, `ideatek:SessionMessagesIn`, `ideatek:SignalsOut`, `ideatek:AddressResult` | `ideatek:IdentityRequest`, `ideatek:ListenRequest`, `ideatek:ConnectionRequest`, `ideatek:LanesRequest`, `ideatek:SessionRequest`, `ideatek:MessagesOut`, `ideatek:SessionMessagesOut`, `ideatek:SignalsIn`, `ideatek:AddressRequest`, and the types of the fields |

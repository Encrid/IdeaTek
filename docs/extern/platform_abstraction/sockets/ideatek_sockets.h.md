# include/platform_abstraction/sockets/ideatek_sockets.h

`include/platform_abstraction/sockets/ideatek_sockets.h` gives the types of the sockets part: the connections over IP, and their messages.
`readme.md`, *The contracts*, tells the rules of each contract.
The sockets module uses GameNetworkingSockets, so the words of this document are the words of GameNetworkingSockets.

## The roles

- The game writes `ideatek:ListenRequest`, `ideatek:ConnectionRequest`, `ideatek:MessagesOut` and `ideatek:AddressRequest`.
- The sockets module writes `ideatek:ListenState`, `ideatek:Inbound`, `ideatek:ConnectionState`, `ideatek:MessagesIn` and `ideatek:AddressResult`. A deployment has one sockets module.
- Each of them reads the types that the other writes.

Out: the discovery on the LAN, peer to peer, relays, certificates, lanes, and messages with no connection.
Comes back: lanes, when a slow stream stops a fast stream; peer to peer, with a web target or a Steam module.
Comes back: the discovery on the LAN, with a Steam module or with a master server.

## The security

The sockets module encrypts each connection, but it does not authenticate the peer.
So a peer can be a different program than the game expects.
Do not send passwords or payment data through the sockets part.

## The addresses

An address is an IPv6 address and a port. An IPv4 address is in the IPv6 form `::ffff:a.b.c.d`.
The bytes of an address are in the order of the network. The port is in the order of the machine.

The game spawns an entity and attaches `ideatek:AddressRequest` to it, with a name in UTF-8 in its share.
In a later round, the sockets module attaches `ideatek:AddressResult` to the same entity.
The OS finds the addresses of the name.
In the next round, the sockets module destroys the entity.

## The listen sockets

The game spawns an entity and attaches `ideatek:ListenRequest` to it.
The sockets module opens a listen socket on the port, and attaches `ideatek:ListenState` to the same entity.
To close the listen socket, the game destroys the entity. The sockets module observes `BRIDGE_ON_LEAVE` of `ideatek:ListenRequest`.

For each connection that comes to a listen socket, the sockets module spawns 1 entity.
It attaches `ideatek:Inbound` and `ideatek:ConnectionState` to the entity.
The game answers with `ideatek:ConnectionRequest` on the entity: `IDEATEK_CONNECTION_ACTION_ACCEPT` or `IDEATEK_CONNECTION_ACTION_CLOSE`.

## The connections

The game spawns an entity and attaches `ideatek:ConnectionRequest` to it, with `IDEATEK_CONNECTION_ACTION_CONNECT`.
The sockets module attaches `ideatek:ConnectionState` to the same entity in the same round.
In each round, the sockets module attaches `ideatek:ConnectionState` again when a byte of it changed.

To end a connection, the game attaches `ideatek:ConnectionRequest` again, with `IDEATEK_CONNECTION_ACTION_CLOSE`.
The game does this also after `IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER` and after `IDEATEK_CONNECTION_STATUS_PROBLEM`.
One round after the close, the sockets module destroys the entity.

## The messages

At `init`, the game spawns 1 entity and attaches `ideatek:MessagesOut` to it.
In each round that sends messages, the game attaches `ideatek:MessagesOut` again. The sockets module sends each batch once.
The game takes the block of its batch at `init`, in its share, and uses it again in each round.

At `init`, the sockets module spawns 1 entity and attaches `ideatek:MessagesIn` to it.
In each round that gets messages, the sockets module attaches `ideatek:MessagesIn` again.
A reader observes `BRIDGE_ON_ATTACH` of `ideatek:MessagesIn`.

A batch is a sequence of records, with no bytes between them. Its records are for all the connections.
The fields of a record have no alignment, so a reader copies each field with `memcpy`.
The numbers are in the order of the machine.

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `connection` | `uint64_t` | 0 | The `id` of the entity of the connection. |
| `length` | `uint32_t` | 8 | The number of bytes of the message. |
| `flags` | `ideatek_send_t` | 12 | Only in `ideatek:MessagesOut`: how to send the message. |
| The bytes | `uint8_t[length]` | 13 out, 12 in | The message. |

A message has at most 524288 bytes, the limit of GameNetworkingSockets.

## The bytes of the batches in

At `init`, the sockets module takes `message_bytes` bytes of its persistent arena.
Each round writes its batch and its addresses into the half that the round before did not use.
So the bytes of `ideatek:MessagesIn` stay good for 1 round.
A message that finds no room in that half stays in the queue of GameNetworkingSockets until the next round.
The `persistent` of its manifest includes `message_bytes`.

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `message_bytes` | 2097152 | 1048576 to 1073741824, an even number | The bytes for the batches in. Each round uses at most the half. |
| `timeout_initial` | 10000 | 1 to 2147483647 | The milliseconds that a new connection waits for the peer. |
| `timeout_connected` | 10000 | 1 to 2147483647 | The milliseconds that a connection waits for a silent peer, before `IDEATEK_CONNECTION_STATUS_PROBLEM`. |

## ideatek:ideatek_address_t

`ideatek_address_t` takes 18 bytes. Its alignment is 2.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `uint8_t[16]` | 0 | The IPv6 address. |
| `port` | `uint16_t` | 16 | The port. |

## ideatek:ideatek_send_t

`ideatek_send_t` is a bitmask of `uint8_t`. Its size and its alignment are 1.
With no flag, the message is unreliable: it can be lost, and it can come out of order.

| Constant | Value | The flag |
| --- | --- | --- |
| `IDEATEK_SEND_NO_NAGLE` | 0x01 | Send the message without a wait for more messages. |
| `IDEATEK_SEND_NO_DELAY` | 0x04 | Drop the message if it cannot go now. |
| `IDEATEK_SEND_RELIABLE` | 0x08 | Send the message again until the peer gets it, in order. |

## ideatek:ListenRequest

`ideatek:ListenRequest` asks for a listen socket.
`ideatek_listen_request_t` takes 2 bytes. Its alignment is 2.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `port` | `uint16_t` | 0 | The port. With 0, the OS chooses a free port. |

## ideatek:ListenState

`ideatek:ListenState` gives the state of a listen socket.
`ideatek_listen_state_t` takes 4 bytes. Its alignment is 2.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `port` | `uint16_t` | 0 | The port of the listen socket. |
| `status` | `ideatek_listen_status_t` | 2 | The status. |

Byte 3 holds no field.

### ideatek:ideatek_listen_status_t

`ideatek_listen_status_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The status |
| --- | --- | --- |
| `IDEATEK_LISTEN_STATUS_LISTENING` | 0 | The listen socket waits for connections. |
| `IDEATEK_LISTEN_STATUS_FAILED` | 1 | The OS did not open the listen socket. |

## ideatek:ConnectionRequest

`ideatek:ConnectionRequest` asks for an action on a connection.
`ideatek_connection_request_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `send_rate` | `uint32_t` | 0 | The bytes per second that the connection sends, 1024 to 268435456. Most games use 262144. |
| `reason` | `int32_t` | 4 | With `IDEATEK_CONNECTION_ACTION_CLOSE`: the reason for the peer, 1000 to 1999. |
| `address` | `ideatek_address_t` | 8 | With `IDEATEK_CONNECTION_ACTION_CONNECT`: the address of the peer. |
| `action` | `ideatek_connection_action_t` | 26 | The action. |
| `linger` | `bool` | 27 | With `IDEATEK_CONNECTION_ACTION_CLOSE`: `true` sends the reliable messages in the queue before the end. |

### ideatek:ideatek_connection_action_t

`ideatek_connection_action_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The action |
| --- | --- | --- |
| `IDEATEK_CONNECTION_ACTION_CONNECT` | 0 | Open a connection to the address. |
| `IDEATEK_CONNECTION_ACTION_ACCEPT` | 1 | Accept a connection that came to a listen socket. |
| `IDEATEK_CONNECTION_ACTION_CLOSE` | 2 | End the connection. |

## ideatek:Inbound

`ideatek:Inbound` marks a connection that came to a listen socket.
`ideatek_inbound_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `listen` | `ideatek_entity_t` | 0 | The entity of the listen socket. |

## ideatek:ConnectionState

`ideatek:ConnectionState` gives the state of a connection.
`ideatek_connection_state_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `queue_time` | `int64_t` | 0 | The microseconds that a new message waits in the queue before it goes. |
| `end_reason` | `int32_t` | 8 | After the end: the reason. 1000 to 1999 is a reason of the peer game. Else 0. |
| `ping` | `int32_t` | 12 | The time of a round trip, in milliseconds. |
| `quality_local` | `float` | 16 | The part of the packets of the peer that come here, 0 to 1. -1 when it is not known. |
| `quality_remote` | `float` | 20 | The part of the packets from here that come to the peer, 0 to 1. -1 when it is not known. |
| `in_bytes` | `float` | 24 | The bytes per second that come in. |
| `out_bytes` | `float` | 28 | The bytes per second that go out. |
| `pending_reliable` | `int32_t` | 32 | The bytes of reliable messages in the queue. |
| `pending_unreliable` | `int32_t` | 36 | The bytes of unreliable messages in the queue. |
| `remote` | `ideatek_address_t` | 40 | The address of the peer. |
| `status` | `ideatek_connection_status_t` | 58 | The status. |

Bytes 59 to 63 hold no field.

### ideatek:ideatek_connection_status_t

`ideatek_connection_status_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The status |
| --- | --- | --- |
| `IDEATEK_CONNECTION_STATUS_CONNECTING` | 0 | The connection is not open yet. |
| `IDEATEK_CONNECTION_STATUS_CONNECTED` | 1 | The connection is open. |
| `IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER` | 2 | The peer ended the connection. |
| `IDEATEK_CONNECTION_STATUS_PROBLEM` | 3 | The connection ended because of a problem here or on the network. |

## ideatek:MessagesOut and ideatek:MessagesIn

`ideatek:MessagesOut` holds the batch that the game sends. `ideatek:MessagesIn` holds the batch that came in.
Both have the C type `ideatek_messages_t`, which takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | The records of the batch: *The messages*. |

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

### ideatek:ideatek_address_status_t

`ideatek_address_status_t` is an enum of `uint8_t`. Its size and its alignment are 1.

| Constant | Value | The status |
| --- | --- | --- |
| `IDEATEK_ADDRESS_STATUS_OK` | 0 | The OS found 1 address or more. |
| `IDEATEK_ADDRESS_STATUS_NOT_FOUND` | 1 | The name has no address. |
| `IDEATEK_ADDRESS_STATUS_FAILED` | 2 | The OS did not find the addresses. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_address_t` | `ideatek_address_t` | `ideatek_address_t_desc` | `IDEATEK_ADDRESS_FIELDS` |
| `ideatek:ideatek_send_t` | `ideatek_send_t` | `ideatek_send_t_desc` | `IDEATEK_SEND_VALUES` |
| `ideatek:ideatek_listen_status_t` | `ideatek_listen_status_t` | `ideatek_listen_status_t_desc` | `IDEATEK_LISTEN_STATUS_VALUES` |
| `ideatek:ideatek_connection_action_t` | `ideatek_connection_action_t` | `ideatek_connection_action_t_desc` | `IDEATEK_CONNECTION_ACTION_VALUES` |
| `ideatek:ideatek_connection_status_t` | `ideatek_connection_status_t` | `ideatek_connection_status_t_desc` | `IDEATEK_CONNECTION_STATUS_VALUES` |
| `ideatek:ideatek_address_status_t` | `ideatek_address_status_t` | `ideatek_address_status_t_desc` | `IDEATEK_ADDRESS_STATUS_VALUES` |
| `ideatek:ListenRequest` | `ideatek_listen_request_t` | `ideatek_listen_request_t_desc` | `IDEATEK_LISTEN_REQUEST_FIELDS` |
| `ideatek:ListenState` | `ideatek_listen_state_t` | `ideatek_listen_state_t_desc` | `IDEATEK_LISTEN_STATE_FIELDS` |
| `ideatek:ConnectionRequest` | `ideatek_connection_request_t` | `ideatek_connection_request_t_desc` | `IDEATEK_CONNECTION_REQUEST_FIELDS` |
| `ideatek:Inbound` | `ideatek_inbound_t` | `ideatek_inbound_t_desc` | `IDEATEK_INBOUND_FIELDS` |
| `ideatek:ConnectionState` | `ideatek_connection_state_t` | `ideatek_connection_state_t_desc` | `IDEATEK_CONNECTION_STATE_FIELDS` |
| `ideatek:MessagesOut` | `ideatek_messages_t` | `ideatek_messages_t_desc` | `IDEATEK_MESSAGES_FIELDS` |
| `ideatek:MessagesIn` | `ideatek_messages_t` | `ideatek_messages_t_desc` | `IDEATEK_MESSAGES_FIELDS` |
| `ideatek:AddressRequest` | `ideatek_address_request_t` | `ideatek_address_request_t_desc` | `IDEATEK_ADDRESS_REQUEST_FIELDS` |
| `ideatek:AddressResult` | `ideatek_address_result_t` | `ideatek_address_result_t_desc` | `IDEATEK_ADDRESS_RESULT_FIELDS` |

```text
{ uint8_t bytes[16]; uint16_t port; }
{ IDEATEK_SEND_NO_NAGLE = 0x01, IDEATEK_SEND_NO_DELAY = 0x04, IDEATEK_SEND_RELIABLE = 0x08 }
{ IDEATEK_LISTEN_STATUS_LISTENING = 0, IDEATEK_LISTEN_STATUS_FAILED = 1 }
{ IDEATEK_CONNECTION_ACTION_CONNECT = 0, IDEATEK_CONNECTION_ACTION_ACCEPT = 1, IDEATEK_CONNECTION_ACTION_CLOSE = 2 }
{ IDEATEK_CONNECTION_STATUS_CONNECTING = 0, IDEATEK_CONNECTION_STATUS_CONNECTED = 1, IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER = 2, IDEATEK_CONNECTION_STATUS_PROBLEM = 3 }
{ IDEATEK_ADDRESS_STATUS_OK = 0, IDEATEK_ADDRESS_STATUS_NOT_FOUND = 1, IDEATEK_ADDRESS_STATUS_FAILED = 2 }
{ uint16_t port; }
{ uint16_t port; ideatek_listen_status_t status; }
{ uint32_t send_rate; int32_t reason; ideatek_address_t address; ideatek_connection_action_t action; bool linger; }
{ ideatek_entity_t listen; }
{ int64_t queue_time; int32_t end_reason; int32_t ping; float quality_local; float quality_remote; float in_bytes; float out_bytes; int32_t pending_reliable; int32_t pending_unreliable; ideatek_address_t remote; ideatek_connection_status_t status; }
{ bridge_text_t data; }
{ bridge_text_t name; }
{ bridge_text_t addresses; ideatek_address_status_t status; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_address_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_send_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_listen_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_connection_action_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_connection_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_address_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ListenRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ListenState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ConnectionRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Inbound` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ConnectionState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MessagesOut` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MessagesIn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AddressRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AddressResult` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares `ideatek:ideatek_entity_t` of the world part and the six field types before the structures.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The four requests and `ideatek:MessagesOut` | `ideatek:ideatek_entity_t`, the six field types and the five results |
| The sockets module | The five results | `ideatek:ideatek_entity_t`, the six field types and the four requests |

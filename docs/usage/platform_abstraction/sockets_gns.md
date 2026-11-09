# BM-IdeaTek-Sockets-GNS

The sockets module of IdeaTek on GameNetworkingSockets. It connects peers over IP and P2P, and carries their messages.

## Install

The release of `BM-IdeaTek-Sockets-GNS` has the tag `sockets_gns-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Sockets-GNS.zip` and `sha256.txt`. `BM-IdeaTek-Sockets-GNS.zip` has a folder for each target.
In `BM-IdeaTek-Sockets-GNS`, each folder holds the module `sockets_gns.dll`, `sockets_gns.so` or `sockets_gns.dylib`, and the 4 notices of GameNetworkingSockets, protobuf, libsodium and mosl.
Each folder of `BM-IdeaTek-Sockets-GNS.zip` also holds this readme and the licence.

The module holds the 4 archives of `BL-IdeaTek-GameNetworkingSockets`, the C++ runtime of zig and mosl for its thread, so nothing stands beside it: `docs/usage/platform_abstraction/gns.md`.
On Windows, the module links `ws2_32`, `crypt32`, `winmm` and `iphlpapi` of the system.
On Linux, the module links with `-z nodelete`: the C++ runtime of GameNetworkingSockets registers handlers for the end of the process, so the module stays in memory after its unload, and the handlers find their code.
The processor must have the AES instructions: `docs/usage/platform_abstraction/gns.md`, *The processor*.
A project names the zip of `BM-IdeaTek-Sockets-GNS` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_sockets_gns:sockets@0.1.0`.
Its role is the sockets module: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The roles*.
The component of its settings is `ideatek_sockets_gns:Settings`. It holds the settings of `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The settings*, and the 5 settings of the module:

| Setting | Type | Default | Range | Meaning |
| --- | --- | --- | --- | --- |
| `max_connections` | `uint32_t` | 64 | 1 to 4096 | The connections and sessions with an entity at one time. A connection that comes above it is closed with the reason 5001 of GameNetworkingSockets and the text `too many connections`. The peer sees the end reason 1999 and the text `Invalid numeric reason code`: GameNetworkingSockets carries only a reason of the application, 1000 to 2999, to the peer. A request above it gets `IDEATEK_CONNECTION_STATUS_PROBLEM`. |
| `max_listen_sockets` | `uint32_t` | 8 | 1 to 256 | The listen sockets at one time. A request above it gets `IDEATEK_LISTEN_STATUS_FAILED`. |
| `max_addresses` | `uint32_t` | 16 | 1 to 1024 | The names that wait for their addresses at one time. A request above it gets `IDEATEK_ADDRESS_STATUS_FAILED`. |
| `session_channels` | `uint32_t` | 16 | 1 to 1024 | The channels of the sessions that the module reads, 0 to `session_channels` less 1. A message on another channel is a failed send. |
| `debug_output` | text | `warning` | `none`, `bug`, `error`, `important`, `warning`, `msg`, `verbose`, `debug` or `everything` | The lines of GameNetworkingSockets that go to the log of the module, from the least to the most. |

The first round of the module reads the settings. Before GameNetworkingSockets starts, each setting of the contract goes to it as the global value of the same meaning.
`ice_implementation` with `auto` takes the native client, because the build holds no WebRTC: `docs/usage/platform_abstraction/gns.md`, *The build*.

### The C face

The module is C. GameNetworkingSockets gives its flat API with `extern "C"` functions, but its headers are C++.
So the module holds its own header of that ABI, `gns_flat.h`. It declares the structs of GameNetworkingSockets that the module uses, with their packing. It declares the functions that the module calls, with a pointer where the flat API has a reference.
The program `gns_layout` of the tests includes both headers and checks each size, alignment and offset, on each target.

### The identity

At `init`, the module starts GameNetworkingSockets with the identity `ip:::1`, and attaches `ideatek:IdentityState` with `IDEATEK_AUTH_STATUS_NONE`.
An `ideatek:IdentityRequest` stops GameNetworkingSockets and starts it again with `identity`.
Each connection then gets `IDEATEK_CONNECTION_STATUS_PROBLEM` with the text `identity changed`, and each listen socket gets `IDEATEK_LISTEN_STATUS_FAILED`. The game closes them as after any problem.

With a `certificate`, the module gives it to GameNetworkingSockets. A certificate that GameNetworkingSockets takes gives `IDEATEK_AUTH_STATUS_CURRENT`. A refused one gives `IDEATEK_AUTH_STATUS_FAILED` and 1 line.
`certificate_request` holds the request of GameNetworkingSockets for the identity, up to 512 bytes.

### The signals

GameNetworkingSockets carries no signal itself. The module gives it a signaling of its own, for the P2P connections and for the sessions.
Each signal that GameNetworkingSockets wants to send becomes a record of `ideatek:SignalsOut`. Each record of `ideatek:SignalsIn` goes to GameNetworkingSockets in the round.
A signal for a virtual port with no listen socket gets a rejection: a record of `ideatek:SignalsOut` with `connection` at 0.

### The addresses

A worker thread of the module asks the OS for the addresses of each name. It takes 1 name at a time, in the order of the requests.
In each round, the system takes what the thread found, and attaches it.
A name with no address gives `IDEATEK_ADDRESS_STATUS_NOT_FOUND`. A name that the OS refuses gives `IDEATEK_ADDRESS_STATUS_FAILED`.

### The round

The system of the module does these steps in each round:

1. Destroys the entities of the connections that closed, and of the names that got a result, in the round before.
2. Writes the lines of GameNetworkingSockets into the log.
3. Runs the callbacks of GameNetworkingSockets: the connections that came, the states that changed, the sessions that a peer opened. A peer that opens a session the game opened to it is accepted at once.
4. Puts the messages that came into the half of the round, then attaches `ideatek:MessagesIn` and `ideatek:SessionMessagesIn`.
5. Attaches `ideatek:SignalsOut` with the signals of GameNetworkingSockets, and `ideatek:LaneStates` for each connection with more than 1 lane.
6. Attaches `ideatek:AddressResult` for each name that the worker thread found.
7. Attaches `ideatek:ConnectionState` to each connection whose state changed by a byte.

The requests and the batches out do not wait for the round. An observer of the module takes each one at the barrier of the phase that attached it, and gives it to GameNetworkingSockets at once.
GameNetworkingSockets runs in its manual poll mode: it has no thread of its own, and does the network when the module polls it. The module polls once a round, and once after each batch of `ideatek:MessagesOut`, `ideatek:SessionMessagesOut` and `ideatek:SignalsIn`. So a batch out goes to the network in its own barrier, and the callbacks of GameNetworkingSockets run on the thread of the phases.

The module flushes a connection after each message that it sends, so `nagle_time` holds no message. Without the flush, a message of the Nagle timer waits for the poll of the next round.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `true` | The module calls GameNetworkingSockets on the thread of the phases, and GameNetworkingSockets has no thread of its own. The worker thread of the addresses is the only other thread of the module. |
| `reloadable` | `false` | Bridge does not store the connections of GameNetworkingSockets. |
| `spawns` | 65 | 1 for the sockets entity. 1 for each connection that comes, and for the second end of a pair, up to `max_connections`. A larger `max_connections` needs a larger `spawns`. |
| `changes` | 65536 | The states of the connections, the batches in and the signals of a round. A round that attaches more fails, with the line of Bridge. |
| `persistent` | 2234112 | `message_bytes` of the contract, plus 512 for `certificate_request`, plus 136448. The 136448 hold the 3 tables, the texts of the settings, and the 2 rings of the signals and the lines. A larger bound needs a larger `persistent`. |
| `scratch` | 16384 | Room for the declarations at the load, and for the lines that the module writes. |
| `log_ring` | 4096 | 128 lines of the log of 32 bytes. GameNetworkingSockets writes many lines above `warning`. |
| `spans` | 65536 | The lists of the 9 observers, and the column base of the 32 types of the module. |

- The phase of the system `round` is `first`, which the deployment maps to the start of the round.
- No pair is needed for the sends: the observers of the module send at the barrier of the phase of the game. The messages that came in a round are in `ideatek:MessagesIn` in the next round.
- The module takes the grants of the sockets module: `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The grants*.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `no sockets: <reason>` | GameNetworkingSockets does not start. The load fails. |
| `no thread` | The system refuses the worker thread of the addresses. The load fails. |
| `gns: <text>` | A line of GameNetworkingSockets at or above `debug_output`. |
| `the identity <text> is refused` | GameNetworkingSockets does not take the identity of a request. The identity stays. |
| `the certificate is refused: <reason>` | GameNetworkingSockets does not take the certificate. The status is `IDEATEK_AUTH_STATUS_FAILED`. |
| `the certificate is taken` | GameNetworkingSockets takes the certificate. The status is `IDEATEK_AUTH_STATUS_CURRENT`. |
| `no state of the connection of <id>` | GameNetworkingSockets gives no info or no status of a connection of the module. The connection keeps its last state. |
| `the request of <id>: <field> is outside <range>` | A field of a request is outside its range, with its value in `<field>`. The request gets its failed status. |
| `more than <max> connections: the connection of <peer> is closed` | A connection comes above `max_connections`. |
| `more than <max> listen sockets: the request of <id> fails` | A listen request comes above `max_listen_sockets`. |
| `more than <max> names: the request of <id> fails` | An address request comes above `max_addresses`. |
| `the message of <id> is refused: <reason>` | GameNetworkingSockets refuses a message, or its connection, lane or channel is wrong. `failed_sends` of the connection grows by 1. |
| `the batch of <id> is malformed at <offset>` | A record of a batch out does not fit in its batch. The rest of the batch is left. |
| `a signal of <count> bytes is refused` | GameNetworkingSockets does not take a signal of `ideatek:SignalsIn`. |
| `a message of <peer> came before its session` | A message of a session came before the game accepted the session. The message waits. |
| `the connection of <id> has a problem: <detailed status>` | A connection gets `IDEATEK_CONNECTION_STATUS_PROBLEM`. The text is the detailed status of GameNetworkingSockets. |
| `<name>: a text of <count> bytes is outside 0 to 1023` | The text setting `<name>` has 1024 bytes or more, or lies outside the arena of the module. The system fails in each round. |

A number outside its range writes the line of `readme.md`, *The contracts*.

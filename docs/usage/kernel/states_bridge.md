# BM-IdeaTek-States-Bridge

The states module of IdeaTek on the queries of Bridge: the requests, the changes and the scoped entities of a machine.

## Install

The release of `BM-IdeaTek-States-Bridge` has the tag `states_bridge-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-States-Bridge.zip` and `sha256.txt`. `BM-IdeaTek-States-Bridge.zip` has a folder for each target.
In `BM-IdeaTek-States-Bridge`, each folder holds the module `states_bridge.dll`, `states_bridge.so` or `states_bridge.dylib`.
Each folder of `BM-IdeaTek-States-Bridge.zip` also holds this readme and the licence.

The module stands on Bridge alone: nothing stands beside it.
A project names the zip of `BM-IdeaTek-States-Bridge` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_states_bridge:states@0.1.0`.
Its role is the states module: `docs/extern/kernel/ideatek_states.h.md`, *The roles*.
The module has no setting, as the contract says. Its manifest states `config` at `none`.

### The systems

The module keeps nothing of its own: it walks the queries that Bridge writes at each barrier.
Its 3 systems run in its phase `first`, in this order:

1. `forget` detaches `ideatek:StateChange` of each state machine that carries it: the change of the round before.
2. `clear` walks each entity with `ideatek:StateScoped`, and destroys the entity that leaves: *The scoped entities*.
3. `apply` walks each entity with `ideatek:StateRequest`. It detaches the request, writes `ideatek:State`, and attaches `ideatek:StateChange`.

The 3 systems change the world at the barrier of the phase. So no phase sees the new value of a state machine next to the scoped entities of its old value.
A request with the current value also counts: the scoped entities of that value go, and `from` equals `to` in `ideatek:StateChange`.

### The scoped entities

An entity with `ideatek:StateScoped` leaves when 1 of these is true of the chain of its state machines:

- Its state machine has a request in this round, and the value of the entity is the current value of the state machine.
- Its state machine is itself a scoped entity that leaves. So the entities of a sub state go at the same barrier as the sub state.
- Its state machine is destroyed, or carries no `ideatek:State`. Such an entity leaves in the first phase of the round after the one in which its state machine went.

`clear` follows the chain of at most 8 state machines. A longer chain gets 1 line of the log, and the entity stays.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `false` | The module calls no library: any thread of Bridge can run it. |
| `reloadable` | `false` | The module keeps no state, but its reload is not tested. |
| `spawns` | `"none"` | The module spawns no entity: it destroys the scoped entities of the game. |
| `changes` | 16384 | The detaches, the writes and the attaches of the state machines of a round, and the destroys of their scoped entities. |
| `persistent` | `"none"` | The module keeps nothing between the rounds. |
| `scratch` | 4096 | The room of the declarations at `init`, and of the lines of the log. |
| `log_ring` | 512 | 32 bytes of the ring for each line of the log. |
| `spans` | 8192 | The spans of its 3 queries, and the columns of its 5 types. |

- `first` of the module is the phase that starts the round, in the map of the deployment.
- A pair puts `ideatek_states_bridge:states:apply` after `ideatek_frame_sdl3:clock:round`, and before each reader of `ideatek:State` of the first phase.
- The module takes the grants of the states module: `docs/extern/kernel/ideatek_states.h.md`, *The grants*.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `no state machine for <entity>` | An entity with `ideatek:StateRequest` carries no `ideatek:State`. The module detaches the request. |
| `the scoped chain of <entity> is deeper than 8` | The chain of the state machines of a scoped entity has more than 8 steps. The entity stays. |

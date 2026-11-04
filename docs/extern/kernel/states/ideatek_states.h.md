# include/kernel/states/ideatek_states.h

`include/kernel/states/ideatek_states.h` gives the types of the states part: the state machines of the game, their requests and their changes.
`readme.md`, *The contracts*, tells the rules of each contract.
`ideatek:StateScoped` names its state machine with `ideatek_entity_t` of `include/platform/world/ideatek_world.h`. Thus the header includes that header.

## The roles

- The game spawns each state machine with `ideatek:State`. Then it reads `ideatek:State` and `ideatek:StateChange`.
- A module writes `ideatek:StateRequest` and `ideatek:StateScoped`. The game is such a module.
- The states module writes `ideatek:State` and `ideatek:StateChange`, and reads `ideatek:StateRequest` and `ideatek:StateScoped`. A deployment has one states module.

The states module has no setting.

Out: a condition that stops a system, and a computed state as a type of the contract.
Comes back: a condition that stops a system, when Bridge gets one.
Comes back: a computed state as a type, when a contract can hold code.

## The state machines

A state machine is an entity with `ideatek:State`. The game gives the meaning of each value of `value`.
A deployment can hold many state machines, for example 1 for the menus and 1 for a level.
The spawn of a state machine is no change. An observer of `BRIDGE_ON_ENTER` of `ideatek:State` hears the first value.

A sub state is a state machine that carries `ideatek:StateScoped`. It lives only while its parent state machine has the named value.
A computed state is a system of the game. It reads other state machines and writes `ideatek:StateRequest` on its own state machine.
The pause is not a state: it is `paused` of `ideatek:Frame` in `include/kernel/frame/ideatek_frame.h`.

A system that runs only in a state keeps the id of its state machine. It reads `ideatek:State` of that entity and returns at once for another value.

## The changes

The states module runs in the first phase of the round. A pair of the deployment puts it after the clock module and before each reader of `ideatek:State`.
For each state machine with `ideatek:StateRequest`, the states module applies the request and detaches it. Thus it applies at most 1 request for each state machine in a round.

A request with the current value is a change from the value to itself.
A request on an entity with no `ideatek:State` changes nothing. The states module detaches it and writes 1 line to the log.
The states module detaches `ideatek:StateChange` in the round after the change.

```pseudo
PROCEDURE apply(m : U64)
   old <- state_of(m)
   new <- request_of(m)
   CALL detach_request(m)
   CALL clear(m, old)
   CALL set_state(m, new)
   CALL set_change(m, old, new)
ENDPROCEDURE

PROCEDURE clear(m : U64, v : U32)
   e <- first_scoped(m, v)
   WHILE e <> 0
      CALL remove(e)
      e <- next_scoped(m, v, e)
   ENDWHILE
ENDPROCEDURE

PROCEDURE remove(e : U64)
   IF has_state(e) THEN
      s <- first_scoped_any(e)
      WHILE s <> 0
         CALL remove(s)
         s <- next_scoped_any(e, s)
      ENDWHILE
   ENDIF
   CALL destroy(e)
ENDPROCEDURE
```

- `first_scoped` and `next_scoped` walk the entities whose `ideatek:StateScoped` names the state machine `m` and the value `v`.
- `first_scoped_any` and `next_scoped_any` walk the entities whose `ideatek:StateScoped` names the state machine `e`, with each value.
- So a leaf goes before its root, and a sub state takes its own entities with it.
- An entity that names another value of the state machine stays.
- When a state machine is destroyed by another module, the states module destroys each entity that names it.

## ideatek:State

`ideatek:State` is the current value of a state machine.
Its C type `ideatek_state_t` has 4 bytes, and an alignment of 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `value` | `uint32_t` | 0 | The current value. |

## ideatek:StateRequest

`ideatek:StateRequest` asks for the next value of a state machine. A module attaches it to the state machine.
Its C type `ideatek_state_request_t` has 4 bytes, and an alignment of 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `value` | `uint32_t` | 0 | The next value. |

When 2 modules attach a request in the same phase, the barrier keeps the last attach.

## ideatek:StateChange

`ideatek:StateChange` tells the change of a state machine in this round. The states module attaches it to the state machine.
Its C type `ideatek_state_change_t` has 8 bytes, and an alignment of 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `from` | `uint32_t` | 0 | The value before the change. |
| `to` | `uint32_t` | 4 | The value after the change. |

To wait for a change, a module observes `BRIDGE_ON_ATTACH` of `ideatek:StateChange`.

## ideatek:StateScoped

`ideatek:StateScoped` ties an entity to a value of a state machine. The states module destroys the entity when the state machine leaves that value.
Its C type `ideatek_state_scoped_t` has 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `machine` | `ideatek_entity_t` | 0 | The state machine. |
| `value` | `uint32_t` | 8 | The value. |

Bytes 12 to 15 are padding: the alignment of 8 rounds the size up to 16.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:State` | `ideatek_state_t` | `ideatek_state_t_desc` | `IDEATEK_STATE_FIELDS` |
| `ideatek:StateRequest` | `ideatek_state_request_t` | `ideatek_state_request_t_desc` | `IDEATEK_STATE_REQUEST_FIELDS` |
| `ideatek:StateChange` | `ideatek_state_change_t` | `ideatek_state_change_t_desc` | `IDEATEK_STATE_CHANGE_FIELDS` |
| `ideatek:StateScoped` | `ideatek_state_scoped_t` | `ideatek_state_scoped_t_desc` | `IDEATEK_STATE_SCOPED_FIELDS` |

```text
{ uint32_t value; }
{ uint32_t value; }
{ uint32_t from; uint32_t to; }
{ ideatek_entity_t machine; uint32_t value; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:State` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StateRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StateChange` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:StateScoped` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares `ideatek:ideatek_entity_t` before `ideatek:StateScoped`.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:State`, `ideatek:StateRequest` and `ideatek:StateScoped` | `ideatek:ideatek_entity_t` and `ideatek:StateChange` |
| Another module | `ideatek:StateRequest` and `ideatek:StateScoped` | `ideatek:ideatek_entity_t`, `ideatek:State` and `ideatek:StateChange` |
| The states module | `ideatek:State` and `ideatek:StateChange` | `ideatek:ideatek_entity_t`, `ideatek:StateRequest` and `ideatek:StateScoped` |

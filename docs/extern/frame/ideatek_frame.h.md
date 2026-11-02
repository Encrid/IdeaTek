# include/frame/ideatek_frame.h

`include/frame/ideatek_frame.h` gives the types of the time of a round and of the fixed steps: their C types and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.

## The time

- The unit of time is 1 second. Each time has the type `double`.
- The game time stops during a pause, and goes faster or slower with the time scale. The real time does not.
- A fixed step always advances the simulation by the same time. Physics, gameplay and the net modules advance in fixed steps.
- Render, audio, the camera and the UI advance with the time of the round.

## The roles

- The clock module writes `ideatek:Frame` and `ideatek:Tick`. A deployment has one clock module.
- A reader reads `ideatek:Frame` or `ideatek:Tick`. Each other module is a reader.

The clock module writes `ideatek:Frame` and `ideatek:Tick` in the first phase of the round.
The deployment has a fixed phase, which runs `steps` times in each round, or 1 time when `steps` is 0.
At the start of each run of the fixed phase, the clock module writes `tick`.
In a phase where the clock module writes, a pair of the deployment puts the clock module before each reader.

The rate of the steps, the most steps in a round and the time scale are settings of the clock module.

## ideatek:Frame

`ideatek:Frame` is the time of the round.
Its C type `ideatek_frame_t` has 32 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `delta` | `double` | 0 | The game time since the last round. It is 0 during a pause. |
| `elapsed` | `double` | 8 | The game time since the first round. |
| `real_delta` | `double` | 16 | The real time since the last round. |
| `index` | `uint64_t` | 24 | The number of the round. The first round is 1. |

## ideatek:Tick

`ideatek:Tick` tells the fixed steps of the round.
Its C type `ideatek_tick_t` has 24 bytes, and an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `step` | `double` | 0 | The game time of one step. |
| `tick` | `uint64_t` | 8 | In a run of the fixed phase, the number of the step that the run does. The first step is 1. |
| `steps` | `uint32_t` | 16 | How many steps the round does. |
| `fraction` | `float` | 20 | The part of a step that the round has after its last step, from 0 to less than 1. |

When `steps` is 0, the readers do no step in the fixed phase.
Render shows the state between the last 2 steps, at `fraction`.
Two peers that do the step with the same `tick` and the same inputs get the same state.

In the first phase of each round, the clock module writes the values:

```pseudo
PROCEDURE start_round(real : F64)
   real_delta <- real
   delta <- real * time_scale
   elapsed <- elapsed + delta
   index <- index + 1
   step <- 1 / rate
   pending <- pending + delta
   steps <- INT(pending / step)
   IF steps > most_steps THEN
      steps <- most_steps
      pending <- steps * step
   ENDIF
   pending <- pending - steps * step
   fraction <- pending / step
ENDPROCEDURE
```

- `real` is the real time since the last round.
- `pending` is the game time that no step did yet. It is 0 at the first round.
- A round that has more steps than `most_steps` drops the time of the other steps. Thus a slow round does not make the next round slower.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:Frame` | `ideatek_frame_t` | `ideatek_frame_t_desc` | `IDEATEK_FRAME_FIELDS` |
| `ideatek:Tick` | `ideatek_tick_t` | `ideatek_tick_t_desc` | `IDEATEK_TICK_FIELDS` |

```text
{ double delta; double elapsed; double real_delta; uint64_t index; }
{ double step; uint64_t tick; uint32_t steps; float fraction; }
```

## The declarations


| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:Frame` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Tick` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |

The clock module attaches each singleton with the id 0 in its `init`.

## The grants


| Role | `write` | `read` |
| --- | --- | --- |
| The clock module | `ideatek:Frame` and `ideatek:Tick` | No type. |
| A reader | No type. | `ideatek:Frame` or `ideatek:Tick`, or both |

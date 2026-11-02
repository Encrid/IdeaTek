# include/frame/ideatek_frame.h

`include/frame/ideatek_frame.h` gives the types of the time of a round, of the fixed steps and of the time request: their C types and their descriptors.
`readme.md`, *The contracts*, tells the rules of each contract.

## The time

- The unit of time is 1 second. Each time has the type `double`.
- The game time stops during a pause, and goes faster or slower with the time scale. The real time does not.
- A fixed step always advances the simulation by the same time. Physics, gameplay and the net modules advance in fixed steps.
- Render, audio, the camera and the UI advance with the time of the round.

## The roles

- The clock module writes `ideatek:Frame`, `ideatek:Tick` and `ideatek:TimeRequest`. A deployment has one clock module.
- A game module writes `ideatek:TimeRequest`.
- A reader reads `ideatek:Frame` or `ideatek:Tick`. Each other module is a reader.

The clock module writes `ideatek:Frame` and `ideatek:Tick` in the first phase of the round.
The deployment has a fixed phase, which runs `steps` times in each round, or 1 time when `steps` is 0.
At the start of each run of the fixed phase, the clock module writes `tick` and the `elapsed` of `ideatek:Tick`.
In a phase where the clock module writes, a pair of the deployment puts the clock module before each reader.

## The settings of the clock module

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `most_steps` | 8 | 1 to 100 | The most steps in a round. |
| `time_source` | `"os"` | `"os"` or `"fixed"` | With `"os"`, `real` is the real time since the last round. With `"fixed"`, `real` is `fixed_round` in each round. |
| `fixed_round` | 0.015625 | more than 0 | The time of each round when `time_source` is `"fixed"`. |

A test or a recording uses `"fixed"`, so that each run gets the same times.

## ideatek:Frame

`ideatek:Frame` is the time of the round.
Its C type `ideatek_frame_t` takes 56 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `delta` | `double` | 0 | The game time since the last round. It is 0 during a pause. |
| `elapsed` | `double` | 8 | The game time since the first round. |
| `real_delta` | `double` | 16 | The real time since the last round. |
| `real_elapsed` | `double` | 24 | The real time since the first round. |
| `scale` | `double` | 32 | The time scale of the round. |
| `index` | `uint64_t` | 40 | The number of the round. The first round is 1. |
| `paused` | `uint8_t` | 48 | 1 when the round is in a pause, else 0. |

During a pause, `delta` and `steps` are 0, and `elapsed`, `tick` and `fraction` do not change.
`real_delta`, `real_elapsed` and `index` advance in each round.
A menu that runs during a pause reads `paused` and uses `real_delta`.

## ideatek:Tick

`ideatek:Tick` tells the fixed steps of the round.
Its C type `ideatek_tick_t` takes 32 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `step` | `double` | 0 | The game time of one step: 1 divided by the rate. |
| `elapsed` | `double` | 8 | In a run of the fixed phase, the game time at the end of the step that the run does. |
| `tick` | `uint64_t` | 16 | In a run of the fixed phase, the number of the step that the run does. The first step is 1. |
| `steps` | `uint32_t` | 24 | How many steps the round does. |
| `fraction` | `float` | 28 | The part of a step that the round has after its last step, from 0 to less than 1. |

When `steps` is 0, the readers do no step in the fixed phase.
Render shows the state between the last 2 steps, at `fraction`.
Two peers that do the step with the same `tick`, the same rate and the same inputs get the same state.
The time scale changes how many steps a round does. It does not change `step`.

## ideatek:TimeRequest

`ideatek:TimeRequest` is the time that a game module asks for.
Its C type `ideatek_time_request_t` takes 16 bytes, with an alignment of 8.

| Field | C type | Offset | Value | Default |
| --- | --- | --- | --- | --- |
| `scale` | `double` | 0 | The time scale. 1 is the real speed. | 1 |
| `rate` | `uint32_t` | 8 | The steps in 1 second, from 1 to 1000. | 64 |
| `paused` | `uint8_t` | 12 | 1 for a pause, else 0. | 0 |

The clock module attaches `ideatek:TimeRequest` with the defaults in its `init`.
Then a game module writes it, and the clock module applies it at the start of the next round.
The clock module refuses these values:

- a `scale` that is not finite or is less than 0
- a `rate` that is 0 or more than 1000
- a `paused` that is not 0 or 1
For a refused field, the clock module keeps the last value of the field and writes 1 line to the log.

## The values of a round

In the first phase of each round, the clock module writes the values:

```pseudo
PROCEDURE start_round(real : F64)
   CALL apply_request()
   real_delta <- real
   real_elapsed <- real_elapsed + real
   index <- index + 1
   step <- 1 / rate
   IF paused THEN
      delta <- 0
   ELSE
      delta <- real * scale
   ENDIF
   pending <- pending + delta
   steps <- INT(pending / step)
   IF steps > most_steps THEN
      delta <- delta - (pending - most_steps * step)
      steps <- most_steps
      pending <- steps * step
   ENDIF
   pending <- pending - steps * step
   elapsed <- elapsed + delta
   fraction <- pending / step
ENDPROCEDURE
```

- `apply_request` copies each field of `ideatek:TimeRequest` that the clock module does not refuse.
- `real` is the real time since the last round. It is 0 at the first round.
- `pending` is the game time that no step did yet. It is 0 at the first round.
- At the first round, each time, `steps`, `tick` and `fraction` are 0, and `index` is 1.
- A round that has more steps than `most_steps` drops the time of the other steps from `delta` and from `elapsed`. Thus a slow round does not make the next round slower.
- Thus `delta` is at most `most_steps` times `step`: 0.125 seconds with the defaults.
- A time scale above 1 is limited to `most_steps` times the rounds in a second, divided by the rate.

At the start of each run of the fixed phase, the clock module adds 1 to `tick` and adds `step` to the `elapsed` of `ideatek:Tick`.
After the last step of a round, the `elapsed` of `ideatek:Frame` less the `elapsed` of `ideatek:Tick` is `pending`.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:Frame` | `ideatek_frame_t` | `ideatek_frame_t_desc` | `IDEATEK_FRAME_FIELDS` |
| `ideatek:Tick` | `ideatek_tick_t` | `ideatek_tick_t_desc` | `IDEATEK_TICK_FIELDS` |
| `ideatek:TimeRequest` | `ideatek_time_request_t` | `ideatek_time_request_t_desc` | `IDEATEK_TIME_REQUEST_FIELDS` |

```text
{ double delta; double elapsed; double real_delta; double real_elapsed; double scale; uint64_t index; uint8_t paused; }
{ double step; double elapsed; uint64_t tick; uint32_t steps; float fraction; }
{ double scale; uint32_t rate; uint8_t paused; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:Frame` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Tick` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TimeRequest` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |

The clock module attaches each singleton with the id 0 in its `init`.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The clock module | `ideatek:Frame`, `ideatek:Tick` and `ideatek:TimeRequest` | No type. |
| A game module | `ideatek:TimeRequest` | No type. |
| A reader | No type. | `ideatek:Frame` or `ideatek:Tick`, or both |

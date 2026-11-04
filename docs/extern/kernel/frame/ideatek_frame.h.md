# include/kernel/frame/ideatek_frame.h

`include/kernel/frame/ideatek_frame.h` gives the types of the time of a round, of the fixed steps and of the time request: their C types and their descriptors.
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
| `sleeper` | `"clock"` | `"clock"` or `"events"` | The module that waits when `max_rate` limits the rounds. |
| `smoothing` | `"none"` | `"none"` or `"refresh"` | With `"refresh"`, `real` near a common refresh time of a display becomes that time. |

A test or a recording uses `"fixed"`, so that each run gets the same times.

## The smoothing

With vsync, the display shows each image after the same time, but the read of the real time varies
from round to round. With `"refresh"`, when `real` is in the range of a row, `real` becomes the
time of the row. Else `real` does not change.

| Refresh rate | Time | Range |
| --- | --- | --- |
| 60 Hz | 16.6667 ms | 1 ms |
| 72 Hz | 13.8889 ms | 0.25 ms |
| 75 Hz | 13.3333 ms | 0.25 ms |
| 85 Hz | 11.7647 ms | 0.25 ms |
| 90 Hz | 11.1111 ms | 0.25 ms |
| 100 Hz | 10 ms | 0.5 ms |
| 120 Hz | 8.3333 ms | 0.5 ms |
| 144 Hz | 6.9445 ms | 0.5 ms |
| 240 Hz | 4.1667 ms | 1 ms |

The range is the most difference between `real` and the time of the row.

## The limit of the rate

Bridge starts each round when the last round ends. Only a module can wait between 2 rounds.
`max_rate` of `ideatek:TimeRequest` is the most rounds in 1 second. 0 sets no limit.
Exactly 1 module waits, and `sleeper` tells which module:

- `"clock"`: the clock module waits at the start of its system, before it reads the real time. It waits until `1 / max_rate` passed since its last start. It does not wait at the first round. This works with no window.
- `"events"`: the clock module does not wait. It writes `wait`, and the window module waits for the events of the system for at most `wait`. An event ends the wait at once.

`max_rate` is the limit of the frame rate that a settings menu sets. With vsync, the longer wait of `max_rate` and of the display is the wait of the round.
In a web page, the browser starts each round, and no module waits. There, `max_rate` has no effect.
A wait takes at most 1 second. A timeout of the system that waits must be longer than 1 second.

## ideatek:Frame

`ideatek:Frame` is the time of the round.
Its C type `ideatek_frame_t` takes 64 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `delta` | `double` | 0 | The game time since the last round. It is 0 during a pause. |
| `elapsed` | `double` | 8 | The game time since the first round. |
| `real_delta` | `double` | 16 | The real time since the last round. |
| `real_elapsed` | `double` | 24 | The real time since the first round. |
| `scale` | `double` | 32 | The time scale of the round. |
| `wait` | `double` | 40 | The longest time that the window module waits for the events of the system. It is 0 when `sleeper` is `"clock"` or when `max_rate` is 0. |
| `index` | `uint64_t` | 48 | The number of the round. The first round is 1. |
| `paused` | `uint8_t` | 56 | 1 when the round is in a pause, else 0. |

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
Its C type `ideatek_time_request_t` takes 24 bytes, with an alignment of 8.

| Field | C type | Offset | Value | Default |
| --- | --- | --- | --- | --- |
| `scale` | `double` | 0 | The time scale. 1 is the real speed. | 1 |
| `rate` | `uint32_t` | 8 | The steps in 1 second, from 1 to 1000. | 64 |
| `max_rate` | `uint32_t` | 12 | The most rounds in 1 second, from 0 to 1000. 0 sets no limit. | 0 |
| `paused` | `uint8_t` | 16 | 1 for a pause, else 0. | 0 |

The clock module attaches `ideatek:TimeRequest` with the defaults in its `init`.
Then a game module writes it, and the clock module applies it at the start of the next round.
The clock module refuses these values:

- a `scale` that is not finite or is less than 0
- a `rate` that is 0 or more than 1000
- a `max_rate` that is more than 1000
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
   wait <- 0
   IF sleeper = "events" AND max_rate > 0 THEN
      wait <- 1 / max_rate
   ENDIF
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
- At the first round, `delta`, `elapsed`, `real_delta`, `real_elapsed`, `steps`, `tick` and `fraction` are 0, and `index` is 1.
- A round that has more steps than `most_steps` drops the time of the other steps from `delta` and from `elapsed`. Thus a slow round does not make the next round slower.
- Thus `delta` is at most `most_steps` times `step`: 0.125 seconds with the defaults.
- A time scale above 1 is limited to `most_steps` times the rounds in a second, divided by the rate.

At the start of each run of the fixed phase, the clock module adds 1 to `tick` and adds `step` to the `elapsed` of `ideatek:Tick`.
After the last step of a round, the `elapsed` of `ideatek:Frame` less the `elapsed` of `ideatek:Tick` is `pending`.

## A timer

The contract has no type for a timer. A module keeps its timers in its own components, and reads the time from `ideatek:Frame`:

```pseudo
FUNCTION is_done(elapsed : F64, deadline : F64) RETURNS BOOLEAN
   RETURN elapsed >= deadline
ENDFUNCTION

FUNCTION finishes(elapsed : F64, start : F64, period : F64, handled : U64) RETURNS U64
   DECLARE total : U64
   total <- INT((elapsed - start) / period)
   RETURN total - handled
ENDFUNCTION
```

- A timer that runs 1 time ends when `is_done` is `TRUE`. Its `deadline` is the `elapsed` of its start plus its time.
- A timer that repeats finishes `finishes` times in the round. A slow round can give more than 1 finish. The module then adds the result to `handled`.
- A timer of the game time reads the `elapsed` of `ideatek:Frame`. It stops during a pause, and follows the time scale.
- A timer of the real time reads `real_elapsed`.
- A timer of the fixed phase reads the `elapsed` of `ideatek:Tick`, so that each peer gets the same finishes.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:Frame` | `ideatek_frame_t` | `ideatek_frame_t_desc` | `IDEATEK_FRAME_FIELDS` |
| `ideatek:Tick` | `ideatek_tick_t` | `ideatek_tick_t_desc` | `IDEATEK_TICK_FIELDS` |
| `ideatek:TimeRequest` | `ideatek_time_request_t` | `ideatek_time_request_t_desc` | `IDEATEK_TIME_REQUEST_FIELDS` |

```text
{ double delta; double elapsed; double real_delta; double real_elapsed; double scale; double wait; uint64_t index; uint8_t paused; }
{ double step; double elapsed; uint64_t tick; uint32_t steps; float fraction; }
{ double scale; uint32_t rate; uint32_t max_rate; uint8_t paused; }
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

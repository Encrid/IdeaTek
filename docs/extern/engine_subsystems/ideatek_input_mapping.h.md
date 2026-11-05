# include/engine_subsystems/ideatek_input_mapping.h

`include/engine_subsystems/ideatek_input_mapping.h` gives the types of the map of the inputs: actions, bindings, modifiers, triggers, contexts, players, rebinding and the last device.
`readme.md`, *The contracts*, tells the rules of each contract.
Its fields name entities with `ideatek_entity_t`, so the header includes the world part.

## The roles

- The game writes the actions, the bindings with their modifiers and triggers, the contexts, the players, the devices of each player and the captures.
- The mapping module reads the input records of the input part of layer 2. It writes `ideatek:InputActionState`, `ideatek:InputCaptured` and `ideatek:InputLastDevice`.

## The time of the map

The mapping module runs first in each fixed step. The input of a round goes into its first step.
A round with no step keeps the edges until the next step. A press and a release in 1 round give STARTED and COMPLETED together.
The times of the triggers count with `elapsed` of `ideatek:Tick`.

A context with `IDEATEK_CONTEXT_FLAGS_REAL_TIME` is mapped in each round instead, also in a pause. Its times count with `real_delta` of `ideatek:Frame`.

## The sources

| Source | `control` | Value |
| --- | --- | --- |
| `IDEATEK_INPUT_SOURCE_KEY` | The USB usage, compared with the physical code | 0 or 1 |
| `IDEATEK_INPUT_SOURCE_KEY_LAYOUT` | The USB usage, compared with the key of the layout | 0 or 1 |
| `IDEATEK_INPUT_SOURCE_MOUSE_BUTTON` | The button, from 1 to 32. 1 is the left button | 0 or 1 |
| `IDEATEK_INPUT_SOURCE_MOUSE_MOTION` | 0 | The motion in pixels on x and y |
| `IDEATEK_INPUT_SOURCE_MOUSE_WHEEL` | 0 | The ticks on x and y |
| `IDEATEK_INPUT_SOURCE_GAMEPAD_BUTTON` | 1 bit of `ideatek_gamepad_button_t` | 0 or 1 |
| `IDEATEK_INPUT_SOURCE_GAMEPAD_STICK` | 0 for the left stick, 1 for the right stick | x and y |
| `IDEATEK_INPUT_SOURCE_GAMEPAD_TRIGGER` | 0 for the left trigger, 1 for the right trigger | x, from 0 to 1 |
| `IDEATEK_INPUT_SOURCE_XR_BOOLEAN` | 0 | 0 or 1, from `ideatek:XrControlState` |
| `IDEATEK_INPUT_SOURCE_XR_FLOAT` | 0 | x, from `ideatek:XrControlState` |
| `IDEATEK_INPUT_SOURCE_XR_VECTOR2` | 0 | x and y, from `ideatek:XrControlState` |
| `IDEATEK_INPUT_SOURCE_GAMEPAD_GYRO` | 0 | The turn of the step about x, y and z in radians, from `turn` of `ideatek:GamepadMotion` |
| `IDEATEK_INPUT_SOURCE_GAMEPAD_ACCEL` | 0 | x, y and z in m / s^2, from `accel` of `ideatek:GamepadMotion` |
| `IDEATEK_INPUT_SOURCE_TOUCH` | The rank of the finger among the fingers that are down, from 0 | 0 or 1 |
| `IDEATEK_INPUT_SOURCE_TOUCH_MOTION` | The rank of the finger | The motion in pixels on x and y |
| `IDEATEK_INPUT_SOURCE_EXTERNAL` | 0 | x, y and z of `ideatek:InputExternal`: *The external sources* |

An XR binding also has `ideatek:XrBindingPath` of the xr part, and the XR module writes its value.
A gyro or accel binding needs `ideatek:GamepadSensorRequest` with `enabled` on its gamepad, as the input part tells.

## The modifiers

The modifiers of a binding are optional types on the binding entity. They apply in this order: dead zone, curve, response curve, scale, negate, swizzle, clamp, smooth, linear step.

Keys W, A, S and D give 1 action of 2 axes with 4 bindings. D is the key. A adds a negate of X. W adds a swizzle. S adds a negate of X and a swizzle.
The y of a stick of the input part points down, so a forward stick needs a negate of Y.

## The triggers

The triggers of a binding are optional types on the binding entity, as the modifiers are. A binding has at most 1 type of each kind.
A control is actuated when its magnitude is at least the threshold.
The action is TRIGGERED when 1 of its explicit triggers triggers, and each chord is true.
The explicit triggers are pressed, released, hold, hold and release, tap, multi tap, pulse, toggle, flick and cooldown.
With none of these 10, the binding acts as down: TRIGGERED while its value is not 0.

The references do not agree on some times and rates, so they have no default and the game sets them.
A tap that is longer than its time gives CANCELED.

## The contexts

A binding with no valid context is not read. A control is 1 source, 1 control and 1 gamepad.
An active context with CONSUME that binds a control blocks it for the active contexts of a lower priority. The same priority does not block.
With IGNORE_HELD, a button that is held when the context turns active is ignored until its release.
A context that turns inactive gives CANCELED to its actions in the next step.

## The players and the devices

A context with a player of 0 reads each device, so 1 player needs no setup.
A context with a player reads only the devices whose `ideatek:InputDevice` names that player. Each player has its own actions and contexts, as a copy of a prefab.
To join, the game reads the gamepad event of a gamepad with no player, and attaches `ideatek:InputDevice`. A consume works for each device apart.

## The rebinding

While an `ideatek:InputCapture` exists, each control of the devices of its player that becomes actuated gives `ideatek:InputCaptured`. It is an event of 1 round in the form of the input part.
While a capture exists, the module reads no binding of that player. With a player of 0, it reads no binding at all.
The game takes the first event, writes the binding, and removes the capture. To cancel, the game ignores the event of its cancel control.

The game finds the conflicts in the bindings of the context. No XR source is captured: the runtime of XR rebinds them. No touch, gyro, accel or external source is captured either.

## The external sources

A binding of `IDEATEK_INPUT_SOURCE_EXTERNAL` reads `ideatek:InputExternal` on its own entity. Another module writes it, for example a module of Steam Input.
That module finds the actions and the contexts by their `name`. A binding with no `ideatek:InputExternal` gives 0.
The modifiers and the triggers of an external binding apply as for any binding.

## The origins

The mapping module writes `ideatek:InputActionOrigins` on each action in each step. A record of `origins`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `control` | `uint32_t` | 0 | The control, or `origin` of `ideatek:InputExternal` for an external binding. |
| `source` | `uint8_t` | 4 | The `ideatek_input_source_t` of the binding. |

The records have no bytes between them. They are the bindings of the active contexts whose source fits the kind of `ideatek:InputLastDevice` of their context.

## The combos

An action with `ideatek:InputCombo` needs no binding. A record of `steps`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `action` | `uint64_t` | 0 | The entity of the action of the step. |
| `timeout` | `float` | 8 | The longest wait after the step before, in seconds. |
| `edges` | `ideatek_action_edges_t` | 12 | The edges of the action that end the step. |

A record of `cancels` is `action` at offset 0 and `edges` at offset 8. The records have no bytes between them.
The action triggers in the step where the last step ends. A step that misses its timeout, or the edges of a cancel, start again at the first step.
The timeout of the first step is not read.

## The cardinal bindings

A binding with `ideatek:InputCardinal` reads 4 controls of its source: a key, a key of the layout, or a gamepad button. Its own `control` is not read.
x is `right` minus `left`, and y is `up` minus `down`.

## The response curves

A record of a field of `ideatek:InputResponseCurve` is 2 floats, an input magnitude and an output magnitude, by rising input, with no bytes between them.
The output between 2 records is a straight blend. Before the first record and after the last, the output of that record stays. An axis with no bytes does not change.

## The last device

A use is a control that becomes actuated on a device that the context reads. The module writes `ideatek:InputLastDevice` on each context.
Each finger belongs to the keyboard and mouse entity, because the input part gives a finger no device.
When a finger event and a mouse event come in 1 step, TOUCH wins. So a touch that also gives mouse events does not change the kind.

## ideatek:ideatek_action_type_t

`ideatek_action_type_t` tells the value of an action.
`ideatek_action_type_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ACTION_TYPE_BUTTON` | A value from 0 to 1. |
| 1 | `IDEATEK_ACTION_TYPE_AXIS_1` | 1 axis. |
| 2 | `IDEATEK_ACTION_TYPE_AXIS_2` | 2 axes. |
| 3 | `IDEATEK_ACTION_TYPE_AXIS_3` | 3 axes. |

## ideatek:ideatek_accumulate_t

`ideatek_accumulate_t` tells how the bindings of an action join.
`ideatek_accumulate_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ACCUMULATE_HIGHEST` | The binding of the highest magnitude wins. |
| 1 | `IDEATEK_ACCUMULATE_SUM` | The bindings add. |

## ideatek:InputAction

`ideatek:InputAction` is an action of the game, for example jump or move.
`ideatek_input_action_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | No default | The name, for an external module and the saved bindings. |
| `type` | `ideatek_action_type_t` | 8 | No default | The value. |
| `accumulate` | `ideatek_accumulate_t` | 9 | `IDEATEK_ACCUMULATE_HIGHEST` | The join of the bindings. |

Bytes 10 to 11 hold no field.

## ideatek:ideatek_action_state_t

`ideatek_action_state_t` tells the state of an action, as in Enhanced Input of Unreal.
`ideatek_action_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ACTION_STATE_IDLE` | Nothing happens. |
| 1 | `IDEATEK_ACTION_STATE_ONGOING` | A trigger is on its way, for example a hold. |
| 2 | `IDEATEK_ACTION_STATE_TRIGGERED` | The action is on. |

## ideatek:ideatek_action_edges_t

`ideatek_action_edges_t` names the changes of an action in a step.
`ideatek_action_edges_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_ACTION_EDGES_STARTED` | The action left IDLE. |
| 0x02 | `IDEATEK_ACTION_EDGES_TRIGGERED` | The action triggered in the step. |
| 0x04 | `IDEATEK_ACTION_EDGES_CANCELED` | The action went back to IDLE without a trigger. |
| 0x08 | `IDEATEK_ACTION_EDGES_COMPLETED` | The action went back to IDLE after a trigger. |

## ideatek:InputActionState

`ideatek:InputActionState` is the value and the state of an action. The mapping module writes it.
`ideatek_input_action_state_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The first axis. A button gives 0, 1 or the depth of an analog button. |
| `y` | `float` | 4 | The second axis. |
| `z` | `float` | 8 | The third axis. |
| `held` | `float` | 12 | The seconds since the start. |
| `state` | `ideatek_action_state_t` | 16 | The state. |
| `edges` | `ideatek_action_edges_t` | 17 | The changes of the step. |

Bytes 18 to 19 hold no field.

## ideatek:ideatek_input_source_t

`ideatek_input_source_t` tells the kind of a control: *The sources*.
`ideatek_input_source_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_INPUT_SOURCE_KEY` | A key, by its place. |
| 1 | `IDEATEK_INPUT_SOURCE_KEY_LAYOUT` | A key, by its layout. |
| 2 | `IDEATEK_INPUT_SOURCE_MOUSE_BUTTON` | A mouse button. |
| 3 | `IDEATEK_INPUT_SOURCE_MOUSE_MOTION` | The motion of the mouse. |
| 4 | `IDEATEK_INPUT_SOURCE_MOUSE_WHEEL` | The wheel of the mouse. |
| 5 | `IDEATEK_INPUT_SOURCE_GAMEPAD_BUTTON` | A gamepad button. |
| 6 | `IDEATEK_INPUT_SOURCE_GAMEPAD_STICK` | A stick. |
| 7 | `IDEATEK_INPUT_SOURCE_GAMEPAD_TRIGGER` | A trigger of a gamepad. |
| 8 | `IDEATEK_INPUT_SOURCE_XR_BOOLEAN` | A button of an XR controller. |
| 9 | `IDEATEK_INPUT_SOURCE_XR_FLOAT` | A trigger of an XR controller. |
| 10 | `IDEATEK_INPUT_SOURCE_XR_VECTOR2` | A stick or a pad of an XR controller. |
| 11 | `IDEATEK_INPUT_SOURCE_GAMEPAD_GYRO` | The gyroscope of a gamepad. |
| 12 | `IDEATEK_INPUT_SOURCE_GAMEPAD_ACCEL` | The accelerometer of a gamepad. |
| 13 | `IDEATEK_INPUT_SOURCE_TOUCH` | A finger on a screen. |
| 14 | `IDEATEK_INPUT_SOURCE_TOUCH_MOTION` | The motion of a finger. |
| 15 | `IDEATEK_INPUT_SOURCE_EXTERNAL` | A value from another module. |

## ideatek:InputBinding

`ideatek:InputBinding` binds 1 control to 1 action.
`ideatek_input_binding_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `context` | `ideatek_entity_t` | 0 | The context. |
| `action` | `ideatek_entity_t` | 8 | The action. |
| `control` | `uint32_t` | 16 | The control: *The sources*. |
| `source` | `ideatek_input_source_t` | 20 | The kind of the control. |

Bytes 21 to 23 hold no field.

## ideatek:ideatek_dead_zone_kind_t

`ideatek_dead_zone_kind_t` tells the shape of a dead zone.
`ideatek_dead_zone_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_DEAD_ZONE_KIND_AXIAL` | Each axis apart. |
| 1 | `IDEATEK_DEAD_ZONE_KIND_RADIAL` | The length of the vector, remapped. |
| 2 | `IDEATEK_DEAD_ZONE_KIND_UNSCALED_RADIAL` | The length of the vector, not remapped. |

## ideatek:InputDeadZone

`ideatek:InputDeadZone` remaps a value from `lower` to `upper` into 0 to 1.
`ideatek_input_dead_zone_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `lower` | `float` | 0 | 0.2 | The value under which the result is 0. |
| `upper` | `float` | 4 | 1 | The value over which the result is 1. |
| `kind` | `ideatek_dead_zone_kind_t` | 8 | `IDEATEK_DEAD_ZONE_KIND_RADIAL` | The shape. |

Bytes 9 to 11 hold no field.

## ideatek:InputCurve

`ideatek:InputCurve` raises each axis to an exponent, with its sign kept.
`ideatek_input_curve_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `x` | `float` | 0 | 1 | The exponent of x. |
| `y` | `float` | 4 | 1 | The exponent of y. |
| `z` | `float` | 8 | 1 | The exponent of z. |

## ideatek:InputScale

`ideatek:InputScale` multiplies each axis.
`ideatek_input_scale_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `x` | `float` | 0 | 1 | The factor of x. |
| `y` | `float` | 4 | 1 | The factor of y. |
| `z` | `float` | 8 | 1 | The factor of z. |

## ideatek:ideatek_axes_t

`ideatek_axes_t` names axes of a value.
`ideatek_axes_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_AXES_X` | The x axis. |
| 0x02 | `IDEATEK_AXES_Y` | The y axis. |
| 0x04 | `IDEATEK_AXES_Z` | The z axis. |

## ideatek:InputNegate

`ideatek:InputNegate` changes the sign of axes.
`ideatek_input_negate_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `axes` | `ideatek_axes_t` | 0 | X, Y and Z | The axes that change sign. |

## ideatek:ideatek_swizzle_t

`ideatek_swizzle_t` tells a new order of the axes.
`ideatek_swizzle_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SWIZZLE_YXZ` | x and y change places. |
| 1 | `IDEATEK_SWIZZLE_ZYX` | x and z change places. |
| 2 | `IDEATEK_SWIZZLE_XZY` | y and z change places. |
| 3 | `IDEATEK_SWIZZLE_YZX` | The axes turn left. |
| 4 | `IDEATEK_SWIZZLE_ZXY` | The axes turn right. |

## ideatek:InputSwizzle

`ideatek:InputSwizzle` changes the order of the axes.
`ideatek_input_swizzle_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `order` | `ideatek_swizzle_t` | 0 | `IDEATEK_SWIZZLE_YXZ` | The new order. |

## ideatek:InputResponseCurve

`ideatek:InputResponseCurve` maps the magnitude of each axis through points, with its sign kept.
`ideatek_input_response_curve_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x_points` | `bridge_text_t` | 0 | The points of x: *The response curves*. |
| `y_points` | `bridge_text_t` | 8 | The points of y. |
| `z_points` | `bridge_text_t` | 16 | The points of z. |

## ideatek:InputClamp

`ideatek:InputClamp` keeps each axis between 2 values.
`ideatek_input_clamp_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `min` | `float[3]` | 0 | No default | The lowest value of x, y and z. |
| `max` | `float[3]` | 12 | No default | The highest value of x, y and z. |

## ideatek:InputSmooth

`ideatek:InputSmooth` smooths the value: `value = target + (value - target) exp(-decay_rate dt)`.
`ideatek_input_smooth_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `decay_rate` | `float` | 0 | No default | The speed of the approach, in 1 / s. |

## ideatek:InputLinearStep

`ideatek:InputLinearStep` moves the value toward the input at a fixed rate.
`ideatek_input_linear_step_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `accel_rate` | `float` | 0 | No default | The rise of the magnitude in each second. |
| `decel_rate` | `float` | 4 | No default | The fall of the magnitude in each second. |

## ideatek:InputPressed

`ideatek:InputPressed` triggers once when the control becomes actuated.
`ideatek_input_pressed_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |

## ideatek:InputReleased

`ideatek:InputReleased` triggers once when the control stops being actuated.
`ideatek_input_released_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |

## ideatek:ideatek_hold_flags_t

`ideatek_hold_flags_t` names the options of a hold.
`ideatek_hold_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_HOLD_FLAGS_ONE_SHOT` | The hold triggers once, not in each step after the time. |

## ideatek:InputHold

`ideatek:InputHold` triggers when the control stays actuated for a time.
`ideatek_input_hold_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |
| `time` | `float` | 4 | 1 | The seconds of the hold. |
| `flags` | `ideatek_hold_flags_t` | 8 | 0 | The options. |

Bytes 9 to 11 hold no field.

## ideatek:InputTap

`ideatek:InputTap` triggers when the control is released within a time.
`ideatek_input_tap_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |
| `time` | `float` | 4 | 0.2 | The longest tap. |

## ideatek:ideatek_pulse_flags_t

`ideatek_pulse_flags_t` names the options of a pulse.
`ideatek_pulse_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_PULSE_FLAGS_ON_START` | The first pulse comes at the start. |

## ideatek:InputPulse

`ideatek:InputPulse` triggers again and again while the control is actuated.
`ideatek_input_pulse_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |
| `interval` | `float` | 4 | 1 | The seconds between pulses. |
| `limit` | `uint32_t` | 8 | 0 | The most pulses. 0 means no limit. |
| `flags` | `ideatek_pulse_flags_t` | 12 | `IDEATEK_PULSE_FLAGS_ON_START` | The options. |

Bytes 13 to 15 hold no field.

## ideatek:InputHoldRelease

`ideatek:InputHoldRelease` triggers at the release after a hold of at least `time`, as a charged attack.
`ideatek_input_hold_release_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |
| `time` | `float` | 4 | No default | The shortest hold, in seconds. |

## ideatek:InputMultiTap

`ideatek:InputMultiTap` triggers after `count` taps, as a double tap.
`ideatek_input_multi_tap_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |
| `time` | `float` | 4 | 0.2 | The longest tap. |
| `gap` | `float` | 8 | No default | The longest wait between 2 taps. |
| `count` | `uint32_t` | 12 | 2 | The taps. |

## ideatek:InputToggle

`ideatek:InputToggle` turns on at a press and off at the next press. It triggers in each step while it is on.
`ideatek_input_toggle_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |

## ideatek:InputFlick

`ideatek:InputFlick` triggers when the control goes from rest to actuated within a time.
`ideatek_input_flick_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |
| `rest` | `float` | 4 | No default | The magnitude under which the control rests. |
| `time` | `float` | 8 | No default | The longest move from `rest` to `threshold`. |

## ideatek:InputCooldown

`ideatek:InputCooldown` triggers when the control becomes actuated, then waits for a time.
`ideatek_input_cooldown_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `threshold` | `float` | 0 | 0.5 | The magnitude at which the control is actuated. |
| `time` | `float` | 4 | No default | The seconds before it can trigger again. |

## ideatek:ideatek_chord_flags_t

`ideatek_chord_flags_t` names the options of a chord.
`ideatek_chord_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_CHORD_FLAGS_BLOCK` | The binding is blocked while the other action is triggered. |

## ideatek:InputChord

`ideatek:InputChord` needs another action to be triggered, for example a key with Shift.
`ideatek_input_chord_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `action` | `ideatek_entity_t` | 0 | No default | The other action. |
| `flags` | `ideatek_chord_flags_t` | 8 | 0 | The options. |

Bytes 9 to 15 hold no field.

## ideatek:InputCombo

`ideatek:InputCombo` makes its action trigger after a sequence of other actions: *The combos*.
`ideatek_input_combo_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `steps` | `bridge_text_t` | 0 | The records of the steps: *The combos*. |
| `cancels` | `bridge_text_t` | 8 | The records of the cancels. |
| `steps_count` | `uint32_t` | 16 | The records in `steps`. |
| `cancels_count` | `uint32_t` | 20 | The records in `cancels`. |

## ideatek:InputCardinal

`ideatek:InputCardinal` makes 1 binding of 4 keys or buttons: *The cardinal bindings*.
`ideatek_input_cardinal_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `up` | `uint32_t` | 0 | The control of +y. |
| `down` | `uint32_t` | 4 | The control of -y. |
| `left` | `uint32_t` | 8 | The control of -x. |
| `right` | `uint32_t` | 12 | The control of +x. |

## ideatek:InputExternal

`ideatek:InputExternal` is the value of an external binding: *The external sources*.
`ideatek_input_external_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `x` | `float` | 0 | 0 | The first axis. |
| `y` | `float` | 4 | 0 | The second axis. |
| `z` | `float` | 8 | 0 | The third axis. |
| `origin` | `uint32_t` | 12 | 0 | The control of the external module, for the prompts. |

## ideatek:InputActionOrigins

`ideatek:InputActionOrigins` names the controls that drive an action now. The mapping module writes it.
`ideatek_input_action_origins_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `origins` | `bridge_text_t` | 0 | The records of the controls: *The origins*. |
| `count` | `uint32_t` | 8 | The records in `origins`. |

## ideatek:ideatek_context_flags_t

`ideatek_context_flags_t` names the options of a context.
`ideatek_context_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_CONTEXT_FLAGS_ACTIVE` | The context is read. |
| 0x02 | `IDEATEK_CONTEXT_FLAGS_CONSUME` | Its controls block the contexts of a lower priority. |
| 0x04 | `IDEATEK_CONTEXT_FLAGS_IGNORE_HELD` | A held button waits for its release. |
| 0x08 | `IDEATEK_CONTEXT_FLAGS_REAL_TIME` | The context is mapped in each round, also in a pause. |

## ideatek:InputContext

`ideatek:InputContext` is a set of bindings that turns on and off: *The contexts*.
`ideatek_input_context_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `name` | `bridge_text_t` | 0 | No default | The name, for an external module. |
| `player` | `ideatek_entity_t` | 8 | 0 | The player. 0 means each device. |
| `priority` | `int32_t` | 16 | 0 | The rank against other contexts. |
| `flags` | `ideatek_context_flags_t` | 20 | ACTIVE, CONSUME and IGNORE_HELD | The options. |

Bytes 21 to 23 hold no field.

## ideatek:InputPlayer

`ideatek:InputPlayer` makes an entity a local player.
`ideatek_input_player_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `flags` | `uint8_t` | 0 | No flag yet. It is 0. |

## ideatek:InputDevice

`ideatek:InputDevice` gives the keyboard and mouse entity or a gamepad entity of the input part to a player.
`ideatek_input_device_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | The player. |

## ideatek:ideatek_capture_sources_t

`ideatek_capture_sources_t` names the sources that a capture takes.
`ideatek_capture_sources_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_CAPTURE_SOURCES_KEY` | Keys by place. |
| 0x02 | `IDEATEK_CAPTURE_SOURCES_KEY_LAYOUT` | Keys by layout. |
| 0x04 | `IDEATEK_CAPTURE_SOURCES_MOUSE_BUTTON` | Mouse buttons. |
| 0x08 | `IDEATEK_CAPTURE_SOURCES_MOUSE_MOTION` | The motion of the mouse. |
| 0x10 | `IDEATEK_CAPTURE_SOURCES_MOUSE_WHEEL` | The wheel. |
| 0x20 | `IDEATEK_CAPTURE_SOURCES_GAMEPAD_BUTTON` | Gamepad buttons. |
| 0x40 | `IDEATEK_CAPTURE_SOURCES_GAMEPAD_STICK` | Sticks. |
| 0x80 | `IDEATEK_CAPTURE_SOURCES_GAMEPAD_TRIGGER` | Gamepad triggers. |

## ideatek:InputCapture

`ideatek:InputCapture` asks for the next control of a player: *The rebinding*.
`ideatek_input_capture_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | 0 | The player. 0 means each device. |
| `sources` | `ideatek_capture_sources_t` | 8 | Each source except KEY_LAYOUT | The sources that it takes. |

Bytes 9 to 15 hold no field.

## ideatek:InputCaptured

`ideatek:InputCaptured` gives a control that became actuated during a capture.
`ideatek_input_captured_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `capture` | `ideatek_entity_t` | 0 | The capture. |
| `device` | `ideatek_entity_t` | 8 | The device of the control. |
| `control` | `uint32_t` | 16 | The control. |
| `order` | `uint32_t` | 20 | The rank in the round. |
| `source` | `ideatek_input_source_t` | 24 | The kind of the control. |

Bytes 25 to 31 hold no field.

## ideatek:ideatek_device_kind_t

`ideatek_device_kind_t` tells the kind of a device, for its prompts.
`ideatek_device_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_DEVICE_KIND_KEYBOARD_MOUSE` | The keyboard and the mouse. |
| 1 | `IDEATEK_DEVICE_KIND_GAMEPAD` | A gamepad. |
| 2 | `IDEATEK_DEVICE_KIND_TOUCH` | The fingers on a screen. |

## ideatek:InputLastDevice

`ideatek:InputLastDevice` names the device that a context used last: *The last device*.
`ideatek_input_last_device_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `device` | `ideatek_entity_t` | 0 | The device. |
| `kind` | `ideatek_device_kind_t` | 8 | Its kind. |

Bytes 9 to 15 hold no field.

The game picks the glyph of a prompt from the binding whose source fits `kind`. A gamepad gives its name in `ideatek:Gamepad` of the input part.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_action_type_t` | `ideatek_action_type_t` | `ideatek_action_type_t_desc` | `IDEATEK_ACTION_TYPE_VALUES` |
| `ideatek:ideatek_accumulate_t` | `ideatek_accumulate_t` | `ideatek_accumulate_t_desc` | `IDEATEK_ACCUMULATE_VALUES` |
| `ideatek:InputAction` | `ideatek_input_action_t` | `ideatek_input_action_t_desc` | `IDEATEK_INPUT_ACTION_FIELDS` |
| `ideatek:ideatek_action_state_t` | `ideatek_action_state_t` | `ideatek_action_state_t_desc` | `IDEATEK_ACTION_STATE_VALUES` |
| `ideatek:ideatek_action_edges_t` | `ideatek_action_edges_t` | `ideatek_action_edges_t_desc` | `IDEATEK_ACTION_EDGES_VALUES` |
| `ideatek:InputActionState` | `ideatek_input_action_state_t` | `ideatek_input_action_state_t_desc` | `IDEATEK_INPUT_ACTION_STATE_FIELDS` |
| `ideatek:ideatek_input_source_t` | `ideatek_input_source_t` | `ideatek_input_source_t_desc` | `IDEATEK_INPUT_SOURCE_VALUES` |
| `ideatek:InputBinding` | `ideatek_input_binding_t` | `ideatek_input_binding_t_desc` | `IDEATEK_INPUT_BINDING_FIELDS` |
| `ideatek:ideatek_dead_zone_kind_t` | `ideatek_dead_zone_kind_t` | `ideatek_dead_zone_kind_t_desc` | `IDEATEK_DEAD_ZONE_KIND_VALUES` |
| `ideatek:InputDeadZone` | `ideatek_input_dead_zone_t` | `ideatek_input_dead_zone_t_desc` | `IDEATEK_INPUT_DEAD_ZONE_FIELDS` |
| `ideatek:InputCurve` | `ideatek_input_curve_t` | `ideatek_input_curve_t_desc` | `IDEATEK_INPUT_CURVE_FIELDS` |
| `ideatek:InputScale` | `ideatek_input_scale_t` | `ideatek_input_scale_t_desc` | `IDEATEK_INPUT_SCALE_FIELDS` |
| `ideatek:ideatek_axes_t` | `ideatek_axes_t` | `ideatek_axes_t_desc` | `IDEATEK_AXES_VALUES` |
| `ideatek:InputNegate` | `ideatek_input_negate_t` | `ideatek_input_negate_t_desc` | `IDEATEK_INPUT_NEGATE_FIELDS` |
| `ideatek:ideatek_swizzle_t` | `ideatek_swizzle_t` | `ideatek_swizzle_t_desc` | `IDEATEK_SWIZZLE_VALUES` |
| `ideatek:InputSwizzle` | `ideatek_input_swizzle_t` | `ideatek_input_swizzle_t_desc` | `IDEATEK_INPUT_SWIZZLE_FIELDS` |
| `ideatek:InputResponseCurve` | `ideatek_input_response_curve_t` | `ideatek_input_response_curve_t_desc` | `IDEATEK_INPUT_RESPONSE_CURVE_FIELDS` |
| `ideatek:InputClamp` | `ideatek_input_clamp_t` | `ideatek_input_clamp_t_desc` | `IDEATEK_INPUT_CLAMP_FIELDS` |
| `ideatek:InputSmooth` | `ideatek_input_smooth_t` | `ideatek_input_smooth_t_desc` | `IDEATEK_INPUT_SMOOTH_FIELDS` |
| `ideatek:InputLinearStep` | `ideatek_input_linear_step_t` | `ideatek_input_linear_step_t_desc` | `IDEATEK_INPUT_LINEAR_STEP_FIELDS` |
| `ideatek:InputPressed` | `ideatek_input_pressed_t` | `ideatek_input_pressed_t_desc` | `IDEATEK_INPUT_PRESSED_FIELDS` |
| `ideatek:InputReleased` | `ideatek_input_released_t` | `ideatek_input_released_t_desc` | `IDEATEK_INPUT_RELEASED_FIELDS` |
| `ideatek:ideatek_hold_flags_t` | `ideatek_hold_flags_t` | `ideatek_hold_flags_t_desc` | `IDEATEK_HOLD_FLAGS_VALUES` |
| `ideatek:InputHold` | `ideatek_input_hold_t` | `ideatek_input_hold_t_desc` | `IDEATEK_INPUT_HOLD_FIELDS` |
| `ideatek:InputTap` | `ideatek_input_tap_t` | `ideatek_input_tap_t_desc` | `IDEATEK_INPUT_TAP_FIELDS` |
| `ideatek:ideatek_pulse_flags_t` | `ideatek_pulse_flags_t` | `ideatek_pulse_flags_t_desc` | `IDEATEK_PULSE_FLAGS_VALUES` |
| `ideatek:InputPulse` | `ideatek_input_pulse_t` | `ideatek_input_pulse_t_desc` | `IDEATEK_INPUT_PULSE_FIELDS` |
| `ideatek:InputHoldRelease` | `ideatek_input_hold_release_t` | `ideatek_input_hold_release_t_desc` | `IDEATEK_INPUT_HOLD_RELEASE_FIELDS` |
| `ideatek:InputMultiTap` | `ideatek_input_multi_tap_t` | `ideatek_input_multi_tap_t_desc` | `IDEATEK_INPUT_MULTI_TAP_FIELDS` |
| `ideatek:InputToggle` | `ideatek_input_toggle_t` | `ideatek_input_toggle_t_desc` | `IDEATEK_INPUT_TOGGLE_FIELDS` |
| `ideatek:InputFlick` | `ideatek_input_flick_t` | `ideatek_input_flick_t_desc` | `IDEATEK_INPUT_FLICK_FIELDS` |
| `ideatek:InputCooldown` | `ideatek_input_cooldown_t` | `ideatek_input_cooldown_t_desc` | `IDEATEK_INPUT_COOLDOWN_FIELDS` |
| `ideatek:ideatek_chord_flags_t` | `ideatek_chord_flags_t` | `ideatek_chord_flags_t_desc` | `IDEATEK_CHORD_FLAGS_VALUES` |
| `ideatek:InputChord` | `ideatek_input_chord_t` | `ideatek_input_chord_t_desc` | `IDEATEK_INPUT_CHORD_FIELDS` |
| `ideatek:InputCombo` | `ideatek_input_combo_t` | `ideatek_input_combo_t_desc` | `IDEATEK_INPUT_COMBO_FIELDS` |
| `ideatek:InputCardinal` | `ideatek_input_cardinal_t` | `ideatek_input_cardinal_t_desc` | `IDEATEK_INPUT_CARDINAL_FIELDS` |
| `ideatek:InputExternal` | `ideatek_input_external_t` | `ideatek_input_external_t_desc` | `IDEATEK_INPUT_EXTERNAL_FIELDS` |
| `ideatek:InputActionOrigins` | `ideatek_input_action_origins_t` | `ideatek_input_action_origins_t_desc` | `IDEATEK_INPUT_ACTION_ORIGINS_FIELDS` |
| `ideatek:ideatek_context_flags_t` | `ideatek_context_flags_t` | `ideatek_context_flags_t_desc` | `IDEATEK_CONTEXT_FLAGS_VALUES` |
| `ideatek:InputContext` | `ideatek_input_context_t` | `ideatek_input_context_t_desc` | `IDEATEK_INPUT_CONTEXT_FIELDS` |
| `ideatek:InputPlayer` | `ideatek_input_player_t` | `ideatek_input_player_t_desc` | `IDEATEK_INPUT_PLAYER_FIELDS` |
| `ideatek:InputDevice` | `ideatek_input_device_t` | `ideatek_input_device_t_desc` | `IDEATEK_INPUT_DEVICE_FIELDS` |
| `ideatek:ideatek_capture_sources_t` | `ideatek_capture_sources_t` | `ideatek_capture_sources_t_desc` | `IDEATEK_CAPTURE_SOURCES_VALUES` |
| `ideatek:InputCapture` | `ideatek_input_capture_t` | `ideatek_input_capture_t_desc` | `IDEATEK_INPUT_CAPTURE_FIELDS` |
| `ideatek:InputCaptured` | `ideatek_input_captured_t` | `ideatek_input_captured_t_desc` | `IDEATEK_INPUT_CAPTURED_FIELDS` |
| `ideatek:ideatek_device_kind_t` | `ideatek_device_kind_t` | `ideatek_device_kind_t_desc` | `IDEATEK_DEVICE_KIND_VALUES` |
| `ideatek:InputLastDevice` | `ideatek_input_last_device_t` | `ideatek_input_last_device_t_desc` | `IDEATEK_INPUT_LAST_DEVICE_FIELDS` |

```text
{ IDEATEK_ACTION_TYPE_BUTTON = 0, IDEATEK_ACTION_TYPE_AXIS_1 = 1, IDEATEK_ACTION_TYPE_AXIS_2 = 2, IDEATEK_ACTION_TYPE_AXIS_3 = 3 }
{ IDEATEK_ACCUMULATE_HIGHEST = 0, IDEATEK_ACCUMULATE_SUM = 1 }
{ bridge_text_t name; ideatek_action_type_t type; ideatek_accumulate_t accumulate; }
{ IDEATEK_ACTION_STATE_IDLE = 0, IDEATEK_ACTION_STATE_ONGOING = 1, IDEATEK_ACTION_STATE_TRIGGERED = 2 }
{ IDEATEK_ACTION_EDGES_STARTED = 0x01, IDEATEK_ACTION_EDGES_TRIGGERED = 0x02, IDEATEK_ACTION_EDGES_CANCELED = 0x04, IDEATEK_ACTION_EDGES_COMPLETED = 0x08 }
{ float x; float y; float z; float held; ideatek_action_state_t state; ideatek_action_edges_t edges; }
{ IDEATEK_INPUT_SOURCE_KEY = 0, IDEATEK_INPUT_SOURCE_KEY_LAYOUT = 1, IDEATEK_INPUT_SOURCE_MOUSE_BUTTON = 2, IDEATEK_INPUT_SOURCE_MOUSE_MOTION = 3, IDEATEK_INPUT_SOURCE_MOUSE_WHEEL = 4, IDEATEK_INPUT_SOURCE_GAMEPAD_BUTTON = 5, IDEATEK_INPUT_SOURCE_GAMEPAD_STICK = 6, IDEATEK_INPUT_SOURCE_GAMEPAD_TRIGGER = 7, IDEATEK_INPUT_SOURCE_XR_BOOLEAN = 8, IDEATEK_INPUT_SOURCE_XR_FLOAT = 9, IDEATEK_INPUT_SOURCE_XR_VECTOR2 = 10, IDEATEK_INPUT_SOURCE_GAMEPAD_GYRO = 11, IDEATEK_INPUT_SOURCE_GAMEPAD_ACCEL = 12, IDEATEK_INPUT_SOURCE_TOUCH = 13, IDEATEK_INPUT_SOURCE_TOUCH_MOTION = 14, IDEATEK_INPUT_SOURCE_EXTERNAL = 15 }
{ ideatek_entity_t context; ideatek_entity_t action; uint32_t control; ideatek_input_source_t source; }
{ IDEATEK_DEAD_ZONE_KIND_AXIAL = 0, IDEATEK_DEAD_ZONE_KIND_RADIAL = 1, IDEATEK_DEAD_ZONE_KIND_UNSCALED_RADIAL = 2 }
{ float lower; float upper; ideatek_dead_zone_kind_t kind; }
{ float x; float y; float z; }
{ float x; float y; float z; }
{ IDEATEK_AXES_X = 0x01, IDEATEK_AXES_Y = 0x02, IDEATEK_AXES_Z = 0x04 }
{ ideatek_axes_t axes; }
{ IDEATEK_SWIZZLE_YXZ = 0, IDEATEK_SWIZZLE_ZYX = 1, IDEATEK_SWIZZLE_XZY = 2, IDEATEK_SWIZZLE_YZX = 3, IDEATEK_SWIZZLE_ZXY = 4 }
{ ideatek_swizzle_t order; }
{ bridge_text_t x_points; bridge_text_t y_points; bridge_text_t z_points; }
{ float min[3]; float max[3]; }
{ float decay_rate; }
{ float accel_rate; float decel_rate; }
{ float threshold; }
{ float threshold; }
{ IDEATEK_HOLD_FLAGS_ONE_SHOT = 0x01 }
{ float threshold; float time; ideatek_hold_flags_t flags; }
{ float threshold; float time; }
{ IDEATEK_PULSE_FLAGS_ON_START = 0x01 }
{ float threshold; float interval; uint32_t limit; ideatek_pulse_flags_t flags; }
{ float threshold; float time; }
{ float threshold; float time; float gap; uint32_t count; }
{ float threshold; }
{ float threshold; float rest; float time; }
{ float threshold; float time; }
{ IDEATEK_CHORD_FLAGS_BLOCK = 0x01 }
{ ideatek_entity_t action; ideatek_chord_flags_t flags; }
{ bridge_text_t steps; bridge_text_t cancels; uint32_t steps_count; uint32_t cancels_count; }
{ uint32_t up; uint32_t down; uint32_t left; uint32_t right; }
{ float x; float y; float z; uint32_t origin; }
{ bridge_text_t origins; uint32_t count; }
{ IDEATEK_CONTEXT_FLAGS_ACTIVE = 0x01, IDEATEK_CONTEXT_FLAGS_CONSUME = 0x02, IDEATEK_CONTEXT_FLAGS_IGNORE_HELD = 0x04, IDEATEK_CONTEXT_FLAGS_REAL_TIME = 0x08 }
{ bridge_text_t name; ideatek_entity_t player; int32_t priority; ideatek_context_flags_t flags; }
{ uint8_t flags; }
{ ideatek_entity_t player; }
{ IDEATEK_CAPTURE_SOURCES_KEY = 0x01, IDEATEK_CAPTURE_SOURCES_KEY_LAYOUT = 0x02, IDEATEK_CAPTURE_SOURCES_MOUSE_BUTTON = 0x04, IDEATEK_CAPTURE_SOURCES_MOUSE_MOTION = 0x08, IDEATEK_CAPTURE_SOURCES_MOUSE_WHEEL = 0x10, IDEATEK_CAPTURE_SOURCES_GAMEPAD_BUTTON = 0x20, IDEATEK_CAPTURE_SOURCES_GAMEPAD_STICK = 0x40, IDEATEK_CAPTURE_SOURCES_GAMEPAD_TRIGGER = 0x80 }
{ ideatek_entity_t player; ideatek_capture_sources_t sources; }
{ ideatek_entity_t capture; ideatek_entity_t device; uint32_t control; uint32_t order; ideatek_input_source_t source; }
{ IDEATEK_DEVICE_KIND_KEYBOARD_MOUSE = 0, IDEATEK_DEVICE_KIND_GAMEPAD = 1, IDEATEK_DEVICE_KIND_TOUCH = 2 }
{ ideatek_entity_t device; ideatek_device_kind_t kind; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_action_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_accumulate_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_action_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_action_edges_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_input_source_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_dead_zone_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_axes_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_swizzle_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_hold_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_pulse_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_chord_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_context_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_capture_sources_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_device_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputAction` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputActionState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputBinding` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputDeadZone` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputCurve` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputScale` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputNegate` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputSwizzle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputResponseCurve` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputClamp` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputSmooth` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputLinearStep` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputPressed` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputReleased` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputHold` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputTap` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputPulse` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputHoldRelease` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputMultiTap` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputToggle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputFlick` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputCooldown` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputChord` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputCombo` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputCardinal` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputExternal` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputActionOrigins` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputContext` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputPlayer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputDevice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputCapture` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputCaptured` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:InputLastDevice` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The actions, the bindings with their modifiers and triggers, the contexts, the players, the devices and the captures | The types of the fields |
| The mapping module | `ideatek:InputActionState`, `ideatek:InputCaptured`, `ideatek:InputLastDevice`, `ideatek:InputActionOrigins` | The types of the game, `ideatek:Tick`, `ideatek:Frame`, `ideatek:InputExternal`, and the types of the fields |
| An external module | `ideatek:InputExternal` | The types of the game, and the types of the fields |

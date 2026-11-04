# include/engine_subsystems/ideatek_xr.h

`include/engine_subsystems/ideatek_xr.h` gives the types of XR: the session, the views, the tracked poses, the input, the hands, the haptics and the passthrough.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the space part: the speeds of a pose are vectors.

## The roles

- The game places the play space with `ideatek:XrOrigin`, gives the OpenXR path of each XR binding, and sends haptics.
- The XR module is a native module on OpenXR. It writes the session, the cameras of the views, the tracked poses, the values of the XR controls, the hands and the play area.
- The render module draws each view as a camera of the rendering part. The mapping module reads the XR controls as the sources of its bindings.

## The session and the views

The XR module writes `ideatek:XrSession`. The game draws and reads the input only in VISIBLE and FOCUSED.
The module spawns 1 camera entity for each view, 2 for stereo, as children of the origin. Each has a `ideatek:Camera` that targets the images of the runtime, and a `ideatek:Projection` of the kind ASYMMETRIC.
It writes `ideatek:ProjectionAngles` and the `ideatek:Transform` of each eye in each frame, at the predicted time of the display.
The XR module waits for the frame of the runtime inside its own run, as a wait for vsync.

## The tracked poses

The module spawns 1 entity for each pose, as a child of the origin, and writes its `ideatek:Transform`. A controller has 2 poses: GRIP and AIM.
The game attaches its models with `ideatek:Parent` of the space part. A pose with no VALID bit keeps its last place.

## The input

A binding of the map of the inputs with an XR source also has `ideatek:XrBindingPath`. At the start of the session, the module makes 1 OpenXR action for each action with XR bindings, and suggests the bindings for each profile.
In each frame, it writes `ideatek:XrControlState` on each XR binding. A change of an XR binding takes effect at the next start of a session, as OpenXR requires.
The runtime of XR rebinds the controls in its own UI.

## The hands

A tracked hand is a skeleton of the animation part. The module spawns 1 entity for each hand, and 26 joint entities with `ideatek:Joint` whose index is the index of the joint in OpenXR.
The module writes the joints as 1 stage of the pose, before the space module. A skinned hand mesh uses `ideatek:Skin` on this skeleton.
The game finds a pinch or a grab from the distances of the tips.

## The haptics

The game attaches `ideatek:XrHaptic` to the GRIP entity of a controller. The module sends the vibration through 1 output action for each hand, and detaches the type.
This is the form of the rumble of a gamepad in the input part.

## The passthrough and the play area

The game chooses a blend mode in `ideatek:XrOrigin`. The module writes the modes that the device supports and the mode in use.
With ALPHA_BLEND, the render module clears the cameras of XR to an alpha of 0, and does not draw the sky. Where the game draws nothing, the real world shows.
When the bounds of STAGE are known, the module writes `ideatek:XrPlayArea` on the origin.

## ideatek:ideatek_xr_space_t

`ideatek_xr_space_t` tells the reference space of the play space.
`ideatek_xr_space_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_XR_SPACE_LOCAL` | The space at the head at the start. OpenXR always has it. |
| 1 | `IDEATEK_XR_SPACE_STAGE` | The space of the room, on its floor. |
| 2 | `IDEATEK_XR_SPACE_LOCAL_FLOOR` | The local space on the floor. |

## ideatek:ideatek_xr_blend_t

`ideatek_xr_blend_t` tells how the images mix with the real world. The values are the values of OpenXR.
`ideatek_xr_blend_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 1 | `IDEATEK_XR_BLEND_OPAQUE` | Only the images of the game. |
| 2 | `IDEATEK_XR_BLEND_ADDITIVE` | The images add to the real world. |
| 3 | `IDEATEK_XR_BLEND_ALPHA_BLEND` | The real world shows where the alpha is 0. |

## ideatek:XrOrigin

`ideatek:XrOrigin` places the play space at the place of its entity.
`ideatek_xr_origin_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `scale` | `float` | 0 | 1 | The factor of the size of the play space. |
| `space` | `ideatek_xr_space_t` | 4 | `IDEATEK_XR_SPACE_LOCAL` | The reference space. |
| `blend` | `ideatek_xr_blend_t` | 5 | `IDEATEK_XR_BLEND_OPAQUE` | The blend mode. |

Bytes 6 to 7 hold no field.

Godot uses a scale of 1. Unreal uses 100 centimeters, which is 1 in meters.

## ideatek:ideatek_xr_session_state_t

`ideatek_xr_session_state_t` tells the state of a session. The values are the values of `XrSessionState`.
`ideatek_xr_session_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_XR_SESSION_STATE_UNKNOWN` | No state yet. |
| 1 | `IDEATEK_XR_SESSION_STATE_IDLE` | The session waits. |
| 2 | `IDEATEK_XR_SESSION_STATE_READY` | The session can start. |
| 3 | `IDEATEK_XR_SESSION_STATE_SYNCHRONIZED` | The frames run, and nothing shows. |
| 4 | `IDEATEK_XR_SESSION_STATE_VISIBLE` | The frames show, with no input. |
| 5 | `IDEATEK_XR_SESSION_STATE_FOCUSED` | The frames show, with the input. |
| 6 | `IDEATEK_XR_SESSION_STATE_STOPPING` | The session stops. |
| 7 | `IDEATEK_XR_SESSION_STATE_LOSS_PENDING` | The device goes away. |
| 8 | `IDEATEK_XR_SESSION_STATE_EXITING` | The runtime ends the session. |

## ideatek:ideatek_xr_blends_t

`ideatek_xr_blends_t` names the blend modes of a device. The bit of a mode is 1 shifted by its value.
`ideatek_xr_blends_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x02 | `IDEATEK_XR_BLENDS_OPAQUE` | The device supports OPAQUE. |
| 0x04 | `IDEATEK_XR_BLENDS_ADDITIVE` | The device supports ADDITIVE. |
| 0x08 | `IDEATEK_XR_BLENDS_ALPHA_BLEND` | The device supports ALPHA_BLEND. |

## ideatek:XrSession

`ideatek:XrSession` is the state of the session. The XR module writes it.
`ideatek_xr_session_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `refresh` | `float` | 0 | The refresh rate, in Hz. |
| `state` | `ideatek_xr_session_state_t` | 4 | The state. |
| `blend` | `ideatek_xr_blend_t` | 5 | The blend mode in use. |
| `blends` | `ideatek_xr_blends_t` | 6 | The blend modes that the device supports. |

Byte 7 holds no field.

## ideatek:ideatek_xr_tracker_kind_t

`ideatek_xr_tracker_kind_t` tells what a pose tracks.
`ideatek_xr_tracker_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_XR_TRACKER_KIND_HEAD` | The head. |
| 1 | `IDEATEK_XR_TRACKER_KIND_CONTROLLER` | A controller. |
| 2 | `IDEATEK_XR_TRACKER_KIND_TRACKER` | An extra tracker, for example on a foot. |

## ideatek:ideatek_xr_hand_t

`ideatek_xr_hand_t` names a hand.
`ideatek_xr_hand_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_XR_HAND_UNKNOWN` | No hand. |
| 1 | `IDEATEK_XR_HAND_LEFT` | The left hand. |
| 2 | `IDEATEK_XR_HAND_RIGHT` | The right hand. |

## ideatek:ideatek_xr_pose_t

`ideatek_xr_pose_t` names a pose of a controller.
`ideatek_xr_pose_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_XR_POSE_GRIP` | Where the hand holds the controller. |
| 1 | `IDEATEK_XR_POSE_AIM` | Where the controller points. |

## ideatek:XrTracker

`ideatek:XrTracker` tells what a tracked pose is.
`ideatek_xr_tracker_t` takes 3 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `kind` | `ideatek_xr_tracker_kind_t` | 0 | What it tracks. |
| `hand` | `ideatek_xr_hand_t` | 1 | The hand. |
| `pose` | `ideatek_xr_pose_t` | 2 | The pose. |

## ideatek:ideatek_xr_location_t

`ideatek_xr_location_t` names the state of a pose. The bits are the bits of `XrSpaceLocationFlags`.
`ideatek_xr_location_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_XR_LOCATION_ORIENTATION_VALID` | The rotation is valid. |
| 0x02 | `IDEATEK_XR_LOCATION_POSITION_VALID` | The position is valid. |
| 0x04 | `IDEATEK_XR_LOCATION_ORIENTATION_TRACKED` | The rotation is tracked, not estimated. |
| 0x08 | `IDEATEK_XR_LOCATION_POSITION_TRACKED` | The position is tracked, not estimated. |

## ideatek:XrPoseState

`ideatek:XrPoseState` is the state and the speed of a pose.
`ideatek_xr_pose_state_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `linear` | `ideatek_vec3_t` | 0 | The linear speed. |
| `angular` | `ideatek_vec3_t` | 12 | The angular speed. |
| `flags` | `ideatek_xr_location_t` | 24 | The state. |

Bytes 25 to 27 hold no field.

## ideatek:XrBindingPath

`ideatek:XrBindingPath` gives the OpenXR path of an XR binding, on the binding entity.
`ideatek_xr_binding_path_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `profile` | `bridge_text_t` | 0 | The interaction profile, for example `/interaction_profiles/valve/index_controller`. |
| `path` | `bridge_text_t` | 8 | The path, for example `/user/hand/right/input/trigger/value`. |

## ideatek:XrControlState

`ideatek:XrControlState` is the value of an XR binding. The XR module writes it.
`ideatek_xr_control_state_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The first value. |
| `y` | `float` | 4 | The second value. |
| `active` | `bool` | 8 | `true` if the runtime gives the control. |

Bytes 9 to 11 hold no field.

## ideatek:XrHand

`ideatek:XrHand` makes an entity a tracked hand: *The hands*.
`ideatek_xr_hand_record_t` takes 2 bytes. Its alignment is 1.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `hand` | `ideatek_xr_hand_t` | 0 | The hand. |
| `active` | `bool` | 1 | `true` while the hand is tracked. |

## ideatek:XrJointState

`ideatek:XrJointState` is the state of a joint of a hand.
`ideatek_xr_joint_state_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `radius` | `float` | 0 | The radius of the joint. |
| `flags` | `ideatek_xr_location_t` | 4 | The state. |

Bytes 5 to 7 hold no field.

## ideatek:XrHaptic

`ideatek:XrHaptic` makes a controller vibrate: *The haptics*.
`ideatek_xr_haptic_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `amplitude` | `float` | 0 | No default | The strength, from 0 to 1. |
| `frequency` | `float` | 4 | 0 | The frequency, in Hz. 0 lets the runtime choose. |
| `duration_ms` | `uint32_t` | 8 | 0 | The length, in ms. 0 means the shortest pulse. |

## ideatek:XrPlayArea

`ideatek:XrPlayArea` is the size of the safe space of the player, in STAGE space.
`ideatek_xr_play_area_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `width` | `float` | 0 | The width of the play area. |
| `depth` | `float` | 4 | The depth of the play area. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_xr_space_t` | `ideatek_xr_space_t` | `ideatek_xr_space_t_desc` | `IDEATEK_XR_SPACE_VALUES` |
| `ideatek:ideatek_xr_blend_t` | `ideatek_xr_blend_t` | `ideatek_xr_blend_t_desc` | `IDEATEK_XR_BLEND_VALUES` |
| `ideatek:XrOrigin` | `ideatek_xr_origin_t` | `ideatek_xr_origin_t_desc` | `IDEATEK_XR_ORIGIN_FIELDS` |
| `ideatek:ideatek_xr_session_state_t` | `ideatek_xr_session_state_t` | `ideatek_xr_session_state_t_desc` | `IDEATEK_XR_SESSION_STATE_VALUES` |
| `ideatek:ideatek_xr_blends_t` | `ideatek_xr_blends_t` | `ideatek_xr_blends_t_desc` | `IDEATEK_XR_BLENDS_VALUES` |
| `ideatek:XrSession` | `ideatek_xr_session_t` | `ideatek_xr_session_t_desc` | `IDEATEK_XR_SESSION_FIELDS` |
| `ideatek:ideatek_xr_tracker_kind_t` | `ideatek_xr_tracker_kind_t` | `ideatek_xr_tracker_kind_t_desc` | `IDEATEK_XR_TRACKER_KIND_VALUES` |
| `ideatek:ideatek_xr_hand_t` | `ideatek_xr_hand_t` | `ideatek_xr_hand_t_desc` | `IDEATEK_XR_HAND_VALUES` |
| `ideatek:ideatek_xr_pose_t` | `ideatek_xr_pose_t` | `ideatek_xr_pose_t_desc` | `IDEATEK_XR_POSE_VALUES` |
| `ideatek:XrTracker` | `ideatek_xr_tracker_t` | `ideatek_xr_tracker_t_desc` | `IDEATEK_XR_TRACKER_FIELDS` |
| `ideatek:ideatek_xr_location_t` | `ideatek_xr_location_t` | `ideatek_xr_location_t_desc` | `IDEATEK_XR_LOCATION_VALUES` |
| `ideatek:XrPoseState` | `ideatek_xr_pose_state_t` | `ideatek_xr_pose_state_t_desc` | `IDEATEK_XR_POSE_STATE_FIELDS` |
| `ideatek:XrBindingPath` | `ideatek_xr_binding_path_t` | `ideatek_xr_binding_path_t_desc` | `IDEATEK_XR_BINDING_PATH_FIELDS` |
| `ideatek:XrControlState` | `ideatek_xr_control_state_t` | `ideatek_xr_control_state_t_desc` | `IDEATEK_XR_CONTROL_STATE_FIELDS` |
| `ideatek:XrHand` | `ideatek_xr_hand_record_t` | `ideatek_xr_hand_record_t_desc` | `IDEATEK_XR_HAND_RECORD_FIELDS` |
| `ideatek:XrJointState` | `ideatek_xr_joint_state_t` | `ideatek_xr_joint_state_t_desc` | `IDEATEK_XR_JOINT_STATE_FIELDS` |
| `ideatek:XrHaptic` | `ideatek_xr_haptic_t` | `ideatek_xr_haptic_t_desc` | `IDEATEK_XR_HAPTIC_FIELDS` |
| `ideatek:XrPlayArea` | `ideatek_xr_play_area_t` | `ideatek_xr_play_area_t_desc` | `IDEATEK_XR_PLAY_AREA_FIELDS` |

```text
{ IDEATEK_XR_SPACE_LOCAL = 0, IDEATEK_XR_SPACE_STAGE = 1, IDEATEK_XR_SPACE_LOCAL_FLOOR = 2 }
{ IDEATEK_XR_BLEND_OPAQUE = 1, IDEATEK_XR_BLEND_ADDITIVE = 2, IDEATEK_XR_BLEND_ALPHA_BLEND = 3 }
{ float scale; ideatek_xr_space_t space; ideatek_xr_blend_t blend; }
{ IDEATEK_XR_SESSION_STATE_UNKNOWN = 0, IDEATEK_XR_SESSION_STATE_IDLE = 1, IDEATEK_XR_SESSION_STATE_READY = 2, IDEATEK_XR_SESSION_STATE_SYNCHRONIZED = 3, IDEATEK_XR_SESSION_STATE_VISIBLE = 4, IDEATEK_XR_SESSION_STATE_FOCUSED = 5, IDEATEK_XR_SESSION_STATE_STOPPING = 6, IDEATEK_XR_SESSION_STATE_LOSS_PENDING = 7, IDEATEK_XR_SESSION_STATE_EXITING = 8 }
{ IDEATEK_XR_BLENDS_OPAQUE = 0x02, IDEATEK_XR_BLENDS_ADDITIVE = 0x04, IDEATEK_XR_BLENDS_ALPHA_BLEND = 0x08 }
{ float refresh; ideatek_xr_session_state_t state; ideatek_xr_blend_t blend; ideatek_xr_blends_t blends; }
{ IDEATEK_XR_TRACKER_KIND_HEAD = 0, IDEATEK_XR_TRACKER_KIND_CONTROLLER = 1, IDEATEK_XR_TRACKER_KIND_TRACKER = 2 }
{ IDEATEK_XR_HAND_UNKNOWN = 0, IDEATEK_XR_HAND_LEFT = 1, IDEATEK_XR_HAND_RIGHT = 2 }
{ IDEATEK_XR_POSE_GRIP = 0, IDEATEK_XR_POSE_AIM = 1 }
{ ideatek_xr_tracker_kind_t kind; ideatek_xr_hand_t hand; ideatek_xr_pose_t pose; }
{ IDEATEK_XR_LOCATION_ORIENTATION_VALID = 0x01, IDEATEK_XR_LOCATION_POSITION_VALID = 0x02, IDEATEK_XR_LOCATION_ORIENTATION_TRACKED = 0x04, IDEATEK_XR_LOCATION_POSITION_TRACKED = 0x08 }
{ ideatek_vec3_t linear; ideatek_vec3_t angular; ideatek_xr_location_t flags; }
{ bridge_text_t profile; bridge_text_t path; }
{ float x; float y; bool active; }
{ ideatek_xr_hand_t hand; bool active; }
{ float radius; ideatek_xr_location_t flags; }
{ float amplitude; float frequency; uint32_t duration_ms; }
{ float width; float depth; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_space_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_blend_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_session_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_blends_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_tracker_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_hand_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_pose_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_xr_location_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrOrigin` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrSession` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrTracker` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrPoseState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrBindingPath` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrControlState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrHand` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrJointState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrHaptic` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:XrPlayArea` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:XrOrigin`, `ideatek:XrBindingPath`, `ideatek:XrHaptic` | The types of the fields |
| The XR module | Its own state types, `ideatek:Transform`, the cameras with their projections, and `ideatek:Joint` | `ideatek:XrOrigin`, `ideatek:XrBindingPath`, `ideatek:XrHaptic`, `ideatek:InputBinding`, and the types of the fields |

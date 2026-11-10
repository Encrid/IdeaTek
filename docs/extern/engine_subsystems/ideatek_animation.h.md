# include/engine_subsystems/ideatek_animation.h

`include/engine_subsystems/ideatek_animation.h` gives the types of the animation: skeletons, skins, morph weights, clips, players, blend spaces, state machines, clip events, root motion, pose modifiers and springs.
`readme.md`, *The contracts*, tells the rules of each contract.
Its fields use `ideatek_entity_t`, `ideatek_vec3_t` and `ideatek_quat_t`, so the header includes the world and space parts.

## The roles

- The game or a loader is the author. It writes the skeletons, the clips, the plays and the modifiers.
- The animation module mixes the plays. It writes `ideatek:Transform` of the joints, `ideatek:MorphWeights`, `ideatek:RootMotionDelta` and `ideatek:ClipEvent`. It also writes `time` of each play.
- An IK module and a spring module write `ideatek:Transform` of the joints that their modifiers name.
- The render module reads `ideatek:Joint`, `ideatek:Skin`, `ideatek:SkinBinds` and `ideatek:MorphWeights`.
- A graph module reads `ideatek:AnimationParameters` and drives the plays: *The graph modules*.
- A matching module reads `ideatek:Trajectory` and drives the plays: *The motion matching*.

## The pose

The joints of a skeleton are entities with `ideatek:Transform`, `ideatek:Parent` and `ideatek:Joint`.
Several stages write the `ideatek:Transform` of a joint in each step, in this order:

1. The animation module mixes the plays and keeps the root joint in place on the axes of `ideatek:RootMotion`.
2. The IK module applies its modifiers by rising `order`.
3. The spring module applies its modifiers by rising `order`.
4. The physics module writes the joints of a ragdoll, in the fixed phase.
5. The space module computes `ideatek:WorldTransform`.

The pairs of the deployment give this order. Each stage reads the `ideatek:Transform` of the stage before. It writes `lerp(input, result, influence)`.
A joint that no stage writes keeps its `ideatek:Transform`. Two meshes with 1 skeleton share its joints.

## The time rate

The animation module advances each play at the time rate of the place of its player: `docs/extern/engine_subsystems/ideatek_physics.h.md`, *The time of an area*.
A player with a body takes `rate` of its `ideatek:Motion`. For another player, the module reads the time areas at the place of the player.

## The names

A target id and the name of a clip event are the FNV-1a hash of 64 bits of a name in UTF-8.
The name of a target is the path of names from the root of the model, with `/` between them.

## The bytes of a clip

`bytes` of `ideatek:Clip` holds `channel_count` records of `ideatek_clip_channel_t`, then `event_count` records of `ideatek_clip_event_t`, then the times and the values.
All numbers are little-endian. A reader copies each record with `memcpy`.

The times of a channel are `key_count` floats from `times`. The values start at `values`.
A value of `IDEATEK_CHANNEL_PROPERTY_WEIGHTS` holds all the morph weights of its key, in the order of the targets.
`IDEATEK_INTERPOLATION_CUBIC_SPLINE` holds an in-tangent, a value and an out-tangent for each key, as in glTF 2.0.

## The mix

The animation module mixes the plays of a player layer by layer, by rising `layer`.
In 1 layer, each play counts by `weight` divided by the sum of the weights of the layer. A mask scales the weight of each target.
It mixes the rotations as a normalized sum of quaternions with their weights. A target with a sum of 0 is not written.
The weight of a layer is the sum of the weights of its plays, at most 1.

- `IDEATEK_LAYER_BLEND_OVERLAY` blends the pose below toward the mix of the layer, by the weight of the layer.
- `IDEATEK_LAYER_BLEND_ADDITIVE` adds the offsets of the layer to the pose below, scaled by the weight of the layer.
- `IDEATEK_LAYER_BLEND_MODEL_SPACE` is an overlay of the rotations in the space of the skeleton, so an aim stays true when the spine turns.

An offset adds to a translation, and multiplies a rotation and a scale. The first layer of a player is always an overlay on the pose of the joints.
A play of `IDEATEK_LAYER_BLEND_ADDITIVE` plays only a clip with `additive`. Another clip gets 1 line in the log, and the play is skipped.
The plays of 1 layer have 1 blend. A layer whose plays differ gets 1 line in the log, and it is an overlay.

A play with `IDEATEK_REPEAT_ONCE` stops at its end and stays there.
A play with `real_time` takes `real_delta` of `ideatek:Frame`. Each other play takes `delta`.

## The bone masks

A bone mask gives a weight from 0 to 1 to each target of a skeleton. A target that the mask does not name has the weight 0.
A record of `weights` of `ideatek:BoneMask`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `target` | `uint64_t` | 0 | The target id. |
| `weight` | `float` | 8 | The weight of the target. |

A play with a `mask` of 0 has the weight 1 on each target. For example, a mask of the upper body lets a layer aim while the legs run.

## The sync groups

The plays of 1 player with the same `sync_group`, other than 0, keep the same phase. The play with the highest weight leads.
The phase of a play is the last event of the kind `IDEATEK_CLIP_EVENT_KIND_SYNC` that it passed. It also holds the part of the time to the next one.
Each other play of the group takes the time with the same phase in its own clip. A clip with no sync event uses its normalized time.

A foot that touches the ground is a common sync event, so the feet of a walk and of a run stay in step.

## The curves

A channel of `IDEATEK_CHANNEL_PROPERTY_FLOAT` gives 1 float for each key. Its target id is the hash of the name of the curve.
The animation module mixes the curves of a player as the morph weights. An additive layer adds its curves.
In each step with a change, it attaches `ideatek:AnimationCurves` to the player. The module that owns a value reads its curve and applies it.

A loader of glTF names each float of `KHR_animation_pointer` by its JSON pointer, for example the intensity of a light.

## The graph modules

A graph module runs a graph of animation, a resource of its own type, for example a graph of Esoterica.
The game writes `ideatek:AnimationParameters` on the player in each step that a value changes. The graph module reads it.
The graph module then spawns and writes the plays, the positions of the blend spaces and `ideatek:Inertialize`.
A record of `values` of `ideatek:AnimationParameters`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `name` | `uint64_t` | 0 | The hash of the name of the parameter. |
| `id` | `uint64_t` | 8 | With `IDEATEK_PARAMETER_TYPE_ID`, a hash. With `IDEATEK_PARAMETER_TYPE_TARGET`, the `id` of an entity. Else 0. |
| `value` | `float[3]` | 16 | With `IDEATEK_PARAMETER_TYPE_FLOAT`, the first float. With `IDEATEK_PARAMETER_TYPE_VECTOR`, the 3 floats. With `IDEATEK_PARAMETER_TYPE_BOOL`, 0 or 1 in the first float. |
| `type` | `ideatek_parameter_type_t` | 28 | The type of the value. |

## The warping

A warp moves the root motion of a play, so the character reaches a place, for example a ledge or a door.
The game writes `ideatek:WarpTargets` on the player. A record of `targets`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `name` | `uint64_t` | 0 | The hash of the name of the target. |
| `follow` | `uint64_t` | 8 | The `id` of an entity that the target follows, or 0. |
| `position` | `double[3]` | 16 | The position in the world, or the offset from `follow`. |
| `rotation` | `float[4]` | 40 | The rotation in the world, or the offset from `follow`. |

The window of a warp is a clip event of the kind `IDEATEK_CLIP_EVENT_KIND_WARP`, whose name is the name of a target.
During the window, the animation module bends `ideatek:RootMotionDelta`, so the root reaches the target at the end of the window.
With `ideatek:OrientationWarp`, it turns the lower body toward `direction`, and the upper body keeps the facing of the clip.

## The level of detail

`ideatek:AnimationLod` on a player makes far characters cheaper. The game or the render module writes it by distance.
The animation module samples the player once in each `interval` steps, and keeps the pose between.
With a `joint_count` other than 0, it samples only the joints whose `index` is below `joint_count`. The other joints keep their pose.

## The motion matching

A matching module picks, in each search, the frame of a database whose pose and path best match the character. It then plays that frame.
The database is a resource of a type of the matching module. IdeaTek gives no format of it.
The game writes `ideatek:Trajectory` on the player: the path of the character in the past and in the future. A record of `samples`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `time` | `float` | 0 | The seconds from now. A past sample is negative, and a future sample is positive. |
| `position` | `float[3]` | 4 | The position, relative to the player. |
| `facing` | `float[4]` | 16 | The rotation of the facing, relative to the player. |

The matching module spawns the plays of the frames that it picks, and fades or inertializes them. The game reads the plays as any other.

## The inertial blend

An inertial blend goes from 1 pose to another and plays only the new pose. It keeps the speed of the old motion.
A module attaches `ideatek:Inertialize` to a player at a change, for example a new clip.
In the next step, the animation module keeps the difference between the last pose and the new mix for each target. It also keeps the speed of that difference.
It adds the difference to the pose, and decays it to 0 over `duration` with a quintic curve.

The curve starts at the speed of the difference, and ends with no speed and no acceleration. Then the animation module detaches `ideatek:Inertialize`.
An `ideatek:AnimationState` with `inertial` uses an inertial blend of `fade` seconds in place of a fade.

## The blend spaces

A blend space mixes the clips of its points by its position `x` and `y`.
With 1 dimension, the animation module mixes the 2 nearest points. With 2 dimensions, it mixes the 3 points of the triangle that holds the position.
With `synced`, all the clips share 1 normalized time.
Only the point with the highest weight sends clip events. On a tie, the first point sends them.

## The state machines

A machine of animation is a machine of the states part with `ideatek:AnimationStates`.
Each value of the machine is an entity with `ideatek:AnimationState`. The game writes `ideatek:StateRequest` of the states part.
On `ideatek:StateChange`, the animation module fades the old source into the new source over `fade` seconds. Then it destroys the old plays.

## The clip events

A play sends the events of its clip while its weight is above `event_weight`. It sends each event that its time passes, in the order of time, also over a loop.
A seek by a write of `time` sends no event.
An event with a duration of 0 sends only `IDEATEK_EVENT_PHASE_BEGIN`.
The animation module destroys `ideatek:ClipEvent` in the round after it spawns it.

## The root motion

The animation module writes the mixed motion of the root joint in each step into `ideatek:RootMotionDelta`, in the space of the player.
It never moves the entity. A mover, for example a character controller, reads the delta and writes `ideatek:Transform`.
A root of 0 turns the root motion off. A loop gives no jump.

## The springs

A spring chain moves its joints with Verlet steps of `delta` of `ideatek:Frame`, so it stops in a pause.
A spring collider pushes only the springs. It is not a physics collider.
After a jump of the entity, the game sets `active` of the modifier to `false` and back to `true`. The chain then starts again from the pose.

## The hidden joints

`ideatek:HiddenJoint` on a joint hides the triangles that depend on the joint or on its children, on each skinned mesh of its skeleton. The render module closes the hole with the material `cap`.

## The scenes

A scene of a choreography is a clip that a play on a scene player plays. Its channels move the actors, the cameras and the props by target id.
At the start, the game points `player` of `ideatek:AnimationTarget` of each actor to the scene player. At the end, it points them back.
A play with `ideatek:PlayLink` follows its parent play: its time is the parent time minus `start`. The speed and the pause of the parent apply to it.

## ideatek:ideatek_channel_property_t

`ideatek_channel_property_t` tells what a channel of a clip moves.
`ideatek_channel_property_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CHANNEL_PROPERTY_TRANSLATION` | The position of the target. |
| 1 | `IDEATEK_CHANNEL_PROPERTY_ROTATION` | The rotation, as a quaternion. |
| 2 | `IDEATEK_CHANNEL_PROPERTY_SCALE` | The scale. |
| 3 | `IDEATEK_CHANNEL_PROPERTY_WEIGHTS` | The morph weights. |
| 4 | `IDEATEK_CHANNEL_PROPERTY_FLOAT` | A curve: *The curves*. |

## ideatek:ideatek_interpolation_t

`ideatek_interpolation_t` tells the values between keys, as in glTF 2.0.
`ideatek_interpolation_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_INTERPOLATION_STEP` | The value of the key before. |
| 1 | `IDEATEK_INTERPOLATION_LINEAR` | A straight blend of the 2 keys. |
| 2 | `IDEATEK_INTERPOLATION_CUBIC_SPLINE` | A cubic spline with tangents. |

## ideatek:ideatek_layer_blend_t

`ideatek_layer_blend_t` tells how a layer of plays applies: *The mix*.
`ideatek_layer_blend_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LAYER_BLEND_OVERLAY` | The layer replaces the pose below, by its weight. |
| 1 | `IDEATEK_LAYER_BLEND_ADDITIVE` | The layer adds offsets to the pose below. |
| 2 | `IDEATEK_LAYER_BLEND_MODEL_SPACE` | The layer replaces the rotations in the space of the skeleton. |

## ideatek:ideatek_clip_event_kind_t

`ideatek_clip_event_kind_t` tells the use of an event of a clip.
`ideatek_clip_event_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CLIP_EVENT_KIND_GENERIC` | An event for the game. |
| 1 | `IDEATEK_CLIP_EVENT_KIND_SYNC` | A mark of the phase: *The sync groups*. |
| 2 | `IDEATEK_CLIP_EVENT_KIND_WARP` | The window of a warp: *The warping*. |

## ideatek:ideatek_parameter_type_t

`ideatek_parameter_type_t` tells the type of a parameter: *The graph modules*.
`ideatek_parameter_type_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PARAMETER_TYPE_BOOL` | A truth value. |
| 1 | `IDEATEK_PARAMETER_TYPE_ID` | A hash of a name. |
| 2 | `IDEATEK_PARAMETER_TYPE_FLOAT` | A number. |
| 3 | `IDEATEK_PARAMETER_TYPE_VECTOR` | 3 numbers. |
| 4 | `IDEATEK_PARAMETER_TYPE_TARGET` | An entity. |

## ideatek:ideatek_clip_channel_t

`ideatek_clip_channel_t` is 1 channel of a clip.
`ideatek_clip_channel_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `target` | `uint64_t` | 0 | The target id. |
| `key_count` | `uint32_t` | 8 | The number of keys. |
| `times` | `uint32_t` | 12 | The byte of the first time. |
| `values` | `uint32_t` | 16 | The byte of the first value. |
| `property` | `ideatek_channel_property_t` | 20 | What the channel moves. |
| `interpolation` | `ideatek_interpolation_t` | 21 | The values between keys. |

Bytes 22 to 23 hold no field.

## ideatek:ideatek_clip_event_t

`ideatek_clip_event_t` is 1 event of a clip.
`ideatek_clip_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `name` | `uint64_t` | 0 | The hash of the name. |
| `time` | `float` | 8 | The time of the start. |
| `duration` | `float` | 12 | The length. 0 means a point in time. |
| `value` | `float` | 16 | A value for the game. |
| `kind` | `ideatek_clip_event_kind_t` | 20 | The use of the event. |

Bytes 21 to 23 hold no field.

## ideatek:ideatek_repeat_t

`ideatek_repeat_t` tells what a play does at the end of its clip.
`ideatek_repeat_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_REPEAT_ONCE` | The play stops at the end. |
| 1 | `IDEATEK_REPEAT_LOOP` | The play starts again. |
| 2 | `IDEATEK_REPEAT_PING_PONG` | The play turns back at each end. |

## ideatek:ideatek_state_switch_t

`ideatek_state_switch_t` tells when the fade of a state change starts.
`ideatek_state_switch_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_STATE_SWITCH_IMMEDIATE` | The fade starts at the change. |
| 1 | `IDEATEK_STATE_SWITCH_AT_END` | The fade starts at the end of the old clip. |

## ideatek:ideatek_event_phase_t

`ideatek_event_phase_t` tells which edge of an event the time passes.
`ideatek_event_phase_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_EVENT_PHASE_BEGIN` | The time of the play passes the start. |
| 1 | `IDEATEK_EVENT_PHASE_END` | The time of the play passes the end. |

## ideatek:ideatek_motion_axes_t

`ideatek_motion_axes_t` names the parts of a motion.
`ideatek_motion_axes_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_MOTION_AXES_X` | The move on X. |
| 0x02 | `IDEATEK_MOTION_AXES_Y` | The move on Y. |
| 0x04 | `IDEATEK_MOTION_AXES_Z` | The move on Z. |
| 0x08 | `IDEATEK_MOTION_AXES_YAW` | The turn about Y. |
| 0x10 | `IDEATEK_MOTION_AXES_PITCH` | The turn about X. |
| 0x20 | `IDEATEK_MOTION_AXES_ROLL` | The turn about Z. |

## ideatek:ideatek_axis_t

`ideatek_axis_t` names an axis of a joint.
`ideatek_axis_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_AXIS_POSITIVE_X` | +X. |
| 1 | `IDEATEK_AXIS_NEGATIVE_X` | -X. |
| 2 | `IDEATEK_AXIS_POSITIVE_Y` | +Y. |
| 3 | `IDEATEK_AXIS_NEGATIVE_Y` | -Y. |
| 4 | `IDEATEK_AXIS_POSITIVE_Z` | +Z. |
| 5 | `IDEATEK_AXIS_NEGATIVE_Z` | -Z. |

## ideatek:ideatek_collider_shape_t

`ideatek_collider_shape_t` tells the shape of a spring collider.
`ideatek_collider_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_COLLIDER_SHAPE_SPHERE` | A sphere of `radius`. |
| 1 | `IDEATEK_COLLIDER_SHAPE_CAPSULE` | A capsule of `radius` and `height` along Y. |
| 2 | `IDEATEK_COLLIDER_SHAPE_PLANE` | The plane of the Y axis of the entity. |

## ideatek:Joint

`ideatek:Joint` makes an entity a joint of a skeleton.
`ideatek_joint_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `skeleton` | `ideatek_entity_t` | 0 | The skeleton: the entity with `ideatek:AnimationPlayer`. |
| `index` | `uint32_t` | 8 | The rank of the joint in the skeleton. |

Bytes 12 to 15 hold no field.

An entity that follows a joint, for example a sword in a hand, is a child of the joint by `ideatek:Parent`.

## ideatek:Skin

`ideatek:Skin` binds a drawn mesh to a skeleton.
`ideatek_skin_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `skeleton` | `ideatek_entity_t` | 0 | The skeleton that moves the mesh. |
| `binds` | `ideatek_entity_t` | 8 | The resource entity with `ideatek:SkinBinds`. |

The streams `IDEATEK_SEMANTIC_JOINTS` and `IDEATEK_SEMANTIC_WEIGHTS` of the mesh come in sets of 4. A mesh can have any number of sets.

## ideatek:SkinBinds

`ideatek:SkinBinds` holds the inverse bind matrices of a skin, on a resource entity.
`ideatek_skin_binds_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `matrices` | `bridge_text_t` | 0 | An inverse bind matrix of 16 floats for each joint, by column, in the order of the joints. |
| `count` | `uint32_t` | 8 | The number of joints. |
| `revision` | `uint32_t` | 12 | Raised at each change. |

A matrix is relative to the drawn entity.

## ideatek:MorphWeights

`ideatek:MorphWeights` gives the weight of each morph target of a drawn mesh.
`ideatek_morph_weights_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `weights` | `bridge_text_t` | 0 | A float for each morph target, in the order of the targets. |
| `count` | `uint32_t` | 8 | The number of weights. |

An entity with no `ideatek:MorphWeights` has all weights at 0. A count that differs from the mesh gets 1 line in the log.

## ideatek:HiddenJoint

`ideatek:HiddenJoint` hides a joint and its children on each skinned mesh: *The hidden joints*.
`ideatek_hidden_joint_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `cap` | `ideatek_entity_t` | 0 | The material of the cap. 0 means the material of the surface. |

To show the joint again, the game removes the type.

## ideatek:Clip

`ideatek:Clip` is an animation on a resource entity.
`ideatek_clip_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The tables and the keys: *The bytes of a clip*. |
| `duration` | `float` | 8 | The length of the clip. |
| `channel_count` | `uint32_t` | 12 | The number of channels. |
| `event_count` | `uint32_t` | 16 | The number of events. |
| `revision` | `uint32_t` | 20 | Raised at each change. |
| `additive` | `bool` | 24 | `true` if the values are offsets from the reference pose of the clip. |

Bytes 25 to 27 hold no field.

A channel with no target in the player is skipped. The animation module logs the clip once.

## ideatek:AnimationTarget

`ideatek:AnimationTarget` makes an entity a target of the channels of a player.
`ideatek_animation_target_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The target id: *The names*. |
| `player` | `ideatek_entity_t` | 8 | The player that moves the entity. |

## ideatek:AnimationPlayer

`ideatek:AnimationPlayer` is a tag. It makes an entity a player of clips and the root of its skeleton.

## ideatek:ClipPlay

`ideatek:ClipPlay` plays a clip on a player. It is on an entity of its own.
`ideatek_clip_play_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | No default | The player. |
| `clip` | `ideatek_entity_t` | 8 | No default | The clip. |
| `mask` | `ideatek_entity_t` | 16 | 0 | The resource entity of a bone mask, or 0. |
| `sync_group` | `uint64_t` | 24 | 0 | The hash of the name of a sync group, or 0. |
| `time` | `float` | 32 | 0 | The time in the clip. The animation module advances it. |
| `speed` | `float` | 36 | 1 | The factor of the time. |
| `weight` | `float` | 40 | 1 | The part of the play in the mix. |
| `event_weight` | `float` | 44 | 0 | The weight above which the play sends events. |
| `layer` | `int32_t` | 48 | 0 | The layer of the play: *The mix*. |
| `repeat` | `ideatek_repeat_t` | 52 | `IDEATEK_REPEAT_ONCE` | The end of the clip. |
| `paused` | `bool` | 53 | `false` | `true` if the time stays. |
| `real_time` | `bool` | 54 | `false` | `true` if the play takes the real time. |
| `blend` | `ideatek_layer_blend_t` | 55 | `IDEATEK_LAYER_BLEND_OVERLAY` | How the layer of the play applies. |

The game, or a state machine, spawns a play, writes it, and destroys it. The game writes `time` to seek.

## ideatek:BlendSpace

`ideatek:BlendSpace` mixes clips by a position: *The blend spaces*.
`ideatek_blend_space_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | The player. |
| `x` | `float` | 8 | The position on the first dimension. |
| `y` | `float` | 12 | The position on the second dimension. |
| `dimensions` | `uint8_t` | 16 | 1 or 2. |
| `synced` | `bool` | 17 | `true` if the clips share 1 normalized time. |

Bytes 18 to 23 hold no field.

## ideatek:BlendPoint

`ideatek:BlendPoint` is 1 point of a blend space, on an entity of its own.
`ideatek_blend_point_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `space` | `ideatek_entity_t` | 0 | The blend space. |
| `clip` | `ideatek_entity_t` | 8 | The clip of the point. |
| `x` | `float` | 16 | The position of the point on the first dimension. |
| `y` | `float` | 20 | The position on the second dimension. |

## ideatek:AnimationStates

`ideatek:AnimationStates` makes a machine of the states part drive a player.
`ideatek_animation_states_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | The player that the machine drives. |

## ideatek:AnimationState

`ideatek:AnimationState` is 1 value of a machine of animation, on an entity of its own.
`ideatek_animation_state_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `machine` | `ideatek_entity_t` | 0 | The machine. |
| `source` | `ideatek_entity_t` | 8 | A clip or a blend space. |
| `value` | `uint32_t` | 16 | The value of the machine for this state. |
| `fade` | `float` | 20 | The seconds of the fade into this state. |
| `when` | `ideatek_state_switch_t` | 24 | When the fade starts. |
| `inertial` | `bool` | 25 | `true` for an inertial blend: *The inertial blend*. |

Bytes 26 to 31 hold no field.

## ideatek:ClipEvent

`ideatek:ClipEvent` tells that a play passed an event of its clip. It is an event of 1 round.
`ideatek_clip_event_record_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `play` | `ideatek_entity_t` | 0 | The play that passed the event. |
| `player` | `ideatek_entity_t` | 8 | The player. |
| `name` | `uint64_t` | 16 | The hash of the name. |
| `value` | `float` | 24 | The value of the event. |
| `order` | `uint32_t` | 28 | The rank of the event in the round. |
| `phase` | `ideatek_event_phase_t` | 32 | The edge that the time passed. |

Bytes 33 to 39 hold no field.

## ideatek:RootMotion

`ideatek:RootMotion` takes the motion of a root joint out of the pose, on the entity of a player.
`ideatek_root_motion_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `root` | `uint64_t` | 0 | 0 | The target id of the root joint. 0 turns the root motion off. |
| `axes` | `ideatek_motion_axes_t` | 8 | X, Z and yaw | The parts that the root motion takes. |

Bytes 9 to 15 hold no field.

## ideatek:RootMotionDelta

`ideatek:RootMotionDelta` is the root motion of the last step. The animation module writes it.
`ideatek_root_motion_delta_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `rotation` | `ideatek_quat_t` | 0 | The turn of the step. |
| `translation` | `ideatek_vec3_t` | 16 | The move of the step, in the space of the player. |

## ideatek:PoseModifier

`ideatek:PoseModifier` changes the pose after the mix. A solver type on the same entity tells how.
`ideatek_pose_modifier_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | No default | The player of the skeleton. |
| `influence` | `float` | 8 | 1 | The part of the result, from 0 to 1. |
| `order` | `int32_t` | 12 | 0 | The rank inside its module. |
| `active` | `bool` | 16 | `true` | `true` if it applies. |

Bytes 17 to 23 hold no field.

## ideatek:TwoBoneIK

`ideatek:TwoBoneIK` bends 2 bones so that the end reaches a target, for example a foot.
`ideatek_two_bone_ik_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `root` | `ideatek_entity_t` | 0 | The first joint of the chain. |
| `end` | `ideatek_entity_t` | 8 | The last joint. |
| `target` | `ideatek_entity_t` | 16 | The entity that the end reaches. |
| `pole` | `ideatek_entity_t` | 24 | The entity that the middle joint points to. 0 keeps the bend plane of the clip. |

## ideatek:ChainIK

`ideatek:ChainIK` bends a chain of any length with FABRIK.
`ideatek_chain_ik_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `root` | `ideatek_entity_t` | 0 | The first joint of the chain. |
| `end` | `ideatek_entity_t` | 8 | The last joint. |
| `target` | `ideatek_entity_t` | 16 | The entity that the end reaches. |
| `iterations` | `uint32_t` | 24 | The passes of FABRIK. |

Bytes 28 to 31 hold no field.

## ideatek:LookAt

`ideatek:LookAt` turns a joint toward a target, for example a head.
`ideatek_look_at_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `joint` | `ideatek_entity_t` | 0 | No default | The joint that turns. |
| `target` | `ideatek_entity_t` | 8 | No default | The entity to look at. |
| `max_angle` | `float` | 16 | No default | The largest turn from the pose. |
| `forward` | `ideatek_axis_t` | 20 | `IDEATEK_AXIS_POSITIVE_Z` | The axis of the joint that points to the target. |

Bytes 21 to 23 hold no field.

## ideatek:CopyTransform

`ideatek:CopyTransform` gives a joint parts of the place of another entity.
`ideatek_copy_transform_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `joint` | `ideatek_entity_t` | 0 | The joint that takes the place. |
| `source` | `ideatek_entity_t` | 8 | The entity that gives it. |
| `axes` | `ideatek_motion_axes_t` | 16 | The parts that the joint takes. |

Bytes 17 to 23 hold no field.

## ideatek:SpringChain

`ideatek:SpringChain` makes a chain of joints swing, for example hair or a tail.
`ideatek_spring_chain_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `root` | `ideatek_entity_t` | 0 | No default | The first joint of the chain. |
| `end` | `ideatek_entity_t` | 8 | No default | The last joint. |
| `center` | `ideatek_entity_t` | 16 | 0 | The entity whose motion the springs ignore. 0 means the world. |
| `gravity_direction` | `ideatek_vec3_t` | 24 | 0, -1, 0 | The direction of the gravity of the springs. |
| `stiffness` | `float` | 36 | 1 | The pull back to the pose. |
| `drag` | `float` | 40 | 0.5 | The loss of speed. |
| `gravity` | `float` | 44 | 0 | The strength of the gravity of the springs. |
| `radius` | `float` | 48 | 0 | The radius of each joint against the colliders. |

Bytes 52 to 55 hold no field.

The defaults are the defaults of a joint of the `VRMC_springBone` 1.0 schema.

## ideatek:SpringCollider

`ideatek:SpringCollider` keeps the springs of a chain out of a shape, at the place of its entity.
`ideatek_spring_collider_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `modifier` | `ideatek_entity_t` | 0 | The entity of the spring chain. |
| `radius` | `float` | 8 | The radius. |
| `height` | `float` | 12 | The height of a capsule. |
| `shape` | `ideatek_collider_shape_t` | 16 | The shape. |

Bytes 17 to 23 hold no field.

## ideatek:BoneMask

`ideatek:BoneMask` gives a weight to each target, on a resource entity.
`ideatek_bone_mask_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `weights` | `bridge_text_t` | 0 | The records of the weights: *The bone masks*. |
| `count` | `uint32_t` | 8 | The number of records. |
| `revision` | `uint32_t` | 12 | Raised at each change. |

## ideatek:AnimationCurves

`ideatek:AnimationCurves` gives the mixed curves of a player: *The curves*. The animation module writes it.
`ideatek_animation_curves_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `values` | `bridge_text_t` | 0 | A record of a `uint64_t` hash and a `float` value for each curve, 12 bytes. |
| `count` | `uint32_t` | 8 | The number of records. |

## ideatek:AnimationParameters

`ideatek:AnimationParameters` gives the values that the game feeds to a graph module, on the player.
`ideatek_animation_parameters_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `values` | `bridge_text_t` | 0 | The records of the parameters: *The graph modules*. |
| `count` | `uint32_t` | 8 | The number of records. |

## ideatek:WarpTargets

`ideatek:WarpTargets` gives the places that the warps of a player reach.
`ideatek_warp_targets_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `targets` | `bridge_text_t` | 0 | The records of the targets: *The warping*. |
| `count` | `uint32_t` | 8 | The number of records. |

## ideatek:OrientationWarp

`ideatek:OrientationWarp` turns the legs toward the motion: *The warping*.
`ideatek_orientation_warp_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `direction` | `ideatek_vec3_t` | 0 | No default | The direction of the motion, in the space of the player. |
| `max_angle` | `float` | 12 | 1.5708 | The largest turn of the lower body, in radians. |

## ideatek:AnimationLod

`ideatek:AnimationLod` sets the level of detail of a player: *The level of detail*.
`ideatek_animation_lod_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `joint_count` | `uint32_t` | 0 | 0 | The joints that the animation module samples. 0 means all. |
| `interval` | `uint8_t` | 4 | 1 | The steps between 2 samples. 1 samples each step. |

Bytes 5 to 7 hold no field.

## ideatek:Trajectory

`ideatek:Trajectory` gives the path of a character to a matching module.
`ideatek_trajectory_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `samples` | `bridge_text_t` | 0 | The records of the samples, by rising `time`: *The motion matching*. |
| `count` | `uint32_t` | 8 | The number of records. |

## ideatek:MotionMatch

`ideatek:MotionMatch` makes a matching module drive a player, on an entity of its own.
`ideatek_motion_match_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `player` | `ideatek_entity_t` | 0 | No default | The player. |
| `database` | `ideatek_entity_t` | 8 | No default | The resource entity of the database. |
| `blend_time` | `float` | 16 | 0.2 | The seconds of the blend to a new frame. |
| `search_interval` | `float` | 20 | 0.1 | The seconds between 2 searches. |
| `inertial` | `bool` | 24 | `true` | `true` for an inertial blend, else a fade. |

Bytes 25 to 31 hold no field.

## ideatek:Inertialize

`ideatek:Inertialize` asks for an inertial blend on a player: *The inertial blend*.
`ideatek_inertialize_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `duration` | `float` | 0 | The seconds of the blend. |

## ideatek:PlayLink

`ideatek:PlayLink` makes a play follow another play: *The scenes*.
`ideatek_play_link_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `parent` | `ideatek_entity_t` | 0 | The play that this play follows. |
| `start` | `float` | 8 | The time of the parent at which this play starts. |

Bytes 12 to 15 hold no field.

Before `start`, the play has a weight of 0.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_channel_property_t` | `ideatek_channel_property_t` | `ideatek_channel_property_t_desc` | `IDEATEK_CHANNEL_PROPERTY_VALUES` |
| `ideatek:ideatek_interpolation_t` | `ideatek_interpolation_t` | `ideatek_interpolation_t_desc` | `IDEATEK_INTERPOLATION_VALUES` |
| `ideatek:ideatek_layer_blend_t` | `ideatek_layer_blend_t` | `ideatek_layer_blend_t_desc` | `IDEATEK_LAYER_BLEND_VALUES` |
| `ideatek:ideatek_clip_event_kind_t` | `ideatek_clip_event_kind_t` | `ideatek_clip_event_kind_t_desc` | `IDEATEK_CLIP_EVENT_KIND_VALUES` |
| `ideatek:ideatek_parameter_type_t` | `ideatek_parameter_type_t` | `ideatek_parameter_type_t_desc` | `IDEATEK_PARAMETER_TYPE_VALUES` |
| `ideatek:ideatek_clip_channel_t` | `ideatek_clip_channel_t` | `ideatek_clip_channel_t_desc` | `IDEATEK_CLIP_CHANNEL_FIELDS` |
| `ideatek:ideatek_clip_event_t` | `ideatek_clip_event_t` | `ideatek_clip_event_t_desc` | `IDEATEK_CLIP_EVENT_FIELDS` |
| `ideatek:ideatek_repeat_t` | `ideatek_repeat_t` | `ideatek_repeat_t_desc` | `IDEATEK_REPEAT_VALUES` |
| `ideatek:ideatek_state_switch_t` | `ideatek_state_switch_t` | `ideatek_state_switch_t_desc` | `IDEATEK_STATE_SWITCH_VALUES` |
| `ideatek:ideatek_event_phase_t` | `ideatek_event_phase_t` | `ideatek_event_phase_t_desc` | `IDEATEK_EVENT_PHASE_VALUES` |
| `ideatek:ideatek_motion_axes_t` | `ideatek_motion_axes_t` | `ideatek_motion_axes_t_desc` | `IDEATEK_MOTION_AXES_VALUES` |
| `ideatek:ideatek_axis_t` | `ideatek_axis_t` | `ideatek_axis_t_desc` | `IDEATEK_AXIS_VALUES` |
| `ideatek:ideatek_collider_shape_t` | `ideatek_collider_shape_t` | `ideatek_collider_shape_t_desc` | `IDEATEK_COLLIDER_SHAPE_VALUES` |
| `ideatek:Joint` | `ideatek_joint_t` | `ideatek_joint_t_desc` | `IDEATEK_JOINT_FIELDS` |
| `ideatek:Skin` | `ideatek_skin_t` | `ideatek_skin_t_desc` | `IDEATEK_SKIN_FIELDS` |
| `ideatek:SkinBinds` | `ideatek_skin_binds_t` | `ideatek_skin_binds_t_desc` | `IDEATEK_SKIN_BINDS_FIELDS` |
| `ideatek:MorphWeights` | `ideatek_morph_weights_t` | `ideatek_morph_weights_t_desc` | `IDEATEK_MORPH_WEIGHTS_FIELDS` |
| `ideatek:HiddenJoint` | `ideatek_hidden_joint_t` | `ideatek_hidden_joint_t_desc` | `IDEATEK_HIDDEN_JOINT_FIELDS` |
| `ideatek:Clip` | `ideatek_clip_t` | `ideatek_clip_t_desc` | `IDEATEK_CLIP_FIELDS` |
| `ideatek:AnimationTarget` | `ideatek_animation_target_t` | `ideatek_animation_target_t_desc` | `IDEATEK_ANIMATION_TARGET_FIELDS` |
| `ideatek:ClipPlay` | `ideatek_clip_play_t` | `ideatek_clip_play_t_desc` | `IDEATEK_CLIP_PLAY_FIELDS` |
| `ideatek:BlendSpace` | `ideatek_blend_space_t` | `ideatek_blend_space_t_desc` | `IDEATEK_BLEND_SPACE_FIELDS` |
| `ideatek:BlendPoint` | `ideatek_blend_point_t` | `ideatek_blend_point_t_desc` | `IDEATEK_BLEND_POINT_FIELDS` |
| `ideatek:AnimationStates` | `ideatek_animation_states_t` | `ideatek_animation_states_t_desc` | `IDEATEK_ANIMATION_STATES_FIELDS` |
| `ideatek:AnimationState` | `ideatek_animation_state_t` | `ideatek_animation_state_t_desc` | `IDEATEK_ANIMATION_STATE_FIELDS` |
| `ideatek:ClipEvent` | `ideatek_clip_event_record_t` | `ideatek_clip_event_record_t_desc` | `IDEATEK_CLIP_EVENT_RECORD_FIELDS` |
| `ideatek:RootMotion` | `ideatek_root_motion_t` | `ideatek_root_motion_t_desc` | `IDEATEK_ROOT_MOTION_FIELDS` |
| `ideatek:RootMotionDelta` | `ideatek_root_motion_delta_t` | `ideatek_root_motion_delta_t_desc` | `IDEATEK_ROOT_MOTION_DELTA_FIELDS` |
| `ideatek:PoseModifier` | `ideatek_pose_modifier_t` | `ideatek_pose_modifier_t_desc` | `IDEATEK_POSE_MODIFIER_FIELDS` |
| `ideatek:TwoBoneIK` | `ideatek_two_bone_ik_t` | `ideatek_two_bone_ik_t_desc` | `IDEATEK_TWO_BONE_IK_FIELDS` |
| `ideatek:ChainIK` | `ideatek_chain_ik_t` | `ideatek_chain_ik_t_desc` | `IDEATEK_CHAIN_IK_FIELDS` |
| `ideatek:LookAt` | `ideatek_look_at_t` | `ideatek_look_at_t_desc` | `IDEATEK_LOOK_AT_FIELDS` |
| `ideatek:CopyTransform` | `ideatek_copy_transform_t` | `ideatek_copy_transform_t_desc` | `IDEATEK_COPY_TRANSFORM_FIELDS` |
| `ideatek:SpringChain` | `ideatek_spring_chain_t` | `ideatek_spring_chain_t_desc` | `IDEATEK_SPRING_CHAIN_FIELDS` |
| `ideatek:SpringCollider` | `ideatek_spring_collider_t` | `ideatek_spring_collider_t_desc` | `IDEATEK_SPRING_COLLIDER_FIELDS` |
| `ideatek:BoneMask` | `ideatek_bone_mask_t` | `ideatek_bone_mask_t_desc` | `IDEATEK_BONE_MASK_FIELDS` |
| `ideatek:AnimationCurves` | `ideatek_animation_curves_t` | `ideatek_animation_curves_t_desc` | `IDEATEK_ANIMATION_CURVES_FIELDS` |
| `ideatek:AnimationParameters` | `ideatek_animation_parameters_t` | `ideatek_animation_parameters_t_desc` | `IDEATEK_ANIMATION_PARAMETERS_FIELDS` |
| `ideatek:WarpTargets` | `ideatek_warp_targets_t` | `ideatek_warp_targets_t_desc` | `IDEATEK_WARP_TARGETS_FIELDS` |
| `ideatek:OrientationWarp` | `ideatek_orientation_warp_t` | `ideatek_orientation_warp_t_desc` | `IDEATEK_ORIENTATION_WARP_FIELDS` |
| `ideatek:AnimationLod` | `ideatek_animation_lod_t` | `ideatek_animation_lod_t_desc` | `IDEATEK_ANIMATION_LOD_FIELDS` |
| `ideatek:Trajectory` | `ideatek_trajectory_t` | `ideatek_trajectory_t_desc` | `IDEATEK_TRAJECTORY_FIELDS` |
| `ideatek:MotionMatch` | `ideatek_motion_match_t` | `ideatek_motion_match_t_desc` | `IDEATEK_MOTION_MATCH_FIELDS` |
| `ideatek:Inertialize` | `ideatek_inertialize_t` | `ideatek_inertialize_t_desc` | `IDEATEK_INERTIALIZE_FIELDS` |
| `ideatek:PlayLink` | `ideatek_play_link_t` | `ideatek_play_link_t_desc` | `IDEATEK_PLAY_LINK_FIELDS` |

```text
{ IDEATEK_CHANNEL_PROPERTY_TRANSLATION = 0, IDEATEK_CHANNEL_PROPERTY_ROTATION = 1, IDEATEK_CHANNEL_PROPERTY_SCALE = 2, IDEATEK_CHANNEL_PROPERTY_WEIGHTS = 3, IDEATEK_CHANNEL_PROPERTY_FLOAT = 4 }
{ IDEATEK_INTERPOLATION_STEP = 0, IDEATEK_INTERPOLATION_LINEAR = 1, IDEATEK_INTERPOLATION_CUBIC_SPLINE = 2 }
{ IDEATEK_LAYER_BLEND_OVERLAY = 0, IDEATEK_LAYER_BLEND_ADDITIVE = 1, IDEATEK_LAYER_BLEND_MODEL_SPACE = 2 }
{ IDEATEK_CLIP_EVENT_KIND_GENERIC = 0, IDEATEK_CLIP_EVENT_KIND_SYNC = 1, IDEATEK_CLIP_EVENT_KIND_WARP = 2 }
{ IDEATEK_PARAMETER_TYPE_BOOL = 0, IDEATEK_PARAMETER_TYPE_ID = 1, IDEATEK_PARAMETER_TYPE_FLOAT = 2, IDEATEK_PARAMETER_TYPE_VECTOR = 3, IDEATEK_PARAMETER_TYPE_TARGET = 4 }
{ uint64_t target; uint32_t key_count; uint32_t times; uint32_t values; ideatek_channel_property_t property; ideatek_interpolation_t interpolation; }
{ uint64_t name; float time; float duration; float value; ideatek_clip_event_kind_t kind; }
{ IDEATEK_REPEAT_ONCE = 0, IDEATEK_REPEAT_LOOP = 1, IDEATEK_REPEAT_PING_PONG = 2 }
{ IDEATEK_STATE_SWITCH_IMMEDIATE = 0, IDEATEK_STATE_SWITCH_AT_END = 1 }
{ IDEATEK_EVENT_PHASE_BEGIN = 0, IDEATEK_EVENT_PHASE_END = 1 }
{ IDEATEK_MOTION_AXES_X = 0x01, IDEATEK_MOTION_AXES_Y = 0x02, IDEATEK_MOTION_AXES_Z = 0x04, IDEATEK_MOTION_AXES_YAW = 0x08, IDEATEK_MOTION_AXES_PITCH = 0x10, IDEATEK_MOTION_AXES_ROLL = 0x20 }
{ IDEATEK_AXIS_POSITIVE_X = 0, IDEATEK_AXIS_NEGATIVE_X = 1, IDEATEK_AXIS_POSITIVE_Y = 2, IDEATEK_AXIS_NEGATIVE_Y = 3, IDEATEK_AXIS_POSITIVE_Z = 4, IDEATEK_AXIS_NEGATIVE_Z = 5 }
{ IDEATEK_COLLIDER_SHAPE_SPHERE = 0, IDEATEK_COLLIDER_SHAPE_CAPSULE = 1, IDEATEK_COLLIDER_SHAPE_PLANE = 2 }
{ ideatek_entity_t skeleton; uint32_t index; }
{ ideatek_entity_t skeleton; ideatek_entity_t binds; }
{ bridge_text_t matrices; uint32_t count; uint32_t revision; }
{ bridge_text_t weights; uint32_t count; }
{ ideatek_entity_t cap; }
{ bridge_text_t bytes; float duration; uint32_t channel_count; uint32_t event_count; uint32_t revision; bool additive; }
{ uint64_t id; ideatek_entity_t player; }
{ ideatek_entity_t player; ideatek_entity_t clip; ideatek_entity_t mask; uint64_t sync_group; float time; float speed; float weight; float event_weight; int32_t layer; ideatek_repeat_t repeat; bool paused; bool real_time; ideatek_layer_blend_t blend; }
{ ideatek_entity_t player; float x; float y; uint8_t dimensions; bool synced; }
{ ideatek_entity_t space; ideatek_entity_t clip; float x; float y; }
{ ideatek_entity_t player; }
{ ideatek_entity_t machine; ideatek_entity_t source; uint32_t value; float fade; ideatek_state_switch_t when; bool inertial; }
{ ideatek_entity_t play; ideatek_entity_t player; uint64_t name; float value; uint32_t order; ideatek_event_phase_t phase; }
{ uint64_t root; ideatek_motion_axes_t axes; }
{ ideatek_quat_t rotation; ideatek_vec3_t translation; }
{ ideatek_entity_t player; float influence; int32_t order; bool active; }
{ ideatek_entity_t root; ideatek_entity_t end; ideatek_entity_t target; ideatek_entity_t pole; }
{ ideatek_entity_t root; ideatek_entity_t end; ideatek_entity_t target; uint32_t iterations; }
{ ideatek_entity_t joint; ideatek_entity_t target; float max_angle; ideatek_axis_t forward; }
{ ideatek_entity_t joint; ideatek_entity_t source; ideatek_motion_axes_t axes; }
{ ideatek_entity_t root; ideatek_entity_t end; ideatek_entity_t center; ideatek_vec3_t gravity_direction; float stiffness; float drag; float gravity; float radius; }
{ ideatek_entity_t modifier; float radius; float height; ideatek_collider_shape_t shape; }
{ bridge_text_t weights; uint32_t count; uint32_t revision; }
{ bridge_text_t values; uint32_t count; }
{ bridge_text_t values; uint32_t count; }
{ bridge_text_t targets; uint32_t count; }
{ ideatek_vec3_t direction; float max_angle; }
{ uint32_t joint_count; uint8_t interval; }
{ bridge_text_t samples; uint32_t count; }
{ ideatek_entity_t player; ideatek_entity_t database; float blend_time; float search_interval; bool inertial; }
{ float duration; }
{ ideatek_entity_t parent; float start; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_quat_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_channel_property_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_interpolation_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_layer_blend_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_clip_event_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_parameter_type_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_clip_channel_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_clip_event_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_repeat_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_state_switch_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_event_phase_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_motion_axes_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_axis_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_collider_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Joint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Skin` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SkinBinds` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MorphWeights` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:HiddenJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Clip` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AnimationTarget` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AnimationPlayer` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ClipPlay` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BlendSpace` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BlendPoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AnimationStates` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AnimationState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ClipEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:RootMotion` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:RootMotionDelta` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PoseModifier` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TwoBoneIK` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ChainIK` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LookAt` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:CopyTransform` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SpringChain` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SpringCollider` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BoneMask` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AnimationCurves` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AnimationParameters` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WarpTargets` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:OrientationWarp` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AnimationLod` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Trajectory` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MotionMatch` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Inertialize` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PlayLink` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| An author | Each type of the part except the 3 that the animation module writes | The types of the fields |
| The animation module | `ideatek:Transform`, `ideatek:MorphWeights`, `ideatek:RootMotionDelta`, `ideatek:ClipEvent`, `ideatek:AnimationCurves`, `ideatek:Inertialize`, `ideatek:ClipPlay` | The plays, the clips, the blend spaces, the states, the masks, the warps and the levels of detail. Also `ideatek:Frame`, `ideatek:TimeArea`, `ideatek:Motion` and the types of the fields. |
| A graph module | `ideatek:BlendSpace`, `ideatek:Inertialize`, `ideatek:ClipPlay` | `ideatek:AnimationParameters`, `ideatek:Clip`, `ideatek:AnimationCurves`, `ideatek:ClipEvent`, `ideatek:Frame`, and the types of the fields |
| A matching module | `ideatek:Inertialize`, `ideatek:ClipPlay` | `ideatek:MotionMatch`, `ideatek:Trajectory`, `ideatek:Clip`, `ideatek:Joint`, `ideatek:Frame`, and the types of the fields |
| An IK or a spring module | `ideatek:Transform` | The modifiers, `ideatek:Joint`, `ideatek:Frame`, and the types of the fields |
| The render module | No type. | `ideatek:Joint`, `ideatek:Skin`, `ideatek:SkinBinds`, `ideatek:MorphWeights`, `ideatek:HiddenJoint`, and the types of the fields |

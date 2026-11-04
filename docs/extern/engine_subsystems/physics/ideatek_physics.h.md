# include/engine_subsystems/physics/ideatek_physics.h

`include/engine_subsystems/physics/ideatek_physics.h` gives the types of the physics: bodies, shapes, materials, events, queries, joints, vehicles, ragdolls, destruction, cuts, soft bodies, ropes, water and liquids.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the headers of the world and space parts, for the entities, the positions, the vectors and the rotations of its fields.

## The roles

- The game is the author. It writes the bodies, the shapes, the materials, the joints, the queries and the requests.
- The physics module steps the world in the fixed phase. It writes `ideatek:Motion`, the `ideatek:Transform` of each dynamic body, the events, the answers of the queries and the meshes that it makes.
- The render module reads `ideatek:HiddenJoint`, `ideatek:Water` and `ideatek:Wave`.

## The world of the physics

The units are SI: meters, kilograms, seconds, newtons and pascals.
The gravity is a setting of the physics module. The sleep limits, the largest angular speed and the solver are settings of the module too.
The physics module writes `ideatek:Transform` of a dynamic body. A mover writes it for a kinematic body, and nobody writes it for a static body.
The module takes the speed of a kinematic body from the change of its `ideatek:Transform`.

## The shapes

A shape is on its own entity, or on the entity of its body. Its offset is its `ideatek:Transform` relative to its parent.
`IDEATEK_SHAPE_KIND_CONVEX` and `IDEATEK_SHAPE_KIND_TRIANGLES` read the positions of the sub-mesh `part` of an `ideatek:Mesh`. The module builds them again when `revision` changes.
`IDEATEK_SHAPE_KIND_TRIANGLES` is only for a static or a kinematic body. A round kind needs a uniform scale.

`IDEATEK_SHAPE_KIND_PLANE` is only for a static body. Its normal is the Y axis of the shape, and it does not use `size`.

## The filter

2 shapes touch only if `a.touches & b.categories` and `b.touches & a.categories` are both not 0.
A module with fewer bits than 64 logs each bit that it cannot keep once.

## The combination of materials

When 2 materials touch, the larger mode of the 2 wins, in the order AVERAGE, MIN, MULTIPLY, MAX. This is the rule of PhysX and Chaos.
A module whose engine has its own rule mixes the values in that rule.

## The events

An event of the physics is an entity. The physics module spawns it after a step and destroys it after the next step.
`order` is the rank of the event in its step. A reader observes `BRIDGE_ON_ENTER`.
An event of a shape that went away in the same step still names the old entity.

## The queries

A query is an entity that stays. After each step, the physics module answers it with `ideatek:QueryHit` events, so an answer comes 1 step late.
`IDEATEK_QUERY_MODE_ALL` gives at most `max` hits by rising `fraction`. A `max` of 0 has no limit. `IDEATEK_QUERY_MODE_TEST` gives at most 1 hit with only `query` and `shape`.

A ray does not use the shape, and an overlap does not use `translation`. A sensor is a hit only if `touches` has its category.
`IDEATEK_QUERY_KIND_WATER` answers with `ideatek:WaterSample` at `origin`.
A query with a `tick` other than 0 tests against the poses of that tick for each body with `ideatek:LagHistory`. Each other body is at its place of now.

## The joints

A joint is an entity with `ideatek:BodyJoint` and 1 type of a kind, for example `ideatek:HingeJoint`.
Above a threshold, the physics module spawns `ideatek:BodyJointEvent`. With `IDEATEK_JOINT_FLAGS_BREAKABLE`, it also turns the joint off in the same step. The game destroys the entity.
A rope that pulls bodies is a chain of distance joints.
For a limit of an axis of `ideatek:SixDofJoint`, a min equal to the max locks the axis, and a min above the max frees it.

## The vehicles

A vehicle is a dynamic body with `ideatek:Vehicle`. Each wheel is an entity with `ideatek:Wheel`.
The drive torque goes in equal parts to the driven wheels. `forward` scales the largest drive torque, and `right` scales the largest steer angle.
The defaults of a wheel are the defaults of Jolt.

## The ragdolls

A ragdoll is the skeleton itself: each joint entity that falls gets `ideatek:Body`, `ideatek:Shape` and a `ideatek:BodyJoint` to the body of its parent joint.
A kinematic joint follows the animation. The physics module writes the joints that are dynamic, and it writes local places, so the space module stays last.
`influence` of the pose modifier of the physics stage blends the ragdoll with the animation.
`ideatek:PoseDrive` drives a dynamic joint with motors to its animated place, for an active ragdoll.

## The destruction

A body with `IDEATEK_BODY_FLAGS_DESTRUCTIBLE` breaks with no pieces made before.
The game spawns `ideatek:Damage`. Its pressure falls in a straight line to 0 at its radius. Contacts also damage, inside the module.
The strengths of the material tell where the body breaks. The module checks the structure: a part with no path to a static body becomes a piece.

For each piece, the module spawns an entity with `ideatek:Transform`, a dynamic body, a convex shape, an `ideatek:MeshInstance` and a new mesh. Then it spawns `ideatek:BreakEvent`.
A dent rewrites the mesh and raises its `revision`. An explosion is a push. The game owns the life of the pieces.

## The cuts

`ideatek:HiddenJoint` on a joint hides the triangles that depend on the joint or on its children. The cutting module closes the hole with the cap material.
For `ideatek:Cut`, the module never changes a shared mesh. It gives the drawn entity a new mesh. It spawns a new drawn entity for the other side, with its own new mesh on the same joints.

Then it spawns `ideatek:CutEvent`. The game makes the cut joints fall with the ragdoll forms.

## The soft bodies

A soft body is a drawn entity with `ideatek:SoftBody`. The soft body streams of its source mesh pin each vertex to its skinned place.
The physics module skins the source vertices, steps them, and writes its own mesh with positions and normals. It points `ideatek:MeshInstance` at that mesh.

A cloth has pins and a pressure of 0. A closed mesh with a pressure above 0 is a ball or a tire. With no pin stream, the module writes the `ideatek:Transform` of the entity at the center of its vertices.
When the entity jumps, the game spawns `ideatek:SoftBodyReset`.

## The ropes

A rope steps its points in the fixed phase and writes a tube mesh: `sides` around and `segments` along.
A rope never pushes a body. With `IDEATEK_ROPE_FLAGS_COLLIDE`, it rests on the shapes that its filter allows.
An end entity of 0 lets that end hang free. An offset is in the space of its entity.

## The water

Water is an entity with `ideatek:Water` and a sensor box shape. Its surface is the top of the box plus its waves.
Each wave is an entity with `ideatek:Wave`. The render module and the physics module compute the same surface from `elapsed` of `ideatek:Tick`:

```text
k = 2 pi / wavelength
w = sqrt(g k)
phase = k (direction . p) - w t
p.xz = p.xz + steepness / k * direction * cos(phase)
p.y = p.y + amplitude * sin(phase)
```

The sum goes over the waves of the water. A water with no wave is flat. A river is a set of water volumes, each with its own flow.

## The buoyancy

A body with `ideatek:Buoyancy` floats. The force is the density of the water times the volume under the surface times the gravity, at the center of that volume.
The drags act on the speed relative to the flow of the water. They are the factors of Jolt, with no unit.

## The liquids

A liquid is a drawn entity with `ideatek:Liquid` and a sensor box shape: its domain. Liquid that leaves the domain goes away.
An emitter is an entity with `ideatek:LiquidEmitter` and a shape. A rate of 0 fills the shape once.
The module writes the surface as its own mesh. No type names the technique.

## The fields

The physics module reads `ideatek:Wind` and `ideatek:ForceField` of the fields part for each soft body and each rope. A rigid body takes a push instead.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `gravity` | 0, -9.81, 0 | Any vector | The gravity, in m / s^2. |
| `hit_threshold` | 1 | 0 or more | The speed of approach, in m / s, above which a contact gives a hit. |
| `soft_iterations` | 5 | 1 or more | The passes of the solver of the soft bodies. |
| `rope_iterations` | 1 | 1 or more | The passes of the solver of the ropes. |
| `rope_substep` | 0.02 | Above 0 | The longest step of a rope, in seconds. |

## ideatek:ideatek_body_kind_t

`ideatek_body_kind_t` tells who moves a body.
`ideatek_body_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_BODY_KIND_STATIC` | The body does not move. |
| 1 | `IDEATEK_BODY_KIND_KINEMATIC` | A mover moves the body. |
| 2 | `IDEATEK_BODY_KIND_DYNAMIC` | The physics moves the body. |

## ideatek:ideatek_mass_mode_t

`ideatek_mass_mode_t` tells the source of the mass.
`ideatek_mass_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_MASS_MODE_SHAPES` | The mass comes from the shapes and their densities. |
| 1 | `IDEATEK_MASS_MODE_CUSTOM` | The mass comes from `mass`, `center_of_mass` and `inertia`. |

## ideatek:ideatek_locked_t

`ideatek_locked_t` names the parts of the motion of a body that do not move.
`ideatek_locked_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_LOCKED_X` | No move on X. |
| 0x02 | `IDEATEK_LOCKED_Y` | No move on Y. |
| 0x04 | `IDEATEK_LOCKED_Z` | No move on Z. |
| 0x08 | `IDEATEK_LOCKED_ROLL` | No turn about Z. |
| 0x10 | `IDEATEK_LOCKED_PITCH` | No turn about X. |
| 0x20 | `IDEATEK_LOCKED_YAW` | No turn about Y. |

## ideatek:ideatek_body_flags_t

`ideatek_body_flags_t` names the options of a body.
`ideatek_body_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_BODY_FLAGS_CAN_SLEEP` | The body can sleep. |
| 0x02 | `IDEATEK_BODY_FLAGS_CONTINUOUS` | The body does not pass through thin shapes when it is fast. |
| 0x04 | `IDEATEK_BODY_FLAGS_DESTRUCTIBLE` | The body can break: *The destruction*. |

## ideatek:Body

`ideatek:Body` makes an entity a body of the physics.
`ideatek_body_t` takes 44 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `center_of_mass` | `ideatek_vec3_t` | 0 | No default | The center, in the space of the body. |
| `inertia` | `ideatek_vec3_t` | 12 | No default | The inertia on each axis, in kg m^2. |
| `mass` | `float` | 24 | No default | The mass, in kg. |
| `gravity_scale` | `float` | 28 | 1 | The factor of the gravity. |
| `linear_damping` | `float` | 32 | 0 | The loss of linear speed. |
| `angular_damping` | `float` | 36 | 0 | The loss of angular speed. |
| `kind` | `ideatek_body_kind_t` | 40 | No default | Who moves the body. |
| `mass_mode` | `ideatek_mass_mode_t` | 41 | `IDEATEK_MASS_MODE_SHAPES` | The source of the mass. |
| `locked` | `ideatek_locked_t` | 42 | 0 | The parts that do not move. |
| `flags` | `ideatek_body_flags_t` | 43 | `IDEATEK_BODY_FLAGS_CAN_SLEEP` | The options. |

## ideatek:Motion

`ideatek:Motion` is the motion of a body. Only the physics module writes it.
`ideatek_motion_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `linear` | `ideatek_vec3_t` | 0 | The linear speed, in m / s. |
| `angular` | `ideatek_vec3_t` | 12 | The angular speed, in rad / s. |
| `sleeping` | `bool` | 24 | `true` if the body sleeps. |

Bytes 25 to 27 hold no field.

## ideatek:Push

`ideatek:Push` pushes a body. A push wakes the body.
`ideatek_push_t` takes 72 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `point` | `ideatek_position_t` | 0 | The point of the force, in the world. |
| `force` | `ideatek_vec3_t` | 24 | A force at each step, in N. |
| `torque` | `ideatek_vec3_t` | 36 | A torque at each step, in N m. |
| `impulse` | `ideatek_vec3_t` | 48 | An impulse, in N s, once. |
| `angular_impulse` | `ideatek_vec3_t` | 60 | An angular impulse, once. |

The module applies `impulse` and `angular_impulse` once, and then sets them to 0.

## ideatek:ideatek_shape_kind_t

`ideatek_shape_kind_t` tells the kind of a shape.
`ideatek_shape_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SHAPE_KIND_SPHERE` | A sphere: `size.x` is the radius. |
| 1 | `IDEATEK_SHAPE_KIND_BOX` | A box: `size` is the half extents. |
| 2 | `IDEATEK_SHAPE_KIND_CAPSULE` | A capsule along Y: radius `size.x`, half height `size.y`. |
| 3 | `IDEATEK_SHAPE_KIND_CYLINDER` | A cylinder along Y: radius `size.x`, half height `size.y`. |
| 4 | `IDEATEK_SHAPE_KIND_CONVEX` | The convex hull of a sub-mesh. |
| 5 | `IDEATEK_SHAPE_KIND_TRIANGLES` | The triangles of a sub-mesh. |
| 6 | `IDEATEK_SHAPE_KIND_PLANE` | An infinite plane. |

## ideatek:ideatek_shape_flags_t

`ideatek_shape_flags_t` names the options of a shape.
`ideatek_shape_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_SHAPE_FLAGS_SENSOR` | No response and no continuous collision. The mass still counts. |
| 0x02 | `IDEATEK_SHAPE_FLAGS_SENSOR_EVENTS` | The shape gives `ideatek:SensorEvent`. |
| 0x04 | `IDEATEK_SHAPE_FLAGS_CONTACT_EVENTS` | The shape gives `ideatek:ContactEvent`. |
| 0x08 | `IDEATEK_SHAPE_FLAGS_HIT_EVENTS` | The shape gives `ideatek:HitEvent`. |

## ideatek:Shape

`ideatek:Shape` gives a body a part of its form.
`ideatek_shape_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `body` | `ideatek_entity_t` | 0 | No default | The body. |
| `material` | `ideatek_entity_t` | 8 | No default | The physics material. It is not 0. |
| `mesh` | `ideatek_entity_t` | 16 | No default | The mesh of a convex or a triangle shape. |
| `categories` | `uint64_t` | 24 | 1 | The collision categories of the shape. |
| `touches` | `uint64_t` | 32 | All bits | The categories that the shape touches. |
| `size` | `ideatek_vec3_t` | 40 | No default | The size, as the kind tells. |
| `part` | `uint32_t` | 52 | No default | The sub-mesh of a convex or a triangle shape. |
| `kind` | `ideatek_shape_kind_t` | 56 | No default | The kind. |
| `flags` | `ideatek_shape_flags_t` | 57 | 0 | The options. |

Bytes 58 to 63 hold no field.

## ideatek:ideatek_combine_t

`ideatek_combine_t` tells how the values of 2 materials mix.
`ideatek_combine_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_COMBINE_AVERAGE` | The mean of the 2 values. |
| 1 | `IDEATEK_COMBINE_MIN` | The smaller value. |
| 2 | `IDEATEK_COMBINE_MULTIPLY` | The product. |
| 3 | `IDEATEK_COMBINE_MAX` | The larger value. |

## ideatek:PhysicsMaterial

`ideatek:PhysicsMaterial` is the matter of a shape, on a resource entity.
`ideatek_physics_material_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `friction` | `float` | 0 | 0.6 | The friction. |
| `restitution` | `float` | 4 | 0 | The bounce, from 0 to 1. |
| `density` | `float` | 8 | 1000 | The density, in kg / m^3. |
| `tensile` | `float` | 12 | 2e6 | The strength in pull, in Pa. |
| `compression` | `float` | 16 | 20e6 | The strength in push, in Pa. |
| `shear` | `float` | 20 | 6e6 | The strength in shear, in Pa. |
| `friction_combine` | `ideatek_combine_t` | 24 | `IDEATEK_COMBINE_AVERAGE` | The mix of 2 frictions. |
| `restitution_combine` | `ideatek_combine_t` | 25 | `IDEATEK_COMBINE_AVERAGE` | The mix of 2 bounces. |

Bytes 26 to 27 hold no field.

The friction is the median of 5 physics engines. The density is the density of water. The strengths are the defaults of Unreal.

## ideatek:ideatek_touch_kind_t

`ideatek_touch_kind_t` tells the edge of a touch.
`ideatek_touch_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TOUCH_KIND_BEGAN` | The 2 shapes start to touch. |
| 1 | `IDEATEK_TOUCH_KIND_ENDED` | The 2 shapes stop. |

## ideatek:SensorEvent

`ideatek:SensorEvent` tells that a shape enters or leaves a sensor.
`ideatek_sensor_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `sensor` | `ideatek_entity_t` | 0 | The sensor shape. |
| `visitor` | `ideatek_entity_t` | 8 | The shape that comes or goes. |
| `order` | `uint32_t` | 16 | The rank in the step. |
| `kind` | `ideatek_touch_kind_t` | 20 | The edge. |

Bytes 21 to 23 hold no field.

## ideatek:ContactEvent

`ideatek:ContactEvent` tells that 2 shapes start or stop a contact.
`ideatek_contact_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `a` | `ideatek_entity_t` | 0 | The first shape. |
| `b` | `ideatek_entity_t` | 8 | The second shape. |
| `order` | `uint32_t` | 16 | The rank in the step. |
| `kind` | `ideatek_touch_kind_t` | 20 | The edge. |

Bytes 21 to 23 hold no field.

## ideatek:HitEvent

`ideatek:HitEvent` tells that 2 shapes hit faster than `hit_threshold`.
`ideatek_hit_event_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `a` | `ideatek_entity_t` | 0 | The first shape. |
| `b` | `ideatek_entity_t` | 8 | The second shape. |
| `point` | `ideatek_position_t` | 16 | The point of the hit. |
| `normal` | `ideatek_vec3_t` | 40 | The normal from `a` to `b`. |
| `speed` | `float` | 52 | The speed of approach. |
| `order` | `uint32_t` | 56 | The rank in the step. |

Bytes 60 to 63 hold no field.

## ideatek:ideatek_query_kind_t

`ideatek_query_kind_t` tells the kind of a query.
`ideatek_query_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_QUERY_KIND_RAY` | A ray from `origin` along `translation`. |
| 1 | `IDEATEK_QUERY_KIND_SHAPE_CAST` | A shape moved along `translation`. |
| 2 | `IDEATEK_QUERY_KIND_OVERLAP` | The shapes that a shape overlaps. |
| 3 | `IDEATEK_QUERY_KIND_WATER` | The water at `origin`. |

## ideatek:ideatek_query_mode_t

`ideatek_query_mode_t` tells which hits a query gives.
`ideatek_query_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_QUERY_MODE_TEST` | Only if there is a hit. |
| 1 | `IDEATEK_QUERY_MODE_CLOSEST` | The nearest hit. |
| 2 | `IDEATEK_QUERY_MODE_ALL` | Each hit, up to `max`. |

## ideatek:Query

`ideatek:Query` asks the world of the physics a question: *The queries*.
`ideatek_query_t` takes 96 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `origin` | `ideatek_position_t` | 0 | No default | The start, in the world. |
| `categories` | `uint64_t` | 24 | 1 | The categories of the query. |
| `touches` | `uint64_t` | 32 | All bits | The categories that it finds. |
| `rotation` | `ideatek_quat_t` | 40 | No default | The rotation of the shape. |
| `translation` | `ideatek_vec3_t` | 56 | No default | The move of the ray or the shape. |
| `size` | `ideatek_vec3_t` | 68 | No default | The size of the shape. |
| `max` | `uint32_t` | 80 | 0 | The most hits of `IDEATEK_QUERY_MODE_ALL`. |
| `tick` | `uint32_t` | 84 | 0 | The past tick of the test. 0 means now. |
| `kind` | `ideatek_query_kind_t` | 88 | No default | The kind. |
| `shape` | `ideatek_shape_kind_t` | 89 | No default | The kind of the shape. |
| `mode` | `ideatek_query_mode_t` | 90 | No default | The hits. |

Bytes 91 to 95 hold no field.

## ideatek:QueryHit

`ideatek:QueryHit` is 1 answer of a query.
`ideatek_query_hit_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `query` | `ideatek_entity_t` | 0 | The query. |
| `shape` | `ideatek_entity_t` | 8 | The shape hit. |
| `point` | `ideatek_position_t` | 16 | The point of the hit. |
| `normal` | `ideatek_vec3_t` | 40 | The normal of the surface. |
| `fraction` | `float` | 52 | The part of `translation` before the hit. |
| `order` | `uint32_t` | 56 | The rank in the step. |

Bytes 60 to 63 hold no field.

## ideatek:ideatek_joint_flags_t

`ideatek_joint_flags_t` names the options of a joint.
`ideatek_joint_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_JOINT_FLAGS_COLLIDE_CONNECTED` | The 2 bodies collide with each other. |
| 0x02 | `IDEATEK_JOINT_FLAGS_BREAKABLE` | The joint turns off above a threshold. |

## ideatek:BodyJoint

`ideatek:BodyJoint` joins 2 bodies: *The joints*.
`ideatek_body_joint_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `a` | `ideatek_entity_t` | 0 | No default | The first body. |
| `b` | `ideatek_entity_t` | 8 | No default | The second body. |
| `rotation_a` | `ideatek_quat_t` | 16 | No default | The frame of the joint in the space of `a`. |
| `rotation_b` | `ideatek_quat_t` | 32 | No default | The frame in the space of `b`. |
| `position_a` | `ideatek_vec3_t` | 48 | No default | The point of the joint on `a`. |
| `position_b` | `ideatek_vec3_t` | 60 | No default | The point on `b`. |
| `force_threshold` | `float` | 72 | `FLT_MAX` | The force, in N, above which the joint reports. |
| `torque_threshold` | `float` | 76 | `FLT_MAX` | The torque, in N m, above which it reports. |
| `flags` | `ideatek_joint_flags_t` | 80 | 0 | The options. |

Bytes 81 to 87 hold no field.

## ideatek:FixedJoint

`ideatek:FixedJoint` is a tag. It holds the 2 frames of a joint together.

## ideatek:PointJoint

`ideatek:PointJoint` is a tag. It holds the 2 points of a joint together, and lets the bodies turn.

## ideatek:ideatek_joint_features_t

`ideatek_joint_features_t` names the parts of a joint kind that apply.
`ideatek_joint_features_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_JOINT_FEATURES_LIMIT` | The limits apply. |
| 0x02 | `IDEATEK_JOINT_FEATURES_SPRING` | The spring applies. |
| 0x04 | `IDEATEK_JOINT_FEATURES_MOTOR` | The motor applies. |

## ideatek:HingeJoint

`ideatek:HingeJoint` lets the bodies turn about the Z axis of the frames.
`ideatek_hinge_joint_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `lower` | `float` | 0 | The lowest angle. |
| `upper` | `float` | 4 | The highest angle. |
| `spring_hertz` | `float` | 8 | The frequency of the spring. |
| `spring_damping` | `float` | 12 | The damping ratio of the spring. |
| `motor_speed` | `float` | 16 | The speed of the motor, in rad / s. |
| `max_motor_torque` | `float` | 20 | The largest torque of the motor. |
| `features` | `ideatek_joint_features_t` | 24 | The parts that apply. |

Bytes 25 to 27 hold no field.

## ideatek:SliderJoint

`ideatek:SliderJoint` lets the bodies slide along the X axis of the frames.
`ideatek_slider_joint_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `lower` | `float` | 0 | The lowest position. |
| `upper` | `float` | 4 | The highest position. |
| `spring_hertz` | `float` | 8 | The frequency of the spring. |
| `spring_damping` | `float` | 12 | The damping ratio of the spring. |
| `motor_speed` | `float` | 16 | The speed of the motor, in m / s. |
| `max_motor_force` | `float` | 20 | The largest force of the motor. |
| `features` | `ideatek_joint_features_t` | 24 | The parts that apply. |

Bytes 25 to 27 hold no field.

## ideatek:DistanceJoint

`ideatek:DistanceJoint` keeps the 2 points at a distance.
`ideatek_distance_joint_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `length` | `float` | 0 | 1 | The length of rest. |
| `min_length` | `float` | 4 | 0 | The shortest length. |
| `max_length` | `float` | 8 | `FLT_MAX` | The longest length. |
| `spring_hertz` | `float` | 12 | No default | The frequency of the spring. |
| `spring_damping` | `float` | 16 | No default | The damping ratio of the spring. |
| `motor_speed` | `float` | 20 | No default | The speed of the motor, in m / s. |
| `max_motor_force` | `float` | 24 | No default | The largest force of the motor. |
| `features` | `ideatek_joint_features_t` | 28 | No default | The parts that apply. |

Bytes 29 to 31 hold no field.

## ideatek:SwingTwistJoint

`ideatek:SwingTwistJoint` limits a turn to a cone and a twist, for a shoulder.
`ideatek_swing_twist_joint_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `swing_y` | `float` | 0 | The half angle of the cone about Y. |
| `swing_z` | `float` | 4 | The half angle of the cone about Z. |
| `twist_min` | `float` | 8 | The lowest twist about X. |
| `twist_max` | `float` | 12 | The highest twist about X. |

## ideatek:SixDofJoint

`ideatek:SixDofJoint` limits each of the 6 parts of the motion.
`ideatek_six_dof_joint_t` takes 48 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `linear_min` | `ideatek_vec3_t` | 0 | The lowest position on each axis. |
| `linear_max` | `ideatek_vec3_t` | 12 | The highest position on each axis. |
| `angular_min` | `ideatek_vec3_t` | 24 | The lowest angle about each axis. |
| `angular_max` | `ideatek_vec3_t` | 36 | The highest angle about each axis. |

## ideatek:BodyJointEvent

`ideatek:BodyJointEvent` tells that a joint passed a threshold.
`ideatek_body_joint_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `joint` | `ideatek_entity_t` | 0 | The joint. |
| `force` | `float` | 8 | The force of the step. |
| `torque` | `float` | 12 | The torque of the step. |
| `order` | `uint32_t` | 16 | The rank in the step. |

Bytes 20 to 23 hold no field.

## ideatek:Vehicle

`ideatek:Vehicle` makes a dynamic body a vehicle with wheels.
`ideatek_vehicle_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `max_drive_torque` | `float` | 0 | No default | The largest drive torque, in N m. |
| `max_pitch_roll` | `float` | 4 | pi | The largest tilt before the module stops the wheels. |

## ideatek:ideatek_wheel_flags_t

`ideatek_wheel_flags_t` names the roles of a wheel.
`ideatek_wheel_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_WHEEL_FLAGS_DRIVEN` | The wheel takes drive torque. |
| 0x02 | `IDEATEK_WHEEL_FLAGS_STEERS` | The wheel steers. |

## ideatek:Wheel

`ideatek:Wheel` is 1 wheel of a vehicle, on an entity of its own.
`ideatek_wheel_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `vehicle` | `ideatek_entity_t` | 0 | No default | The vehicle. |
| `position` | `ideatek_vec3_t` | 8 | No default | The point of the suspension, in the space of the vehicle. |
| `suspension_direction` | `ideatek_vec3_t` | 20 | 0, -1, 0 | The direction of the suspension. |
| `steering_axis` | `ideatek_vec3_t` | 32 | 0, 1, 0 | The axis of the steer. |
| `radius` | `float` | 44 | 0.3 | The radius. |
| `width` | `float` | 48 | 0.1 | The width. |
| `suspension_min` | `float` | 52 | 0.3 | The shortest suspension. |
| `suspension_max` | `float` | 56 | 0.5 | The longest suspension. |
| `spring_hertz` | `float` | 60 | 1.5 | The frequency of the suspension. |
| `spring_damping` | `float` | 64 | 0.5 | The damping ratio of the suspension. |
| `max_steer` | `float` | 68 | 70 degrees | The largest steer angle. |
| `max_brake_torque` | `float` | 72 | 1500 | The largest brake torque. |
| `max_hand_brake_torque` | `float` | 76 | 4000 | The largest torque of the hand brake. |
| `flags` | `ideatek_wheel_flags_t` | 80 | 0 | The roles. |

Bytes 81 to 87 hold no field.

## ideatek:VehicleInput

`ideatek:VehicleInput` drives a vehicle. The game writes it.
`ideatek_vehicle_input_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `forward` | `float` | 0 | The drive, from -1 to 1. |
| `right` | `float` | 4 | The steer, from -1 to 1. |
| `brake` | `float` | 8 | The brake, from 0 to 1. |
| `hand_brake` | `float` | 12 | The hand brake, from 0 to 1. |

## ideatek:WheelState

`ideatek:WheelState` is the state of a wheel. The physics module writes it.
`ideatek_wheel_state_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `steer_angle` | `float` | 0 | The steer angle. |
| `angle` | `float` | 4 | The turn of the wheel about its axle. |
| `angular_velocity` | `float` | 8 | The speed of the turn. |
| `suspension_length` | `float` | 12 | The length of the suspension. |
| `longitudinal_slip` | `float` | 16 | The slip along the wheel. |
| `lateral_slip` | `float` | 20 | The slip across the wheel. |
| `contact` | `bool` | 24 | `true` if the wheel touches the ground. |

Bytes 25 to 27 hold no field.

## ideatek:PoseDrive

`ideatek:PoseDrive` drives a dynamic joint of a ragdoll to its animated place.
`ideatek_pose_drive_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `position_strength` | `float` | 0 | 0 | The pull to the animated position. |
| `orientation_strength` | `float` | 4 | 0 | The pull to the animated rotation. |

## ideatek:Destructible

`ideatek:Destructible` gives the inside of a body that breaks.
`ideatek_destructible_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `interior` | `ideatek_entity_t` | 0 | The material of the faces inside. 0 means the material of the surface. |

## ideatek:Damage

`ideatek:Damage` damages the bodies near a point. The game spawns it as an event.
`ideatek_damage_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `point` | `ideatek_position_t` | 0 | The center. |
| `radius` | `float` | 24 | The radius. |
| `pressure` | `float` | 28 | The pressure at the center, in Pa. |
| `order` | `uint32_t` | 32 | The rank in the round. |

Bytes 36 to 39 hold no field.

## ideatek:BreakEvent

`ideatek:BreakEvent` tells that a body lost a piece.
`ideatek_break_event_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `body` | `ideatek_entity_t` | 0 | The body that broke. |
| `piece` | `ideatek_entity_t` | 8 | The new piece. |
| `point` | `ideatek_position_t` | 16 | The point of the break. |
| `order` | `uint32_t` | 40 | The rank in the step. |

Bytes 44 to 47 hold no field.

## ideatek:HiddenJoint

`ideatek:HiddenJoint` hides a joint and its children on each skinned mesh.
`ideatek_hidden_joint_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `cap` | `ideatek_entity_t` | 0 | The material of the cap. 0 means the material of the surface. |

To show the joint again, the game removes the type.

## ideatek:Cut

`ideatek:Cut` cuts a skinned mesh at a plane. The game spawns it as an event.
`ideatek_cut_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `drawn` | `ideatek_entity_t` | 0 | The drawn entity to cut. |
| `point` | `ideatek_position_t` | 8 | A point of the plane of the cut. |
| `normal` | `ideatek_vec3_t` | 32 | The normal of the plane. |
| `order` | `uint32_t` | 44 | The rank in the round. |

## ideatek:CutEvent

`ideatek:CutEvent` tells that a cut is done.
`ideatek_cut_event_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `drawn` | `ideatek_entity_t` | 0 | The drawn entity that was cut. |
| `piece` | `ideatek_entity_t` | 8 | The new drawn entity of the other side. |
| `order` | `uint32_t` | 16 | The rank in the step. |

Bytes 20 to 23 hold no field.

## ideatek:SoftBody

`ideatek:SoftBody` makes a drawn entity a cloth or a soft body: *The soft bodies*.
`ideatek_soft_body_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `source` | `ideatek_entity_t` | 0 | No default | The source mesh. |
| `categories` | `uint64_t` | 8 | 1 | The collision categories. |
| `touches` | `uint64_t` | 16 | All bits | The categories that the soft body touches. |
| `edge_compliance` | `float` | 24 | 0 | The give of the edges, in m / N. |
| `shear_compliance` | `float` | 28 | 0 | The give in shear. |
| `bend_compliance` | `float` | 32 | `FLT_MAX` | The give in bend. |
| `vertex_mass` | `float` | 36 | 1 | The mass of each vertex, in kg. |
| `linear_damping` | `float` | 40 | 0.1 | The loss of speed. |
| `friction` | `float` | 44 | 0.2 | The friction. |
| `gravity_scale` | `float` | 48 | 1 | The factor of the gravity. |
| `vertex_radius` | `float` | 52 | 0 | The radius of each vertex. |
| `pressure` | `float` | 56 | 0 | The pressure of a closed mesh, in J, as n R T. |
| `restitution` | `float` | 60 | 0 | The bounce. |

The defaults are the defaults of Jolt. A bend compliance of `FLT_MAX` turns the bend off.

## ideatek:SoftBodyReset

`ideatek:SoftBodyReset` puts a soft body back to its skinned pose. The game spawns it as an event.
`ideatek_soft_body_reset_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `body` | `ideatek_entity_t` | 0 | The soft body. |
| `order` | `uint32_t` | 8 | The rank in the round. |

Bytes 12 to 15 hold no field.

## ideatek:ideatek_rope_flags_t

`ideatek_rope_flags_t` names the options of a rope.
`ideatek_rope_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_ROPE_FLAGS_STIFF` | The rope resists a bend. |
| 0x02 | `IDEATEK_ROPE_FLAGS_COLLIDE` | The rope rests on shapes. |

## ideatek:Rope

`ideatek:Rope` is a cable that hangs and swings, on a drawn entity of its own.
`ideatek_rope_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `start` | `ideatek_entity_t` | 0 | No default | The entity of the start. 0 means free. |
| `end` | `ideatek_entity_t` | 8 | No default | The entity of the end. 0 means free. |
| `categories` | `uint64_t` | 16 | 1 | The collision categories. |
| `touches` | `uint64_t` | 24 | All bits | The categories that the rope rests on. |
| `start_offset` | `ideatek_vec3_t` | 32 | No default | The point on the start entity. |
| `end_offset` | `ideatek_vec3_t` | 44 | No default | The point on the end entity. |
| `length` | `float` | 56 | 1 | The length. |
| `width` | `float` | 60 | 0.1 | The width. |
| `gravity_scale` | `float` | 64 | 1 | The factor of the gravity. |
| `friction` | `float` | 68 | 0.2 | The friction. |
| `segments` | `uint32_t` | 72 | 10 | The parts along the rope. |
| `sides` | `uint32_t` | 76 | 4 | The sides of the tube. |
| `flags` | `ideatek_rope_flags_t` | 80 | 0 | The options. |

Bytes 81 to 87 hold no field.

The defaults are the defaults of the cable of Unreal, in meters.

## ideatek:Water

`ideatek:Water` makes a sensor box a body of water: *The water*.
`ideatek_water_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `flow` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The speed of the water, in m / s. |
| `density` | `float` | 12 | 1000 | The density, in kg / m^3. |

## ideatek:Wave

`ideatek:Wave` is 1 Gerstner wave of a water, on an entity of its own.
`ideatek_wave_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `water` | `ideatek_entity_t` | 0 | No default | The water. |
| `direction` | `ideatek_vec3_t` | 8 | 1, 0, 0 | The direction of travel, in XZ. |
| `wavelength` | `float` | 20 | 0.25 | The length of the wave. |
| `amplitude` | `float` | 24 | 0.1 | The height of the wave. |
| `steepness` | `float` | 28 | 0 | The sharpness of the crests. |

The defaults are the defaults of the Gerstner waves of Unreal, in meters.

## ideatek:WaterSample

`ideatek:WaterSample` answers a query of the kind `IDEATEK_QUERY_KIND_WATER`.
`ideatek_water_sample_t` takes 72 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `query` | `ideatek_entity_t` | 0 | The query. |
| `water` | `ideatek_entity_t` | 8 | The water. |
| `point` | `ideatek_position_t` | 16 | The point of the surface above the origin. |
| `normal` | `ideatek_vec3_t` | 40 | The normal of the surface. |
| `velocity` | `ideatek_vec3_t` | 52 | The speed of the water. |
| `depth` | `float` | 64 | The depth of the origin under the surface. |
| `order` | `uint32_t` | 68 | The rank in the step. |

## ideatek:Buoyancy

`ideatek:Buoyancy` makes a body float in water.
`ideatek_buoyancy_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `linear_drag` | `float` | 0 | 0.5 | The linear drag in the water. |
| `angular_drag` | `float` | 4 | 0.01 | The angular drag in the water. |

## ideatek:ideatek_liquid_flags_t

`ideatek_liquid_flags_t` names the options of a liquid.
`ideatek_liquid_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_LIQUID_FLAGS_PUSH_BODIES` | The liquid pushes the bodies. |

## ideatek:Liquid

`ideatek:Liquid` is a simulated liquid in a domain: *The liquids*.
`ideatek_liquid_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `categories` | `uint64_t` | 0 | 1 | The collision categories. |
| `touches` | `uint64_t` | 8 | All bits | The categories that the liquid touches. |
| `density` | `float` | 16 | 1000 | The density, in kg / m^3. |
| `viscosity` | `float` | 20 | 0.001 | The viscosity, in Pa s. |
| `resolution` | `float` | 24 | 0.1 | The size of the detail, in m. |
| `flags` | `ideatek_liquid_flags_t` | 28 | 0 | The options. |

Bytes 29 to 31 hold no field.

The defaults are the density and the viscosity of water.

## ideatek:LiquidEmitter

`ideatek:LiquidEmitter` adds liquid in the shape of its entity.
`ideatek_liquid_emitter_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `liquid` | `ideatek_entity_t` | 0 | No default | The liquid. |
| `velocity` | `ideatek_vec3_t` | 8 | 0, 0, 0 | The speed of the new liquid. |
| `rate` | `float` | 20 | 0 | The volume for each second, in m^3 / s. 0 fills once. |

## ideatek:LagHistory

`ideatek:LagHistory` keeps the past poses of a body for the queries of a past tick.
`ideatek_lag_history_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `seconds` | `float` | 0 | 0 | The length of the history. |

The default of 0 keeps no history. The game sets the length.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_body_kind_t` | `ideatek_body_kind_t` | `ideatek_body_kind_t_desc` | `IDEATEK_BODY_KIND_VALUES` |
| `ideatek:ideatek_mass_mode_t` | `ideatek_mass_mode_t` | `ideatek_mass_mode_t_desc` | `IDEATEK_MASS_MODE_VALUES` |
| `ideatek:ideatek_locked_t` | `ideatek_locked_t` | `ideatek_locked_t_desc` | `IDEATEK_LOCKED_VALUES` |
| `ideatek:ideatek_body_flags_t` | `ideatek_body_flags_t` | `ideatek_body_flags_t_desc` | `IDEATEK_BODY_FLAGS_VALUES` |
| `ideatek:Body` | `ideatek_body_t` | `ideatek_body_t_desc` | `IDEATEK_BODY_FIELDS` |
| `ideatek:Motion` | `ideatek_motion_t` | `ideatek_motion_t_desc` | `IDEATEK_MOTION_FIELDS` |
| `ideatek:Push` | `ideatek_push_t` | `ideatek_push_t_desc` | `IDEATEK_PUSH_FIELDS` |
| `ideatek:ideatek_shape_kind_t` | `ideatek_shape_kind_t` | `ideatek_shape_kind_t_desc` | `IDEATEK_SHAPE_KIND_VALUES` |
| `ideatek:ideatek_shape_flags_t` | `ideatek_shape_flags_t` | `ideatek_shape_flags_t_desc` | `IDEATEK_SHAPE_FLAGS_VALUES` |
| `ideatek:Shape` | `ideatek_shape_t` | `ideatek_shape_t_desc` | `IDEATEK_SHAPE_FIELDS` |
| `ideatek:ideatek_combine_t` | `ideatek_combine_t` | `ideatek_combine_t_desc` | `IDEATEK_COMBINE_VALUES` |
| `ideatek:PhysicsMaterial` | `ideatek_physics_material_t` | `ideatek_physics_material_t_desc` | `IDEATEK_PHYSICS_MATERIAL_FIELDS` |
| `ideatek:ideatek_touch_kind_t` | `ideatek_touch_kind_t` | `ideatek_touch_kind_t_desc` | `IDEATEK_TOUCH_KIND_VALUES` |
| `ideatek:SensorEvent` | `ideatek_sensor_event_t` | `ideatek_sensor_event_t_desc` | `IDEATEK_SENSOR_EVENT_FIELDS` |
| `ideatek:ContactEvent` | `ideatek_contact_event_t` | `ideatek_contact_event_t_desc` | `IDEATEK_CONTACT_EVENT_FIELDS` |
| `ideatek:HitEvent` | `ideatek_hit_event_t` | `ideatek_hit_event_t_desc` | `IDEATEK_HIT_EVENT_FIELDS` |
| `ideatek:ideatek_query_kind_t` | `ideatek_query_kind_t` | `ideatek_query_kind_t_desc` | `IDEATEK_QUERY_KIND_VALUES` |
| `ideatek:ideatek_query_mode_t` | `ideatek_query_mode_t` | `ideatek_query_mode_t_desc` | `IDEATEK_QUERY_MODE_VALUES` |
| `ideatek:Query` | `ideatek_query_t` | `ideatek_query_t_desc` | `IDEATEK_QUERY_FIELDS` |
| `ideatek:QueryHit` | `ideatek_query_hit_t` | `ideatek_query_hit_t_desc` | `IDEATEK_QUERY_HIT_FIELDS` |
| `ideatek:ideatek_joint_flags_t` | `ideatek_joint_flags_t` | `ideatek_joint_flags_t_desc` | `IDEATEK_JOINT_FLAGS_VALUES` |
| `ideatek:BodyJoint` | `ideatek_body_joint_t` | `ideatek_body_joint_t_desc` | `IDEATEK_BODY_JOINT_FIELDS` |
| `ideatek:ideatek_joint_features_t` | `ideatek_joint_features_t` | `ideatek_joint_features_t_desc` | `IDEATEK_JOINT_FEATURES_VALUES` |
| `ideatek:HingeJoint` | `ideatek_hinge_joint_t` | `ideatek_hinge_joint_t_desc` | `IDEATEK_HINGE_JOINT_FIELDS` |
| `ideatek:SliderJoint` | `ideatek_slider_joint_t` | `ideatek_slider_joint_t_desc` | `IDEATEK_SLIDER_JOINT_FIELDS` |
| `ideatek:DistanceJoint` | `ideatek_distance_joint_t` | `ideatek_distance_joint_t_desc` | `IDEATEK_DISTANCE_JOINT_FIELDS` |
| `ideatek:SwingTwistJoint` | `ideatek_swing_twist_joint_t` | `ideatek_swing_twist_joint_t_desc` | `IDEATEK_SWING_TWIST_JOINT_FIELDS` |
| `ideatek:SixDofJoint` | `ideatek_six_dof_joint_t` | `ideatek_six_dof_joint_t_desc` | `IDEATEK_SIX_DOF_JOINT_FIELDS` |
| `ideatek:BodyJointEvent` | `ideatek_body_joint_event_t` | `ideatek_body_joint_event_t_desc` | `IDEATEK_BODY_JOINT_EVENT_FIELDS` |
| `ideatek:Vehicle` | `ideatek_vehicle_t` | `ideatek_vehicle_t_desc` | `IDEATEK_VEHICLE_FIELDS` |
| `ideatek:ideatek_wheel_flags_t` | `ideatek_wheel_flags_t` | `ideatek_wheel_flags_t_desc` | `IDEATEK_WHEEL_FLAGS_VALUES` |
| `ideatek:Wheel` | `ideatek_wheel_t` | `ideatek_wheel_t_desc` | `IDEATEK_WHEEL_FIELDS` |
| `ideatek:VehicleInput` | `ideatek_vehicle_input_t` | `ideatek_vehicle_input_t_desc` | `IDEATEK_VEHICLE_INPUT_FIELDS` |
| `ideatek:WheelState` | `ideatek_wheel_state_t` | `ideatek_wheel_state_t_desc` | `IDEATEK_WHEEL_STATE_FIELDS` |
| `ideatek:PoseDrive` | `ideatek_pose_drive_t` | `ideatek_pose_drive_t_desc` | `IDEATEK_POSE_DRIVE_FIELDS` |
| `ideatek:Destructible` | `ideatek_destructible_t` | `ideatek_destructible_t_desc` | `IDEATEK_DESTRUCTIBLE_FIELDS` |
| `ideatek:Damage` | `ideatek_damage_t` | `ideatek_damage_t_desc` | `IDEATEK_DAMAGE_FIELDS` |
| `ideatek:BreakEvent` | `ideatek_break_event_t` | `ideatek_break_event_t_desc` | `IDEATEK_BREAK_EVENT_FIELDS` |
| `ideatek:HiddenJoint` | `ideatek_hidden_joint_t` | `ideatek_hidden_joint_t_desc` | `IDEATEK_HIDDEN_JOINT_FIELDS` |
| `ideatek:Cut` | `ideatek_cut_t` | `ideatek_cut_t_desc` | `IDEATEK_CUT_FIELDS` |
| `ideatek:CutEvent` | `ideatek_cut_event_t` | `ideatek_cut_event_t_desc` | `IDEATEK_CUT_EVENT_FIELDS` |
| `ideatek:SoftBody` | `ideatek_soft_body_t` | `ideatek_soft_body_t_desc` | `IDEATEK_SOFT_BODY_FIELDS` |
| `ideatek:SoftBodyReset` | `ideatek_soft_body_reset_t` | `ideatek_soft_body_reset_t_desc` | `IDEATEK_SOFT_BODY_RESET_FIELDS` |
| `ideatek:ideatek_rope_flags_t` | `ideatek_rope_flags_t` | `ideatek_rope_flags_t_desc` | `IDEATEK_ROPE_FLAGS_VALUES` |
| `ideatek:Rope` | `ideatek_rope_t` | `ideatek_rope_t_desc` | `IDEATEK_ROPE_FIELDS` |
| `ideatek:Water` | `ideatek_water_t` | `ideatek_water_t_desc` | `IDEATEK_WATER_FIELDS` |
| `ideatek:Wave` | `ideatek_wave_t` | `ideatek_wave_t_desc` | `IDEATEK_WAVE_FIELDS` |
| `ideatek:WaterSample` | `ideatek_water_sample_t` | `ideatek_water_sample_t_desc` | `IDEATEK_WATER_SAMPLE_FIELDS` |
| `ideatek:Buoyancy` | `ideatek_buoyancy_t` | `ideatek_buoyancy_t_desc` | `IDEATEK_BUOYANCY_FIELDS` |
| `ideatek:ideatek_liquid_flags_t` | `ideatek_liquid_flags_t` | `ideatek_liquid_flags_t_desc` | `IDEATEK_LIQUID_FLAGS_VALUES` |
| `ideatek:Liquid` | `ideatek_liquid_t` | `ideatek_liquid_t_desc` | `IDEATEK_LIQUID_FIELDS` |
| `ideatek:LiquidEmitter` | `ideatek_liquid_emitter_t` | `ideatek_liquid_emitter_t_desc` | `IDEATEK_LIQUID_EMITTER_FIELDS` |
| `ideatek:LagHistory` | `ideatek_lag_history_t` | `ideatek_lag_history_t_desc` | `IDEATEK_LAG_HISTORY_FIELDS` |

```text
{ IDEATEK_BODY_KIND_STATIC = 0, IDEATEK_BODY_KIND_KINEMATIC = 1, IDEATEK_BODY_KIND_DYNAMIC = 2 }
{ IDEATEK_MASS_MODE_SHAPES = 0, IDEATEK_MASS_MODE_CUSTOM = 1 }
{ IDEATEK_LOCKED_X = 0x01, IDEATEK_LOCKED_Y = 0x02, IDEATEK_LOCKED_Z = 0x04, IDEATEK_LOCKED_ROLL = 0x08, IDEATEK_LOCKED_PITCH = 0x10, IDEATEK_LOCKED_YAW = 0x20 }
{ IDEATEK_BODY_FLAGS_CAN_SLEEP = 0x01, IDEATEK_BODY_FLAGS_CONTINUOUS = 0x02, IDEATEK_BODY_FLAGS_DESTRUCTIBLE = 0x04 }
{ ideatek_vec3_t center_of_mass; ideatek_vec3_t inertia; float mass; float gravity_scale; float linear_damping; float angular_damping; ideatek_body_kind_t kind; ideatek_mass_mode_t mass_mode; ideatek_locked_t locked; ideatek_body_flags_t flags; }
{ ideatek_vec3_t linear; ideatek_vec3_t angular; bool sleeping; }
{ ideatek_position_t point; ideatek_vec3_t force; ideatek_vec3_t torque; ideatek_vec3_t impulse; ideatek_vec3_t angular_impulse; }
{ IDEATEK_SHAPE_KIND_SPHERE = 0, IDEATEK_SHAPE_KIND_BOX = 1, IDEATEK_SHAPE_KIND_CAPSULE = 2, IDEATEK_SHAPE_KIND_CYLINDER = 3, IDEATEK_SHAPE_KIND_CONVEX = 4, IDEATEK_SHAPE_KIND_TRIANGLES = 5, IDEATEK_SHAPE_KIND_PLANE = 6 }
{ IDEATEK_SHAPE_FLAGS_SENSOR = 0x01, IDEATEK_SHAPE_FLAGS_SENSOR_EVENTS = 0x02, IDEATEK_SHAPE_FLAGS_CONTACT_EVENTS = 0x04, IDEATEK_SHAPE_FLAGS_HIT_EVENTS = 0x08 }
{ ideatek_entity_t body; ideatek_entity_t material; ideatek_entity_t mesh; uint64_t categories; uint64_t touches; ideatek_vec3_t size; uint32_t part; ideatek_shape_kind_t kind; ideatek_shape_flags_t flags; }
{ IDEATEK_COMBINE_AVERAGE = 0, IDEATEK_COMBINE_MIN = 1, IDEATEK_COMBINE_MULTIPLY = 2, IDEATEK_COMBINE_MAX = 3 }
{ float friction; float restitution; float density; float tensile; float compression; float shear; ideatek_combine_t friction_combine; ideatek_combine_t restitution_combine; }
{ IDEATEK_TOUCH_KIND_BEGAN = 0, IDEATEK_TOUCH_KIND_ENDED = 1 }
{ ideatek_entity_t sensor; ideatek_entity_t visitor; uint32_t order; ideatek_touch_kind_t kind; }
{ ideatek_entity_t a; ideatek_entity_t b; uint32_t order; ideatek_touch_kind_t kind; }
{ ideatek_entity_t a; ideatek_entity_t b; ideatek_position_t point; ideatek_vec3_t normal; float speed; uint32_t order; }
{ IDEATEK_QUERY_KIND_RAY = 0, IDEATEK_QUERY_KIND_SHAPE_CAST = 1, IDEATEK_QUERY_KIND_OVERLAP = 2, IDEATEK_QUERY_KIND_WATER = 3 }
{ IDEATEK_QUERY_MODE_TEST = 0, IDEATEK_QUERY_MODE_CLOSEST = 1, IDEATEK_QUERY_MODE_ALL = 2 }
{ ideatek_position_t origin; uint64_t categories; uint64_t touches; ideatek_quat_t rotation; ideatek_vec3_t translation; ideatek_vec3_t size; uint32_t max; uint32_t tick; ideatek_query_kind_t kind; ideatek_shape_kind_t shape; ideatek_query_mode_t mode; }
{ ideatek_entity_t query; ideatek_entity_t shape; ideatek_position_t point; ideatek_vec3_t normal; float fraction; uint32_t order; }
{ IDEATEK_JOINT_FLAGS_COLLIDE_CONNECTED = 0x01, IDEATEK_JOINT_FLAGS_BREAKABLE = 0x02 }
{ ideatek_entity_t a; ideatek_entity_t b; ideatek_quat_t rotation_a; ideatek_quat_t rotation_b; ideatek_vec3_t position_a; ideatek_vec3_t position_b; float force_threshold; float torque_threshold; ideatek_joint_flags_t flags; }
{ IDEATEK_JOINT_FEATURES_LIMIT = 0x01, IDEATEK_JOINT_FEATURES_SPRING = 0x02, IDEATEK_JOINT_FEATURES_MOTOR = 0x04 }
{ float lower; float upper; float spring_hertz; float spring_damping; float motor_speed; float max_motor_torque; ideatek_joint_features_t features; }
{ float lower; float upper; float spring_hertz; float spring_damping; float motor_speed; float max_motor_force; ideatek_joint_features_t features; }
{ float length; float min_length; float max_length; float spring_hertz; float spring_damping; float motor_speed; float max_motor_force; ideatek_joint_features_t features; }
{ float swing_y; float swing_z; float twist_min; float twist_max; }
{ ideatek_vec3_t linear_min; ideatek_vec3_t linear_max; ideatek_vec3_t angular_min; ideatek_vec3_t angular_max; }
{ ideatek_entity_t joint; float force; float torque; uint32_t order; }
{ float max_drive_torque; float max_pitch_roll; }
{ IDEATEK_WHEEL_FLAGS_DRIVEN = 0x01, IDEATEK_WHEEL_FLAGS_STEERS = 0x02 }
{ ideatek_entity_t vehicle; ideatek_vec3_t position; ideatek_vec3_t suspension_direction; ideatek_vec3_t steering_axis; float radius; float width; float suspension_min; float suspension_max; float spring_hertz; float spring_damping; float max_steer; float max_brake_torque; float max_hand_brake_torque; ideatek_wheel_flags_t flags; }
{ float forward; float right; float brake; float hand_brake; }
{ float steer_angle; float angle; float angular_velocity; float suspension_length; float longitudinal_slip; float lateral_slip; bool contact; }
{ float position_strength; float orientation_strength; }
{ ideatek_entity_t interior; }
{ ideatek_position_t point; float radius; float pressure; uint32_t order; }
{ ideatek_entity_t body; ideatek_entity_t piece; ideatek_position_t point; uint32_t order; }
{ ideatek_entity_t cap; }
{ ideatek_entity_t drawn; ideatek_position_t point; ideatek_vec3_t normal; uint32_t order; }
{ ideatek_entity_t drawn; ideatek_entity_t piece; uint32_t order; }
{ ideatek_entity_t source; uint64_t categories; uint64_t touches; float edge_compliance; float shear_compliance; float bend_compliance; float vertex_mass; float linear_damping; float friction; float gravity_scale; float vertex_radius; float pressure; float restitution; }
{ ideatek_entity_t body; uint32_t order; }
{ IDEATEK_ROPE_FLAGS_STIFF = 0x01, IDEATEK_ROPE_FLAGS_COLLIDE = 0x02 }
{ ideatek_entity_t start; ideatek_entity_t end; uint64_t categories; uint64_t touches; ideatek_vec3_t start_offset; ideatek_vec3_t end_offset; float length; float width; float gravity_scale; float friction; uint32_t segments; uint32_t sides; ideatek_rope_flags_t flags; }
{ ideatek_vec3_t flow; float density; }
{ ideatek_entity_t water; ideatek_vec3_t direction; float wavelength; float amplitude; float steepness; }
{ ideatek_entity_t query; ideatek_entity_t water; ideatek_position_t point; ideatek_vec3_t normal; ideatek_vec3_t velocity; float depth; uint32_t order; }
{ float linear_drag; float angular_drag; }
{ IDEATEK_LIQUID_FLAGS_PUSH_BODIES = 0x01 }
{ uint64_t categories; uint64_t touches; float density; float viscosity; float resolution; ideatek_liquid_flags_t flags; }
{ ideatek_entity_t liquid; ideatek_vec3_t velocity; float rate; }
{ float seconds; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_quat_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_position_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_body_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_mass_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_locked_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_body_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_shape_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_shape_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_combine_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_touch_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_query_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_query_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_joint_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_joint_features_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_wheel_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_rope_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_liquid_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Body` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Motion` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Push` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Shape` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PhysicsMaterial` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SensorEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ContactEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:HitEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Query` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:QueryHit` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BodyJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:FixedJoint` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PointJoint` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:HingeJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SliderJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DistanceJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SwingTwistJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SixDofJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BodyJointEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Vehicle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Wheel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VehicleInput` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WheelState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PoseDrive` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Destructible` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Damage` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BreakEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:HiddenJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Cut` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:CutEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoftBody` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoftBodyReset` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Rope` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Water` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Wave` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WaterSample` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Buoyancy` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Liquid` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LiquidEmitter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LagHistory` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The bodies, the shapes, the materials, the queries, the joints, the requests and their options | The types of the fields |
| The physics module | `ideatek:Transform`, `ideatek:Motion`, the events, `ideatek:WheelState`, `ideatek:Mesh` and `ideatek:MeshInstance` | Each type that the game writes, `ideatek:Tick`, and the types of the fields |
| The render module | No type. | `ideatek:HiddenJoint`, `ideatek:Water`, `ideatek:Wave`, and the types of the fields |

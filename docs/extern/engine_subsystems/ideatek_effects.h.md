# include/engine_subsystems/ideatek_effects.h

`include/engine_subsystems/ideatek_effects.h` gives the types of the effects: curves, particle emitters, their looks, their collision and the spawns of other emitters.
`readme.md`, *The contracts*, tells the rules of each contract.
Its look uses `ideatek_color_t` of the rendering part, so the header includes it.

## The roles

- The game or a tool is the author. It writes the curves and the emitters with their types.
- The effects module simulates the particles and draws them through the render module. It writes `ideatek:ParticleEmitterState` and `ideatek:ParticleEnded`.
- A particle is not an entity. Only the emitter is.

## The curves

A curve gives a value for each time from 0 to 1. The effects module reads it over the life of each particle.
`bytes` of `ideatek:Curve` holds `key_count` keys by rising time, with no bytes between them. A key of `IDEATEK_CURVE_KIND_FLOAT` is 2 floats: the time and the value. A key of `IDEATEK_CURVE_KIND_COLOR` is 5 floats: the time and a linear color with alpha.

The value between 2 keys is a straight blend. Before the first key and after the last key, the value of that key stays.
All numbers are little-endian. Many emitters can share 1 curve.

## The emitters

An emitter is an entity with `ideatek:ParticleEmitter` and a place. Its other types are optional.
The game starts and stops it with `state`. A `rate` of 0 gives no stream of particles. The references do not agree on a rate, so the game sets it.
A `max` of 0 means `rate` times `lifetime_max` plus `burst`.

With no `ideatek:ParticleShape`, the particles start at the origin of the emitter. With no `ideatek:ParticleVelocity`, they start with no speed.
With `IDEATEK_EMITTER_FLAGS_ONE_SHOT`, the module spawns `ideatek:ParticleEnded` when the last particle dies. With `IDEATEK_EMITTER_FLAGS_DESTROY_AT_END`, it then destroys the emitter.
The module chooses where and how often it simulates: CPU or GPU, the step, and the bounds.

## The looks

`ideatek:ParticleLook` tells how the particles draw. A look with no material does not draw, and the render module logs the emitter once.
A ribbon joins the live particles of the emitter in the order of their spawn. Its width is the size of each particle.
The glow comes from the emissive value of the material. Soft particles, the light and the blend are choices of the render module.

## The collision and the spawns

With `IDEATEK_COLLISION_SOURCE_DEPTH`, the particles hit the depth of the view: it is cheap, and it sees only what the camera sees.
With `IDEATEK_COLLISION_SOURCE_SHAPES`, they hit the `ideatek:Shape` of each static or kinematic body that the filter allows. The module logs each kind of shape that it cannot use once.
An emitter with `ideatek:ParticleSpawn` is a child: it spawns at the particles of its parent. A parent can have many children.
No event goes to the game for each particle.

## The fields

Each particle feels the wind and the force fields of the fields part.

## ideatek:ideatek_curve_kind_t

`ideatek_curve_kind_t` tells the values of a curve.
`ideatek_curve_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CURVE_KIND_FLOAT` | Each key holds 1 float. |
| 1 | `IDEATEK_CURVE_KIND_COLOR` | Each key holds a color with alpha. |

## ideatek:Curve

`ideatek:Curve` is a curve of values over a time from 0 to 1, on a resource entity.
`ideatek_curve_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The keys: *The curves*. |
| `key_count` | `uint32_t` | 8 | The number of keys. |
| `revision` | `uint32_t` | 12 | Raised at each change. |
| `kind` | `ideatek_curve_kind_t` | 16 | The values. |

Bytes 17 to 19 hold no field.

## ideatek:ideatek_emitter_state_t

`ideatek_emitter_state_t` tells the state of an emitter.
`ideatek_emitter_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_EMITTER_STATE_STOPPED` | The emitter makes no particle. |
| 1 | `IDEATEK_EMITTER_STATE_PLAYING` | The emitter makes particles. |
| 2 | `IDEATEK_EMITTER_STATE_PAUSED` | The particles stay as they are. |

## ideatek:ideatek_emitter_flags_t

`ideatek_emitter_flags_t` names the options of an emitter.
`ideatek_emitter_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_EMITTER_FLAGS_ONE_SHOT` | The emitter runs once. |
| 0x02 | `IDEATEK_EMITTER_FLAGS_LOCAL` | The particles move with the emitter. |
| 0x04 | `IDEATEK_EMITTER_FLAGS_DESTROY_AT_END` | The module destroys the emitter at its end. |

## ideatek:ParticleEmitter

`ideatek:ParticleEmitter` makes particles at the place of its entity.
`ideatek_particle_emitter_t` takes 40 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `rate` | `float` | 0 | 0 | The particles for each second. |
| `burst_interval` | `float` | 4 | 0 | The seconds between bursts. 0 means 1 burst at the start. |
| `lifetime_min` | `float` | 8 | 1 | The shortest life of a particle. |
| `lifetime_max` | `float` | 12 | 1 | The longest life. |
| `speed_scale` | `float` | 16 | 1 | The factor of the time of the emitter. |
| `prewarm` | `float` | 20 | 0 | The seconds that the emitter runs before its first frame. |
| `burst` | `uint32_t` | 24 | 0 | The particles of each burst. |
| `max` | `uint32_t` | 28 | 0 | The most live particles. |
| `seed` | `uint32_t` | 32 | 0 | The seed of the random values. 0 means random. |
| `state` | `ideatek_emitter_state_t` | 36 | `IDEATEK_EMITTER_STATE_STOPPED` | The state. |
| `flags` | `ideatek_emitter_flags_t` | 37 | 0 | The options. |

Bytes 38 to 39 hold no field.

## ideatek:ideatek_emission_shape_t

`ideatek_emission_shape_t` tells where the particles start.
`ideatek_emission_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_EMISSION_SHAPE_POINT` | The origin. |
| 1 | `IDEATEK_EMISSION_SHAPE_SPHERE` | Inside a sphere of radius `size.x`. |
| 2 | `IDEATEK_EMISSION_SHAPE_SPHERE_SURFACE` | On the surface of that sphere. |
| 3 | `IDEATEK_EMISSION_SHAPE_BOX` | Inside a box of half extents `size`. |
| 4 | `IDEATEK_EMISSION_SHAPE_RING` | On a ring of radius `size.x` about Y. |
| 5 | `IDEATEK_EMISSION_SHAPE_MESH` | On the positions of a mesh. |

## ideatek:ParticleShape

`ideatek:ParticleShape` gives the shape where the particles start.
`ideatek_particle_shape_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `mesh` | `ideatek_entity_t` | 0 | No default | The mesh of `IDEATEK_EMISSION_SHAPE_MESH`. |
| `size` | `ideatek_vec3_t` | 8 | 1, 1, 1 | The size, as the kind tells. |
| `kind` | `ideatek_emission_shape_t` | 20 | No default | The shape. |

Bytes 21 to 23 hold no field.

## ideatek:ParticleVelocity

`ideatek:ParticleVelocity` gives the start speed of the particles, in the space of the emitter.
`ideatek_particle_velocity_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `min` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The lowest start speed on each axis. |
| `max` | `ideatek_vec3_t` | 12 | 0, 0, 0 | The highest start speed on each axis. |
| `radial_min` | `float` | 24 | 0 | The lowest speed out from the center of the shape. |
| `radial_max` | `float` | 28 | 0 | The highest speed out from the center. |

Each particle takes a random vector between `min` and `max`, and a random radial speed.

## ideatek:ParticleForces

`ideatek:ParticleForces` gives the forces on the particles.
`ideatek_particle_forces_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `gravity` | `ideatek_vec3_t` | 0 | 0, -9.81, 0 | The gravity of the particles. |
| `drag` | `float` | 12 | 0 | The loss of speed. |

The default gravity is the standard gravity, as in the physics part.

## ideatek:ParticleEmitterState

`ideatek:ParticleEmitterState` is the state of an emitter. The effects module writes it.
`ideatek_particle_emitter_state_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `alive` | `uint32_t` | 0 | The live particles. |
| `done` | `bool` | 4 | `true` when a one-shot emitter ended. |

Bytes 5 to 7 hold no field.

## ideatek:ParticleEnded

`ideatek:ParticleEnded` tells that a one-shot emitter ended. It is an event of 1 round.
`ideatek_particle_ended_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `emitter` | `ideatek_entity_t` | 0 | The emitter. |
| `order` | `uint32_t` | 8 | The rank in the round. |

Bytes 12 to 15 hold no field.

## ideatek:ideatek_particle_kind_t

`ideatek_particle_kind_t` tells what each particle draws.
`ideatek_particle_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PARTICLE_KIND_SPRITE` | A flat image. |
| 1 | `IDEATEK_PARTICLE_KIND_MESH` | A mesh. |
| 2 | `IDEATEK_PARTICLE_KIND_RIBBON` | A strip through the particles. |

## ideatek:ideatek_particle_align_t

`ideatek_particle_align_t` tells how a sprite turns.
`ideatek_particle_align_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PARTICLE_ALIGN_CAMERA` | A sprite faces the camera. |
| 1 | `IDEATEK_PARTICLE_ALIGN_VELOCITY` | A sprite stretches along its speed. |

## ideatek:ideatek_particle_sort_t

`ideatek_particle_sort_t` tells the draw order of the particles.
`ideatek_particle_sort_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PARTICLE_SORT_NONE` | The order of the spawn. |
| 1 | `IDEATEK_PARTICLE_SORT_DEPTH` | The farthest particle first. |
| 2 | `IDEATEK_PARTICLE_SORT_AGE` | The oldest particle first. |

## ideatek:ParticleLook

`ideatek:ParticleLook` tells how the particles of an emitter draw.
`ideatek_particle_look_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `material` | `ideatek_entity_t` | 0 | No default | The material. |
| `mesh` | `ideatek_entity_t` | 8 | No default | The mesh of `IDEATEK_PARTICLE_KIND_MESH`. |
| `size_curve` | `ideatek_entity_t` | 16 | 0 | The curve of the size over the life. 0 means constant. |
| `color_curve` | `ideatek_entity_t` | 24 | 0 | The curve of the color over the life. 0 means constant. |
| `color` | `ideatek_color_t` | 32 | 1, 1, 1, 1 | The start color. |
| `size_min` | `float` | 48 | 1 | The smallest start size, in m. |
| `size_max` | `float` | 52 | 1 | The largest start size. |
| `rotation_min` | `float` | 56 | 0 | The lowest start angle. |
| `rotation_max` | `float` | 60 | 0 | The highest start angle. |
| `spin_min` | `float` | 64 | 0 | The lowest turn speed, in rad / s. |
| `spin_max` | `float` | 68 | 0 | The highest turn speed. |
| `frame_rate` | `float` | 72 | 0 | The frames of the flipbook for each second. 0 plays them once over the life. |
| `frames_x` | `uint32_t` | 76 | 1 | The columns of the flipbook. |
| `frames_y` | `uint32_t` | 80 | 1 | The rows of the flipbook. |
| `kind` | `ideatek_particle_kind_t` | 84 | `IDEATEK_PARTICLE_KIND_SPRITE` | What each particle draws. |
| `align` | `ideatek_particle_align_t` | 85 | `IDEATEK_PARTICLE_ALIGN_CAMERA` | The turn of a sprite. |
| `sort` | `ideatek_particle_sort_t` | 86 | `IDEATEK_PARTICLE_SORT_NONE` | The draw order. |

Byte 87 holds no field.

## ideatek:ideatek_collision_source_t

`ideatek_collision_source_t` tells what the particles hit.
`ideatek_collision_source_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_COLLISION_SOURCE_DEPTH` | The depth of the view. |
| 1 | `IDEATEK_COLLISION_SOURCE_SHAPES` | The shapes of the physics. |

## ideatek:ideatek_collision_mode_t

`ideatek_collision_mode_t` tells what a particle does at a hit.
`ideatek_collision_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_COLLISION_MODE_BOUNCE` | The particle bounces. |
| 1 | `IDEATEK_COLLISION_MODE_KILL` | The particle dies. |
| 2 | `IDEATEK_COLLISION_MODE_STICK` | The particle stays where it hits. |

## ideatek:ParticleCollision

`ideatek:ParticleCollision` makes the particles of an emitter hit the world.
`ideatek_particle_collision_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `touches` | `uint64_t` | 0 | All bits | The collision categories that the particles hit. |
| `bounce` | `float` | 8 | 0 | The bounce, from 0 to 1. |
| `friction` | `float` | 12 | 0 | The friction. |
| `radius` | `float` | 16 | 0 | The radius of a particle. 0 means half its size. |
| `source` | `ideatek_collision_source_t` | 20 | `IDEATEK_COLLISION_SOURCE_DEPTH` | What the particles hit. |
| `mode` | `ideatek_collision_mode_t` | 21 | `IDEATEK_COLLISION_MODE_BOUNCE` | What a particle does at a hit. |

Bytes 22 to 23 hold no field.

## ideatek:ideatek_spawn_on_t

`ideatek_spawn_on_t` tells when a child emitter spawns.
`ideatek_spawn_on_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SPAWN_ON_START` | When a particle of the parent starts. |
| 1 | `IDEATEK_SPAWN_ON_END` | When it dies. |
| 2 | `IDEATEK_SPAWN_ON_COLLISION` | When it hits. |
| 3 | `IDEATEK_SPAWN_ON_CONSTANT` | At `rate` for each second of its life. |

## ideatek:ParticleSpawn

`ideatek:ParticleSpawn` makes its emitter a child that spawns at the particles of its parent.
`ideatek_particle_spawn_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `parent` | `ideatek_entity_t` | 0 | No default | The parent emitter. |
| `rate` | `float` | 8 | 0 | The spawns for each second with `IDEATEK_SPAWN_ON_CONSTANT`. |
| `inherit_velocity` | `float` | 12 | 0 | The part of the speed of the parent particle that the child takes. |
| `count` | `uint32_t` | 16 | 1 | The particles of each spawn. |
| `on` | `ideatek_spawn_on_t` | 20 | No default | When the child spawns. |

Bytes 21 to 23 hold no field.

Only 1 reference gives a rate, so its default is 0.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_curve_kind_t` | `ideatek_curve_kind_t` | `ideatek_curve_kind_t_desc` | `IDEATEK_CURVE_KIND_VALUES` |
| `ideatek:Curve` | `ideatek_curve_t` | `ideatek_curve_t_desc` | `IDEATEK_CURVE_FIELDS` |
| `ideatek:ideatek_emitter_state_t` | `ideatek_emitter_state_t` | `ideatek_emitter_state_t_desc` | `IDEATEK_EMITTER_STATE_VALUES` |
| `ideatek:ideatek_emitter_flags_t` | `ideatek_emitter_flags_t` | `ideatek_emitter_flags_t_desc` | `IDEATEK_EMITTER_FLAGS_VALUES` |
| `ideatek:ParticleEmitter` | `ideatek_particle_emitter_t` | `ideatek_particle_emitter_t_desc` | `IDEATEK_PARTICLE_EMITTER_FIELDS` |
| `ideatek:ideatek_emission_shape_t` | `ideatek_emission_shape_t` | `ideatek_emission_shape_t_desc` | `IDEATEK_EMISSION_SHAPE_VALUES` |
| `ideatek:ParticleShape` | `ideatek_particle_shape_t` | `ideatek_particle_shape_t_desc` | `IDEATEK_PARTICLE_SHAPE_FIELDS` |
| `ideatek:ParticleVelocity` | `ideatek_particle_velocity_t` | `ideatek_particle_velocity_t_desc` | `IDEATEK_PARTICLE_VELOCITY_FIELDS` |
| `ideatek:ParticleForces` | `ideatek_particle_forces_t` | `ideatek_particle_forces_t_desc` | `IDEATEK_PARTICLE_FORCES_FIELDS` |
| `ideatek:ParticleEmitterState` | `ideatek_particle_emitter_state_t` | `ideatek_particle_emitter_state_t_desc` | `IDEATEK_PARTICLE_EMITTER_STATE_FIELDS` |
| `ideatek:ParticleEnded` | `ideatek_particle_ended_t` | `ideatek_particle_ended_t_desc` | `IDEATEK_PARTICLE_ENDED_FIELDS` |
| `ideatek:ideatek_particle_kind_t` | `ideatek_particle_kind_t` | `ideatek_particle_kind_t_desc` | `IDEATEK_PARTICLE_KIND_VALUES` |
| `ideatek:ideatek_particle_align_t` | `ideatek_particle_align_t` | `ideatek_particle_align_t_desc` | `IDEATEK_PARTICLE_ALIGN_VALUES` |
| `ideatek:ideatek_particle_sort_t` | `ideatek_particle_sort_t` | `ideatek_particle_sort_t_desc` | `IDEATEK_PARTICLE_SORT_VALUES` |
| `ideatek:ParticleLook` | `ideatek_particle_look_t` | `ideatek_particle_look_t_desc` | `IDEATEK_PARTICLE_LOOK_FIELDS` |
| `ideatek:ideatek_collision_source_t` | `ideatek_collision_source_t` | `ideatek_collision_source_t_desc` | `IDEATEK_COLLISION_SOURCE_VALUES` |
| `ideatek:ideatek_collision_mode_t` | `ideatek_collision_mode_t` | `ideatek_collision_mode_t_desc` | `IDEATEK_COLLISION_MODE_VALUES` |
| `ideatek:ParticleCollision` | `ideatek_particle_collision_t` | `ideatek_particle_collision_t_desc` | `IDEATEK_PARTICLE_COLLISION_FIELDS` |
| `ideatek:ideatek_spawn_on_t` | `ideatek_spawn_on_t` | `ideatek_spawn_on_t_desc` | `IDEATEK_SPAWN_ON_VALUES` |
| `ideatek:ParticleSpawn` | `ideatek_particle_spawn_t` | `ideatek_particle_spawn_t_desc` | `IDEATEK_PARTICLE_SPAWN_FIELDS` |

```text
{ IDEATEK_CURVE_KIND_FLOAT = 0, IDEATEK_CURVE_KIND_COLOR = 1 }
{ bridge_text_t bytes; uint32_t key_count; uint32_t revision; ideatek_curve_kind_t kind; }
{ IDEATEK_EMITTER_STATE_STOPPED = 0, IDEATEK_EMITTER_STATE_PLAYING = 1, IDEATEK_EMITTER_STATE_PAUSED = 2 }
{ IDEATEK_EMITTER_FLAGS_ONE_SHOT = 0x01, IDEATEK_EMITTER_FLAGS_LOCAL = 0x02, IDEATEK_EMITTER_FLAGS_DESTROY_AT_END = 0x04 }
{ float rate; float burst_interval; float lifetime_min; float lifetime_max; float speed_scale; float prewarm; uint32_t burst; uint32_t max; uint32_t seed; ideatek_emitter_state_t state; ideatek_emitter_flags_t flags; }
{ IDEATEK_EMISSION_SHAPE_POINT = 0, IDEATEK_EMISSION_SHAPE_SPHERE = 1, IDEATEK_EMISSION_SHAPE_SPHERE_SURFACE = 2, IDEATEK_EMISSION_SHAPE_BOX = 3, IDEATEK_EMISSION_SHAPE_RING = 4, IDEATEK_EMISSION_SHAPE_MESH = 5 }
{ ideatek_entity_t mesh; ideatek_vec3_t size; ideatek_emission_shape_t kind; }
{ ideatek_vec3_t min; ideatek_vec3_t max; float radial_min; float radial_max; }
{ ideatek_vec3_t gravity; float drag; }
{ uint32_t alive; bool done; }
{ ideatek_entity_t emitter; uint32_t order; }
{ IDEATEK_PARTICLE_KIND_SPRITE = 0, IDEATEK_PARTICLE_KIND_MESH = 1, IDEATEK_PARTICLE_KIND_RIBBON = 2 }
{ IDEATEK_PARTICLE_ALIGN_CAMERA = 0, IDEATEK_PARTICLE_ALIGN_VELOCITY = 1 }
{ IDEATEK_PARTICLE_SORT_NONE = 0, IDEATEK_PARTICLE_SORT_DEPTH = 1, IDEATEK_PARTICLE_SORT_AGE = 2 }
{ ideatek_entity_t material; ideatek_entity_t mesh; ideatek_entity_t size_curve; ideatek_entity_t color_curve; ideatek_color_t color; float size_min; float size_max; float rotation_min; float rotation_max; float spin_min; float spin_max; float frame_rate; uint32_t frames_x; uint32_t frames_y; ideatek_particle_kind_t kind; ideatek_particle_align_t align; ideatek_particle_sort_t sort; }
{ IDEATEK_COLLISION_SOURCE_DEPTH = 0, IDEATEK_COLLISION_SOURCE_SHAPES = 1 }
{ IDEATEK_COLLISION_MODE_BOUNCE = 0, IDEATEK_COLLISION_MODE_KILL = 1, IDEATEK_COLLISION_MODE_STICK = 2 }
{ uint64_t touches; float bounce; float friction; float radius; ideatek_collision_source_t source; ideatek_collision_mode_t mode; }
{ IDEATEK_SPAWN_ON_START = 0, IDEATEK_SPAWN_ON_END = 1, IDEATEK_SPAWN_ON_COLLISION = 2, IDEATEK_SPAWN_ON_CONSTANT = 3 }
{ ideatek_entity_t parent; float rate; float inherit_velocity; uint32_t count; ideatek_spawn_on_t on; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_color_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_curve_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_emitter_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_emitter_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_emission_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_particle_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_particle_align_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_particle_sort_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_collision_source_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_collision_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_spawn_on_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Curve` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleEmitter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleShape` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleVelocity` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleForces` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleEmitterState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleEnded` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleLook` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleCollision` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleSpawn` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| An author | The curves and the emitters with their types | The types of the fields |
| The effects module | `ideatek:ParticleEmitterState`, `ideatek:ParticleEnded` | The curves, the emitters, `ideatek:WorldTransform`, `ideatek:Shape`, `ideatek:Wind`, `ideatek:ForceField`, and the types of the fields |

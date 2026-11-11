# include/engine_subsystems/ideatek_physics.h

`include/engine_subsystems/ideatek_physics.h` gives the types of the physics: bodies, shapes, materials, characters, joints, vehicles, destruction, soft bodies, ropes, fluids, fields, time areas and air areas.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the headers of the world and space parts, for the entities, the positions, the vectors and the rotations of its fields.

## The roles

- The game is the author. It writes the bodies, the shapes, the materials, the joints, the queries and the requests.
- The physics module steps the world in the fixed phase. It writes `ideatek:Motion`, the `ideatek:Transform` of each dynamic body, the events, the answers of the queries and the meshes that it makes.
- The render module reads `ideatek:Water`, `ideatek:Wave`, the particles of the liquids and the grids of the gases.

## The world of the physics

The units are SI: meters, kilograms, seconds, newtons and pascals.
The gravity is a setting of the physics module. The sleep limits, the largest angular speed and the solver are settings of the module too.
The physics module writes `ideatek:Transform` of a dynamic body. A mover writes it for a kinematic body, and nobody writes it for a static body.
The module takes the speed of a kinematic body from the change of its `ideatek:Transform`.

With `ideatek:KinematicTarget`, the module moves the kinematic body to the target over the step, so its speed is right in the step.
`IDEATEK_MOTION_QUALITY_LINEAR_CAST` casts a fast body along its motion, so it does not pass through thin shapes.
When a body starts or stops to sleep, the module spawns `ideatek:WakeEvent`.

## The shapes

A shape is on its own entity, or on the entity of its body. Its offset is its `ideatek:Transform` relative to its parent.
`IDEATEK_SHAPE_KIND_CONVEX` and `IDEATEK_SHAPE_KIND_TRIANGLES` read the positions of the sub-mesh `part` of an `ideatek:Mesh`. The module builds them again when `revision` changes.
`IDEATEK_SHAPE_KIND_TRIANGLES` is only for a static or a kinematic body. A round kind needs a uniform scale.

`IDEATEK_SHAPE_KIND_PLANE` is only for a static body. Its normal is the Y axis of the shape, and it does not use `size`.
`IDEATEK_SHAPE_KIND_HEIGHT_FIELD` is only for a static body: *The height fields*.
A capsule or a cylinder with a `size.z` above 0 is tapered: `size.x` is the bottom radius and `size.z` the top radius.
`ideatek:ShapeMaterials` on a triangle shape gives each triangle its own material.

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
A hit on a triangle shape or a height field gives `face`, `uv` and the `material` of the face. An overlap gives the `depth` of the overlap.
`IDEATEK_QUERY_KIND_WATER` answers with `ideatek:WaterSample` at `origin`. `IDEATEK_QUERY_KIND_GAS` and `IDEATEK_QUERY_KIND_SIGHT` answer with the samples of the gases: *The gases*.

A query with a `tick` other than 0 tests against the poses of that tick for each body with `ideatek:LagHistory`. Each other body is at its place of now.

## The joints

A joint is an entity with `ideatek:BodyJoint` and 1 type of a kind, for example `ideatek:HingeJoint`.
Above a threshold, the physics module spawns `ideatek:BodyJointEvent`. With `IDEATEK_JOINT_FLAGS_BREAKABLE`, it also turns the joint off in the same step. The game destroys the entity.
A rope that pulls bodies is a chain of distance joints.
For a limit of an axis of `ideatek:SixDofJoint`, a min equal to the max locks the axis, and a min above the max frees it.

## The vehicles

A vehicle is a dynamic body with `ideatek:Vehicle`. Each wheel is an entity with `ideatek:Wheel`.
With no `ideatek:VehicleEngine`, the drive torque goes in equal parts to the driven wheels. `forward` scales the largest drive torque, and `right` scales the largest steer angle.
With `ideatek:VehicleEngine` and `ideatek:VehicleTransmission`, the engine drives the wheels through the gears and each `ideatek:VehicleDifferential`.

A tracked vehicle steers by `left_ratio` and `right_ratio`. `track` of a wheel tells its side. A motorcycle leans into its turns.
A curve of friction holds pairs of 2 floats: a slip and a factor of friction. An empty curve is the curve of Jolt.
The defaults of a vehicle are the defaults of Jolt.

## The characters

A character is an entity with `ideatek:Character` and a capsule or a box `ideatek:Shape`. It has no `ideatek:Body`.
The game writes `ideatek:CharacterMove` in each step. The physics module moves the character, slides it along the walls and writes its `ideatek:Transform`.
It climbs a step up to `step_up`, and it keeps the character on the floor down to `stick_down`. A slope above `max_slope` does not hold the character.

It writes `ideatek:CharacterState` after each step. A crouch is a new size of the shape: the module refuses it while the larger shape would overlap.
With `IDEATEK_CHARACTER_FLAGS_INNER_BODY`, the module also keeps a kinematic body in the shape, so the queries and the sensors find the character.
The defaults are the defaults of the virtual character of Jolt.

## The snapshots

A snapshot holds the state of the world of the physics: the places, the speeds, the contacts and the joints.
The game spawns an entity with `ideatek:PhysicsSnapshotRequest`. In the same fixed step, the module attaches `ideatek:PhysicsSnapshot` to it.
`IDEATEK_SNAPSHOT_ACTION_SAVE` saves the bodies whose categories share a bit with `categories`. `IDEATEK_SNAPSHOT_ACTION_RESTORE` puts the world back to the bytes of `data`.

The bytes have the format of the module. A save and a restore with the same module and the same settings give the same steps after them.
With `deterministic`, the same inputs give the same steps on each machine of a target, for a rollback of the net modules.

## The height fields

A height field reads its heights from a 2D texture, the `source` of its shape, in `R16_UNORM` or `R32_SFLOAT`.
`size.x` and `size.z` are the distances between 2 samples, and `size.y` is the factor of a height. The origin is the corner of the first sample.
To edit the terrain, the game writes new texels and raises `revision` of the texture. The module builds the changed part again.
A sample of 65535 in a height field of `R16_UNORM` is a hole, with no collision, as `cNoCollisionValue16` of Jolt.

With `ideatek:ShapeMaterials`, each cell has its own material.

## The joint drives

A drive moves a joint toward a target. `IDEATEK_DRIVE_MODE_VELOCITY` reaches a speed, and `IDEATEK_DRIVE_MODE_POSITION` reaches an angle or a position with a spring.
`min_force` and `max_force` limit the force or the torque of the drive. A `min_force` of 0 makes a drive that only pushes.
With `IDEATEK_SPRING_MODE_FREQUENCY`, `spring` is a frequency in Hz and `damping` is a ratio. With `IDEATEK_SPRING_MODE_STIFFNESS`, they are a stiffness and a damping.

A limit with a `limit_hertz` above 0 is soft: it acts as a spring past its end. `friction` resists the motion of the joint.
With `IDEATEK_JOINT_FLAGS_REPORT`, the module writes `ideatek:BodyJointState` with the force and the torque of the joint after each step.
The defaults are the defaults of Jolt.

## The joints of 2 joints

`ideatek:GearJoint` turns 2 hinges at a ratio, and `ideatek:RackAndPinionJoint` couples a hinge to a slider. They name the 2 joints that they couple.
`ideatek:PulleyJoint` keeps the sum of 2 lengths to 2 fixed points constant, with `ratio` on the second length.
`ideatek:PathJoint` moves the second body along a path in the space of the first. A record of `path`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `position` | `float[3]` | 0 | The point of the path. |
| `tangent` | `float[3]` | 12 | The direction of the path at the point. |
| `normal` | `float[3]` | 24 | The normal of the path at the point. |

The path is a Hermite curve through the points. `ideatek:NoCollideJoint` only stops the collision of the 2 bodies.

## The articulations

An articulation is a tree of bodies with joints in reduced coordinates. It does not drift, so a long chain or a robot stays stiff.
The root is an entity with `ideatek:Articulation`. Each link is a body with `ideatek:ArticulationLink` that names its parent.
`motions` gives the motion of each of the 6 parts of the joint: 0 locked, 1 limited, 2 free. The order is X, Y, Z, then the turns about X, Y and Z.

`limits` holds 6 records of 2 floats, the lowest and the highest. `drives` holds 6 records of `ideatek_joint_drive_t`.
After each step, the module writes `ideatek:ArticulationState` on the root. `ideatek:ArticulationCommand` sets the positions and the speeds of the joints.
`positions` and `velocities` hold 1 float for each free or limited part, by link and then by part.

## The ragdolls

A ragdoll is the skeleton itself: each joint entity that falls gets `ideatek:Body`, `ideatek:Shape` and a `ideatek:BodyJoint` to the body of its parent joint.
A kinematic joint follows the animation. The physics module writes the joints that are dynamic, and it writes local places, so the space module stays last.
`influence` of the pose modifier of the physics stage blends the ragdoll with the animation.
`ideatek:PoseDrive` drives a dynamic joint with motors to its animated place, for an active ragdoll.

## The destruction

A body with `IDEATEK_BODY_FLAGS_DESTRUCTIBLE` breaks in 1 of 2 modes. With an `asset` of 0, it breaks with no pieces made before.
With an asset, a tool cut the body into chunks before: *The chunks*. The physics module only breaks the bonds between the chunks.

The game spawns `ideatek:Damage`. Its pressure falls in a straight line to 0 at its radius. Contacts also damage, inside the module.
The strengths of the material tell where the body breaks. The module checks the structure: a part with no path to a static body becomes a piece.

For each piece, the module spawns an entity with `ideatek:Transform`, a dynamic body, a convex shape, an `ideatek:MeshInstance` and a new mesh. Then it spawns `ideatek:BreakEvent`.
A dent rewrites the mesh and raises its `revision`. An explosion is a push. The game owns the life of the pieces.

The terrain module applies the same rule to the cells of a terrain, and spawns the same `ideatek:BreakEvent`: `docs/extern/engine_subsystems/ideatek_terrain.h.md`, *The pieces*.
Each module finds its pieces in its own data, because 1 bond for each pair of cells gives thousands of bonds to each chunk.

## The chunks

`ideatek:DestructibleAsset` holds the chunks and the bonds of a body that a tool cut. A record of `chunks`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `centroid` | `float[3]` | 0 | The center of the chunk, in the space of the body. |
| `volume` | `float` | 12 | The volume of the chunk. |
| `parent` | `uint32_t` | 16 | The chunk that holds this chunk, or `UINT32_MAX` for a root. |
| `support` | `uint8_t` | 20 | 1 if the chunk carries the structure, else 0. |

A record of `bonds`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `normal` | `float[3]` | 0 | The normal of the face between the 2 chunks. |
| `centroid` | `float[3]` | 12 | The center of the face. |
| `area` | `float` | 24 | The area of the face. |
| `chunk_a` | `uint32_t` | 28 | The first chunk. |
| `chunk_b` | `uint32_t` | 32 | The second chunk, or `UINT32_MAX` for a bond to the world. |

A bond breaks when the force on it, divided by its area, passes a strength of the material. The damage and the weight of the structure both load the bonds.
The chunk of a mesh is the sub-mesh with the index of the chunk. A group of chunks with no path to a bond to the world becomes a piece.

## The soft bodies

A soft body is a drawn entity with `ideatek:SoftBody`. The soft body streams of its source mesh pin each vertex to its skinned place.
The physics module skins the source vertices, steps them, and writes its own mesh with positions and normals. It points `ideatek:MeshInstance` at that mesh.

A cloth has pins and a pressure of 0. A closed mesh with a pressure above 0 is a ball or a tire. With no pin stream, the module writes the `ideatek:Transform` of the entity at the center of its vertices.
When the entity jumps, the game spawns `ideatek:SoftBodyReset`.
`ideatek:SoftBodyAttachment` pins 1 vertex to a body, at an offset in the space of the body.
The long range attachments of `lra` keep a cloth from stretching far from its pins.

## The ropes

A rope steps its points in the fixed phase and writes a tube mesh: `sides` around and `segments` along.
A rope never pushes a body. With `IDEATEK_ROPE_FLAGS_COLLIDE`, it rests on the shapes that its filter allows.
A rope is a chain of Cosserat rods: `stretch_compliance` and `bend_compliance` set its give.
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
`technique` asks for a method. A module that has no such method writes 1 line in the log and uses its own.
SPH and PBD move particles of the size `resolution`. FLIP moves particles on a grid of cells of the size `resolution`.

Liquids of 1 domain with different `phase` values do not mix, for example oil and water.
An emitter is an entity with `ideatek:LiquidEmitter`. A rate of 0 fills the shape once. It emits from `start` to `end` seconds of its life, and an `end` of 0 has no end.
After each step, the module writes `ideatek:LiquidParticles` and the surface as its own mesh. With `IDEATEK_LIQUID_FLAGS_FOAM`, it also writes `ideatek:LiquidFoam`.

## The gases

Gases fill a grid of cells in a sensor box: the entity with `ideatek:GasVolume`. The grid has the size of the box.
Each record of `species` is 1 gas, and its index is its number. Each cell holds the amount of each gas in kg / m^3, a temperature in K and a velocity.
Air is a gas of `species` as each other gas. A cell with no gas is a vacuum.

A record of `species`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `density` | `float` | 0 | The density relative to air at the same temperature and pressure. Air has 1. |
| `diffusion` | `float` | 4 | How fast the gas spreads alone, in m^2 / s. |
| `opacity` | `float` | 8 | The part of the light that 1 kg / m^3 of the gas stops in each meter. |
| `specific_heat` | `float` | 12 | The heat that warms 1 kg of the gas by 1 K, in J. |
| `color` | `float[3]` | 16 | The linear color of the gas. |

A record of `reactions`:

| Field | C type | Offset in the record | Value |
| --- | --- | --- | --- |
| `reactant` | `uint32_t` | 0 | The gas that burns. |
| `product` | `uint32_t` | 4 | The gas that the burn gives, or `UINT32_MAX` for none. |
| `ignition` | `float` | 8 | The temperature above which the reactant burns, in K. |
| `rate` | `float` | 12 | The part of the reactant of a cell that burns in each second. |
| `heat` | `float` | 16 | The heat that 1 kg of the reactant gives, in J. |
| `yield` | `float` | 20 | The kg of the product for each kg of the reactant. |
| `needs_air` | `uint32_t` | 24 | 1 if the reactant burns only where the pressure of the air is above 0. 0 if it also burns in a vacuum, as a fuel that holds its own oxygen. |

The records have no bytes between them. The game writes the gases and the reactions from its materials. The module only applies them in each cell.

The pressure of a cell is the sum, over its gases, of the amount times 287.05 divided by `density`, times the temperature.
This is the law of the ideal gas, with air as the unit. A gas that heats rises in pressure, and flows toward the cells of lower pressure.
Each cell loses the part `cooling` of its difference to `ambient_temperature` in each second.

The gravity at a cell comes from the module, from *The gravity areas* and from *The spin of an area*.
A gas rises by that gravity times the density of the gas around it minus its own density, divided by its own density.
So heat rises in a gas. In a vacuum, a gas falls as a body falls, and no gas rises.

Each face of the grid has an outside: `faces` holds 6 records of `ideatek_gas_face_t`, for -X, +X, -Y, +Y, -Z and +Z.
A wall stops the gas. Space takes each gas that reaches it, so a breach vents a room. A gas outside gives and takes 1 gas at its temperature and pressure.

A placed area is a start state: an entity with `ideatek:GasFill`. When it attaches, the module fills the cells of its shape with 1 gas, to its pressure and temperature.
Then the laws run. A cell that no fill reaches starts as a vacuum.

An emitter is an entity with `ideatek:GasEmitter`. It adds `rate` kg of 1 gas in each second, at `temperature`. It adds `heat` watts to the cells of its shape, and moves them toward `velocity` at `couple_rate`.
An emitter with a `rate` of 0 adds no gas: it is a wind or a heater inside the grid.

With `IDEATEK_GAS_FLAGS_COLLIDE`, the shapes push the gas. With `IDEATEK_GAS_FLAGS_PUSH_BODIES`, the gas pushes each body in it as a wind, with the velocity and the density of its cells: *The fields*.
With `IDEATEK_GAS_FLAGS_PRESSURE`, the pressure of the gases pushes the shapes in the grid, and loads the bonds of a destructible body as a strain. So a heated gas breaks a closed body: *The destruction*.

After each step, the module writes a 3D texture of the temperature, and 1 3D texture for each 4 gases. It points `ideatek:GasGrid` at them. Render draws them with the `color` and the `opacity` of each gas.
An entity with `ideatek:AirProbe` gets the pressure, the temperature and the density of the gas under its place, written in place after each step.
A query of the kind `IDEATEK_QUERY_KIND_GAS` answers with `ideatek:GasSample` at `origin`. A query of the kind `IDEATEK_QUERY_KIND_SIGHT` answers with `ideatek:SightSample`, for the sight of a sensor through smoke or steam.

## The fields

The wind and the force fields act on the things that the physics module moves, and on the particles of the effects part.

- A wind is a speed of the air. A reader drags its things toward the velocity of the wind, by its own drag.
- A force field changes the motion of the things in its shape, as its `target` tells.
- The direction of a wind and of a directional field is -Z of its `ideatek:WorldTransform`. A vortex turns about the Y axis of its entity.

A field acts inside its shape: the whole world, a sphere of radius `size.x`, a box of half extents `size`, or a cylinder along Y.
The strength is full under the distance `inner` from the center. Past `inner`, it is `(1 - (d - inner) / (size - inner)) ^ falloff`, so a falloff of 0 keeps it constant.
The fields add where they overlap. A field that fills the world has no falloff.

A gust adds a part of `speed` between `gust_min` and `gust_max`. It changes about `gust_frequency` times in each second.
`IDEATEK_FORCE_KIND_NOISE` gives a smooth random direction, which changes about `frequency` times in each second and in each meter.
`IDEATEK_FORCE_KIND_VECTOR_FIELD` reads the acceleration from `texture`: a 3D texture of 3 floats that fills the box of the field.

Soft bodies, ropes, liquids and gases feel each wind and each force field.
A rigid body feels the targets FORCE, VELOCITY, SLEEP and WAKE, and a character feels FORCE and VELOCITY. STRAIN acts only on the bonds of a destructible body.
With `IDEATEK_FIELD_TARGET_VELOCITY`, `acceleration` is a speed in m / s. With `IDEATEK_FIELD_TARGET_STRAIN`, it is a pressure in Pa on the bonds of the destruction.

A force field gives the same acceleration to each mass. A wind pushes a rigid body and a character by their `drag`, along the speed of the air relative to the body.
The force is 0.5 times the density of the gas at the body, times the square of that speed, times `drag`.
Where no gas grid covers a body, the density is `air_density` times the pressure of *The air of an area*, divided by `air_reference_pressure`. So a wind pushes no body in a vacuum.

So a light body flies in a wind, and a heavy body stays. A `drag` of 0 keeps the wind off a body.

The defaults are neutral: a speed and an acceleration of 0, a shape that fills the world, no falloff and no gust. The references do not agree on a size, so the game sets it.

## The gravity areas

An entity with `ideatek:GravityArea` changes the gravity in its shape, for example around a planet or inside a ship.
With `point`, the gravity pulls to the center of the area. Else it goes along -Y of the entity.
`IDEATEK_GRAVITY_MODE_REPLACE` takes the place of the setting `gravity`, and `IDEATEK_GRAVITY_MODE_ADD` adds to it.
Where areas overlap, the replacing area with the highest `priority` wins, and each adding area adds. `inner` and `falloff` act as for a field.

## The time of an area

An entity with `ideatek:TimeArea` changes the time rate in its shape. A rate of 1 is the time of the world, 0.5 is half the speed, and 0 stops the time.
`IDEATEK_TIME_MODE_REPLACE` takes the place of the rate of the world, and `IDEATEK_TIME_MODE_MULTIPLY` multiplies it.
Where areas overlap, the replacing area with the highest `priority` wins, and each multiplying area multiplies. Past `inner`, the rate goes toward 1 as the strength of a field: *The fields*.
The `scale` of `ideatek:TimeRequest` also multiplies each rate: `docs/extern/kernel/ideatek_frame.h.md`, *ideatek:TimeRequest*.

After each step, the physics module writes the rate at the place of each dynamic body in `rate` of `ideatek:Motion`. It writes the rate of each character in `rate` of `ideatek:CharacterState`.
A body moves by the step times its rate. Its speed stays in m / s of the world, so a body that leaves the area goes on at its speed.

A body at a rate of 0 stops, and keeps its speed. Other bodies meet it as a kinematic body that does not move. The module keeps each impulse that the body gets, and applies it when the rate rises above 0.

A reader with no body reads the areas at the origin of its `ideatek:WorldTransform`, as it reads the wind. The animation module plays each entity at the rate of its place. The effects module moves each particle at the rate of its place.
A sound follows the rate of its place through the `pitch` of its player: `docs/extern/engine_subsystems/ideatek_sound.h.md`.

## The spin of an area

An entity with `ideatek:SpinArea` spins the things in its shape about an axis. The axis goes through the place of the entity, along its +Y.
Each thing feels a pull out from the axis: `rate` squared times its distance to the axis. On the axis, the pull is 0.
A thing that moves also feels -2 times the spin times its velocity, so a long throw curves.

The bodies, the characters, the particles, the liquids and the cells of gas feel both. The effects module moves its particles in the same way.
A spin adds to the gravity and to the gravity areas. Where spin areas overlap, the area with the highest `priority` wins. `inner` and `falloff` act as for a field.

## The work of the contacts

After each step, the module writes the singleton `ideatek:ContactWork`: 1 record of `ideatek_contact_work_t` for each contact of the step.
A record holds the 2 shapes, their chunks, the point and the count of points. It also holds the energy of a hit and the work of a slide, in J.
The records are in the order of the contacts of the solver. Above `max_contact_work` records, the module keeps the records with the most energy.
The log then gets 1 line.

## The tear of particles

A hit or a slide whose work for each m^2 passes the strength of the surface tears particles off it.
The strength is the lowest strength of its `ideatek:PhysicsMaterial`. The mass that the module tears is the work above that strength, times `tear_rate`.
The particles go into the particle set with `IDEATEK_PARTICLE_SET_FLAGS_TEARS`. Above its `cap`, the module tears no particle, and the log gets 1 line.

## The particle sets

A particle set is an entity with `ideatek:ParticleSet`. It holds 1 record of `ideatek_particle_t` for each particle, with no entity for each particle.
The module moves each particle as a grain: it collides with the shapes, with the terrain and with the other particles of its set.
A particle has its mass, so it pushes a body that it hits. Each wind and each force field pushes it, by its radius.
The friction and the bounce of each particle are those of `material` of its set.

## The requests of the phases

A module writes the singleton `ideatek:PhaseRequest` to move a chunk or a particle to another solver. The physics module applies it in the next step.

| To | What the module does |
| --- | --- |
| `IDEATEK_SOLVER_SOFT` | The chunk becomes a piece, as a break makes, and the piece becomes a soft body. |
| `IDEATEK_SOLVER_LIQUID` | The mass of the chunk goes into the liquid of its place, or into a new liquid. |
| `IDEATEK_SOLVER_GAS` | The mass goes into the cells of the gas grid at its place, as `species` of the request. |
| `IDEATEK_SOLVER_RIGID` | A soft chunk becomes rigid again. A liquid or a gas that cools condenses into particles. |

A deployment with no solver for the new phase keeps the chunk in its phase, and the log gets 1 line.

## The surfaces

A module writes the singleton `ideatek:SurfaceMaterial` to give a chunk the friction and the bounce of another `ideatek:PhysicsMaterial`, for example a coating of oil.
A record with a `material` of 0 gives back the material of the shape.

## The heat of the gas

A module writes the singleton `ideatek:HeatSource`: 1 record of `ideatek_heat_source_t` for each surface that meets the gas in this step.
The module gives each source the heat of the cells of its place: `rate` times `area` times the difference of temperature. It adds `smoke` kg of the gas `smoke_species`, and turns `ash` kg into particles.
Then it writes the singleton `ideatek:HeatTaken`: the heat in J that each source took, in the order of the sources. The other module applies it at the next step.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `gravity` | 0, -9.81, 0 | Any vector | The gravity, in m / s^2. |
| `hit_threshold` | 1 | 0 or more | The speed of approach, in m / s, above which a contact gives a hit. |
| `soft_iterations` | 5 | 1 or more | The passes of the solver of the soft bodies. |
| `rope_iterations` | 1 | 1 or more | The passes of the solver of the ropes. |
| `rope_substep` | 0.02 | Above 0 | The longest step of a rope, in seconds. |
| `velocity_steps` | 10 | 1 or more | The passes of the solver on the speeds, in each step. |
| `position_steps` | 2 | 0 or more | The passes of the solver on the places, in each step. |
| `speculative_distance` | 0.02 | 0 or more | The distance, in m, under which the module makes a contact before the shapes touch. |
| `sleep_time` | 0.5 | Above 0 | The seconds that a body stays slow before it sleeps. |
| `stress_iterations` | 0 | 0 or more | The passes of the solver of the stress on the bonds. 0 lets the module choose. |
| `snapshot_bytes` | 1048576 | 0 or more | The bytes for a snapshot. Each half holds 1 snapshot. |
| `deterministic` | `false` | `true` or `false` | The same inputs give the same steps on each machine of a target. A measure of its cost on the reference hardware gives its default. |
| `air_density` | 1.204 | Above 0 | The density of the air at `air_reference_pressure`, in kg / m^3, where no gas grid covers a body. The default is the air at 20 degrees Celsius and at sea level. |
| `air_reference_pressure` | 101325 | Above 0 | The pressure of `air_density`, in Pa. |
| `air_pressure` | 101325 | 0 or more | The pressure where no gas grid and no air area act, in Pa. |
| `max_contact_work` | 65536 | 1 or more | The records of `ideatek:ContactWork` in 1 step. |
| `tear_rate` | 0.000001 | 0 or more | The kg that 1 J of work above the strength tears. A measure on the reference hardware can change it. |

## ideatek:ideatek_body_kind_t

`ideatek_body_kind_t` tells who moves a body.
`ideatek_body_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_BODY_KIND_STATIC` | The body does not move. |
| 1 | `IDEATEK_BODY_KIND_KINEMATIC` | A mover moves the body. |
| 2 | `IDEATEK_BODY_KIND_DYNAMIC` | The physics moves the body. |

## ideatek:ideatek_motion_quality_t

`ideatek_motion_quality_t` tells how a body checks its motion.
`ideatek_motion_quality_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_MOTION_QUALITY_DISCRETE` | The body moves by its step. |
| 1 | `IDEATEK_MOTION_QUALITY_LINEAR_CAST` | The module casts the body along its motion. |

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
| 0x02 | `IDEATEK_BODY_FLAGS_ENHANCED_EDGES` | The body does not catch on the inner edges of triangles. |
| 0x04 | `IDEATEK_BODY_FLAGS_DESTRUCTIBLE` | The body can break: *The destruction*. |
| 0x08 | `IDEATEK_BODY_FLAGS_GYROSCOPIC` | The body takes the gyroscopic force, for a fast spin. |

## ideatek:Body

`ideatek:Body` makes an entity a body of the physics.
`ideatek_body_t` takes 68 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `center_of_mass` | `ideatek_vec3_t` | 0 | No default | The center, in the space of the body. |
| `inertia` | `ideatek_vec3_t` | 12 | No default | The inertia on each axis, in kg m^2. |
| `mass` | `float` | 24 | No default | The mass, in kg. |
| `gravity_scale` | `float` | 28 | 1 | The factor of the gravity. |
| `linear_damping` | `float` | 32 | 0 | The loss of linear speed. |
| `angular_damping` | `float` | 36 | 0 | The loss of angular speed. |
| `max_linear_speed` | `float` | 40 | 500 | The largest linear speed, in m / s. |
| `max_angular_speed` | `float` | 44 | 0.25 * pi * 60 | The largest angular speed, in rad / s. |
| `sleep_threshold` | `float` | 48 | 0.03 | The speed, in m / s, under which the body can sleep. |
| `inertia_scale` | `float` | 52 | 1 | The factor of the inertia from the shapes. |
| `drag` | `float` | 56 | 0 | The drag area in the air, in m^2: *The fields*. |
| `kind` | `ideatek_body_kind_t` | 60 | No default | Who moves the body. |
| `mass_mode` | `ideatek_mass_mode_t` | 61 | `IDEATEK_MASS_MODE_SHAPES` | The source of the mass. |
| `locked` | `ideatek_locked_t` | 62 | 0 | The parts that do not move. |
| `flags` | `ideatek_body_flags_t` | 63 | `IDEATEK_BODY_FLAGS_CAN_SLEEP` | The options. |
| `motion_quality` | `ideatek_motion_quality_t` | 64 | `IDEATEK_MOTION_QUALITY_DISCRETE` | How the body checks its motion. |
| `velocity_steps` | `uint8_t` | 65 | 0 | The passes on the speeds for this body. 0 takes the setting. |
| `position_steps` | `uint8_t` | 66 | 0 | The passes on the places for this body. 0 takes the setting. |

Byte 67 holds no field.

## ideatek:KinematicTarget

`ideatek:KinematicTarget` moves a kinematic body over the next step. The module detaches it.
`ideatek_kinematic_target_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `position` | `ideatek_position_t` | 0 | The place at the end of the step. |
| `rotation` | `ideatek_quat_t` | 24 | The rotation at the end of the step. |

## ideatek:WakeEvent

`ideatek:WakeEvent` tells that a body starts or stops to sleep.
`ideatek_wake_event_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `body` | `ideatek_entity_t` | 0 | The body. |
| `order` | `uint32_t` | 8 | The rank in the step. |
| `asleep` | `bool` | 12 | `true` if the body starts to sleep, `false` if it wakes. |

Bytes 13 to 15 hold no field.

## ideatek:Motion

`ideatek:Motion` is the motion of a body. Only the physics module writes it.
`ideatek_motion_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `linear` | `ideatek_vec3_t` | 0 | The linear speed, in m / s. |
| `angular` | `ideatek_vec3_t` | 12 | The angular speed, in rad / s. |
| `sleeping` | `bool` | 24 | `true` if the body sleeps. |
| `rate` | `float` | 28 | The time rate at the place of the body: *The time of an area*. |

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
| 2 | `IDEATEK_SHAPE_KIND_CAPSULE` | A capsule along Y: radius `size.x`, half height `size.y`, top radius `size.z`. |
| 3 | `IDEATEK_SHAPE_KIND_CYLINDER` | A cylinder along Y: radius `size.x`, half height `size.y`, top radius `size.z`. |
| 4 | `IDEATEK_SHAPE_KIND_CONVEX` | The convex hull of a sub-mesh. |
| 5 | `IDEATEK_SHAPE_KIND_TRIANGLES` | The triangles of a sub-mesh. |
| 6 | `IDEATEK_SHAPE_KIND_PLANE` | An infinite plane. |
| 7 | `IDEATEK_SHAPE_KIND_HEIGHT_FIELD` | A grid of heights: *The height fields*. |

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
| `source` | `ideatek_entity_t` | 16 | No default | The mesh of a convex or a triangle shape, or the texture of a height field. |
| `categories` | `uint64_t` | 24 | 1 | The collision categories of the shape. |
| `touches` | `uint64_t` | 32 | All bits | The categories that the shape touches. |
| `size` | `ideatek_vec3_t` | 40 | No default | The size, as the kind tells. |
| `part` | `uint32_t` | 52 | No default | The sub-mesh of a convex or a triangle shape. |
| `kind` | `ideatek_shape_kind_t` | 56 | No default | The kind. |
| `flags` | `ideatek_shape_flags_t` | 57 | 0 | The options. |

Bytes 58 to 63 hold no field.

## ideatek:ShapeMaterials

`ideatek:ShapeMaterials` gives each face of a shape its material, on the entity of the shape.
`ideatek_shape_materials_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `materials` | `bridge_text_t` | 0 | The `ideatek_entity_t` of each material of the list. |
| `indices` | `bridge_text_t` | 8 | A `uint8_t` for each triangle or each cell: the rank of its material in the list. |

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
| 4 | `IDEATEK_QUERY_KIND_GAS` | The gases at `origin`. |
| 5 | `IDEATEK_QUERY_KIND_SIGHT` | The light that crosses the gases along a ray from `origin` along `translation`. |

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
| `back_faces` | `bool` | 91 | `false` | `true` if a ray also hits the back of a triangle. |

Bytes 92 to 95 hold no field.

## ideatek:QueryHit

`ideatek:QueryHit` is 1 answer of a query.
`ideatek_query_hit_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `query` | `ideatek_entity_t` | 0 | The query. |
| `shape` | `ideatek_entity_t` | 8 | The shape hit. |
| `point` | `ideatek_position_t` | 16 | The point of the hit. |
| `normal` | `ideatek_vec3_t` | 40 | The normal of the surface. |
| `fraction` | `float` | 52 | The part of `translation` before the hit. |
| `material` | `ideatek_entity_t` | 56 | The material of the face, or of the shape. |
| `uv` | `float[2]` | 64 | The barycentric place of the hit in the triangle. |
| `depth` | `float` | 72 | The depth of an overlap. |
| `face` | `uint32_t` | 76 | The triangle or the cell of the hit. |
| `sub_shape` | `uint32_t` | 80 | The part of a shape with parts. |
| `order` | `uint32_t` | 84 | The rank in the step. |

## ideatek:ideatek_joint_flags_t

`ideatek_joint_flags_t` names the options of a joint.
`ideatek_joint_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_JOINT_FLAGS_COLLIDE_CONNECTED` | The 2 bodies collide with each other. |
| 0x02 | `IDEATEK_JOINT_FLAGS_BREAKABLE` | The joint turns off above a threshold. |
| 0x04 | `IDEATEK_JOINT_FLAGS_REPORT` | The module writes `ideatek:BodyJointState`. |

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

## ideatek:ideatek_drive_mode_t

`ideatek_drive_mode_t` tells what a drive reaches: *The joint drives*.
`ideatek_drive_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_DRIVE_MODE_OFF` | The drive does not act. |
| 1 | `IDEATEK_DRIVE_MODE_VELOCITY` | The drive reaches the speed of `target`. |
| 2 | `IDEATEK_DRIVE_MODE_POSITION` | The drive reaches the angle or the position of `target`. |

## ideatek:ideatek_spring_mode_t

`ideatek_spring_mode_t` tells the meaning of the values of a spring.
`ideatek_spring_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SPRING_MODE_FREQUENCY` | A frequency in Hz and a damping ratio. |
| 1 | `IDEATEK_SPRING_MODE_STIFFNESS` | A stiffness and a damping. |

## ideatek:ideatek_joint_drive_t

`ideatek_joint_drive_t` is a motor of a joint: *The joint drives*.
`ideatek_joint_drive_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `target` | `float` | 0 | No default | The speed or the place to reach. |
| `min_force` | `float` | 4 | -FLT_MAX | The largest force or torque against the target, as a value of 0 or less. |
| `max_force` | `float` | 8 | `FLT_MAX` | The largest force or torque toward the target. |
| `spring` | `float` | 12 | 2 | The frequency or the stiffness of the spring of a position drive. |
| `damping` | `float` | 16 | 1 | The damping ratio or the damping. |
| `mode` | `ideatek_drive_mode_t` | 20 | `IDEATEK_DRIVE_MODE_OFF` | What the drive reaches. |
| `spring_mode` | `ideatek_spring_mode_t` | 21 | `IDEATEK_SPRING_MODE_FREQUENCY` | The meaning of `spring` and `damping`. |

Bytes 22 to 23 hold no field.

## ideatek:HingeJoint

`ideatek:HingeJoint` lets the bodies turn about the Z axis of the frames.
`ideatek_hinge_joint_t` takes 48 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `lower` | `float` | 0 | No default | The lowest angle. |
| `upper` | `float` | 4 | No default | The highest angle. |
| `limit_hertz` | `float` | 8 | 0 | The frequency of a soft limit. 0 is a hard limit. |
| `limit_damping` | `float` | 12 | 0 | The damping ratio of a soft limit. |
| `friction` | `float` | 16 | 0 | The torque of the friction, in N m. |
| `drive` | `ideatek_joint_drive_t` | 20 | No default | The motor, in rad / s or in rad. |
| `features` | `ideatek_joint_features_t` | 44 | No default | The parts that apply. |

Bytes 45 to 47 hold no field.

## ideatek:SliderJoint

`ideatek:SliderJoint` lets the bodies slide along the X axis of the frames.
`ideatek_slider_joint_t` takes 48 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `lower` | `float` | 0 | No default | The lowest position. |
| `upper` | `float` | 4 | No default | The highest position. |
| `limit_hertz` | `float` | 8 | 0 | The frequency of a soft limit. 0 is a hard limit. |
| `limit_damping` | `float` | 12 | 0 | The damping ratio of a soft limit. |
| `friction` | `float` | 16 | 0 | The force of the friction, in N. |
| `drive` | `ideatek_joint_drive_t` | 20 | No default | The motor, in m / s or in m. |
| `features` | `ideatek_joint_features_t` | 44 | No default | The parts that apply. |

Bytes 45 to 47 hold no field.

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
`ideatek_six_dof_joint_t` takes 64 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `linear_min` | `ideatek_vec3_t` | 0 | The lowest position on each axis. |
| `linear_max` | `ideatek_vec3_t` | 12 | The highest position on each axis. |
| `angular_min` | `ideatek_vec3_t` | 24 | The lowest angle about each axis. |
| `angular_max` | `ideatek_vec3_t` | 36 | The highest angle about each axis. |
| `drives` | `bridge_text_t` | 48 | 6 values of `ideatek_joint_drive_t`: on X, Y and Z, then about X, Y and Z. |
| `friction` | `bridge_text_t` | 56 | 6 floats of friction, in the same order. |

## ideatek:GearJoint

`ideatek:GearJoint` turns 2 hinges together: *The joints of 2 joints*.
`ideatek_gear_joint_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `hinge_a` | `ideatek_entity_t` | 0 | No default | The hinge of the first body. |
| `hinge_b` | `ideatek_entity_t` | 8 | No default | The hinge of the second body. |
| `ratio` | `float` | 16 | 1 | The turns of the second body for 1 turn of the first. |

Bytes 20 to 23 hold no field.

## ideatek:RackAndPinionJoint

`ideatek:RackAndPinionJoint` couples a turn to a slide.
`ideatek_rack_and_pinion_joint_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `hinge` | `ideatek_entity_t` | 0 | No default | The hinge of the pinion. |
| `slider` | `ideatek_entity_t` | 8 | No default | The slider of the rack. |
| `ratio` | `float` | 16 | 1 | The meters of the rack for 1 turn of the pinion. |

Bytes 20 to 23 hold no field.

## ideatek:PulleyJoint

`ideatek:PulleyJoint` hangs 2 bodies on 1 rope over 2 fixed points.
`ideatek_pulley_joint_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `fixed_a` | `ideatek_position_t` | 0 | No default | The fixed point of the first rope, in the world. |
| `fixed_b` | `ideatek_position_t` | 24 | No default | The fixed point of the second rope, in the world. |
| `ratio` | `float` | 48 | 1 | The factor of the second length. |
| `min_length` | `float` | 52 | 0 | The shortest sum. |
| `max_length` | `float` | 56 | -1 | The longest sum. A value below 0 is the sum at the start. |

Bytes 60 to 63 hold no field.

## ideatek:ideatek_path_rotation_t

`ideatek_path_rotation_t` tells how a body turns on a path.
`ideatek_path_rotation_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PATH_ROTATION_FREE` | The body turns freely. |
| 1 | `IDEATEK_PATH_ROTATION_TANGENT` | The body turns only about the tangent. |
| 2 | `IDEATEK_PATH_ROTATION_NORMAL` | The body turns only about the normal. |
| 3 | `IDEATEK_PATH_ROTATION_BINORMAL` | The body turns only about the binormal. |
| 4 | `IDEATEK_PATH_ROTATION_PATH` | The body follows the frame of the path. |
| 5 | `IDEATEK_PATH_ROTATION_FIXED` | The body keeps its rotation. |

## ideatek:PathJoint

`ideatek:PathJoint` moves a body along a curve, for a train or a lift.
`ideatek_path_joint_t` takes 44 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `path` | `bridge_text_t` | 0 | The records of the points: *The joints of 2 joints*. |
| `fraction` | `float` | 8 | The place of the body on the path at the start, from 0 to the number of points. |
| `max_friction` | `float` | 12 | The largest force of the friction along the path. |
| `drive` | `ideatek_joint_drive_t` | 16 | The motor along the path. |
| `rotation` | `ideatek_path_rotation_t` | 40 | How the body turns. |
| `looping` | `bool` | 41 | `true` if the end of the path joins its start. |

Bytes 42 to 43 hold no field.

## ideatek:ConeJoint

`ideatek:ConeJoint` holds the points together and keeps the X axes inside a cone.
`ideatek_cone_joint_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `half_angle` | `float` | 0 | The half angle of the cone about the X axis of the frames. |

## ideatek:MotorJoint

`ideatek:MotorJoint` drives a body toward the frame of the other, as the motor joint of Box3D.
`ideatek_motor_joint_t` takes 56 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `linear_velocity` | `ideatek_vec3_t` | 0 | The speed of the second body relative to the first. |
| `angular_velocity` | `ideatek_vec3_t` | 12 | The turn speed of the second body relative to the first. |
| `max_force` | `float` | 24 | The largest force toward `linear_velocity`, in N. |
| `max_torque` | `float` | 28 | The largest torque toward `angular_velocity`, in N m. |
| `linear_hertz` | `float` | 32 | The frequency of the spring to the point of the frame. |
| `linear_damping` | `float` | 36 | The damping ratio of that spring. |
| `max_spring_force` | `float` | 40 | The largest force of that spring, in N. |
| `angular_hertz` | `float` | 44 | The frequency of the spring to the rotation of the frame. |
| `angular_damping` | `float` | 48 | The damping ratio of that spring. |
| `max_spring_torque` | `float` | 52 | The largest torque of that spring, in N m. |

The move and the turn have each a spring, so a held body can follow a point closely and turn slowly.
A spring with a frequency of 0 does not act.

## ideatek:WheelJoint

`ideatek:WheelJoint` is a wheel with a suspension and a steer, as the wheel joint of Box3D.
`ideatek_wheel_joint_t` takes 48 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `lower` | `float` | 0 | The lowest place of the suspension. |
| `upper` | `float` | 4 | The highest place of the suspension. |
| `suspension_hertz` | `float` | 8 | The frequency of the suspension. |
| `suspension_damping` | `float` | 12 | The damping ratio of the suspension. |
| `max_steer` | `float` | 16 | The largest steer angle. |
| `steer_target` | `float` | 20 | The steer angle to reach. |
| `spin` | `ideatek_joint_drive_t` | 24 | The motor of the spin. |

## ideatek:NoCollideJoint

`ideatek:NoCollideJoint` is a tag. It only stops the collision of the 2 bodies of the joint.

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

## ideatek:BodyJointState

`ideatek:BodyJointState` gives the load of a joint with `IDEATEK_JOINT_FLAGS_REPORT`.
`ideatek_body_joint_state_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `force` | `ideatek_vec3_t` | 0 | The force of the last step, in N, in the space of `a`. |
| `torque` | `ideatek_vec3_t` | 12 | The torque of the last step, in N m. |

## ideatek:ideatek_articulation_joint_t

`ideatek_articulation_joint_t` tells the kind of the joint of a link.
`ideatek_articulation_joint_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ARTICULATION_JOINT_FIX` | The link does not move on its parent. |
| 1 | `IDEATEK_ARTICULATION_JOINT_PRISMATIC` | The link slides. |
| 2 | `IDEATEK_ARTICULATION_JOINT_REVOLUTE` | The link turns about 1 axis. |
| 3 | `IDEATEK_ARTICULATION_JOINT_SPHERICAL` | The link turns about each axis. |

## ideatek:ideatek_articulation_flags_t

`ideatek_articulation_flags_t` names the options of an articulation.
`ideatek_articulation_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_ARTICULATION_FLAGS_FIX_BASE` | The root does not move. |
| 0x02 | `IDEATEK_ARTICULATION_FLAGS_SELF_COLLISION` | The links collide with each other. |

## ideatek:Articulation

`ideatek:Articulation` makes a body the root of an articulation: *The articulations*.
`ideatek_articulation_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `sleep_threshold` | `float` | 0 | 0.03 | The speed under which the articulation can sleep. |
| `velocity_steps` | `uint8_t` | 4 | 0 | The passes on the speeds. 0 takes the setting. |
| `position_steps` | `uint8_t` | 5 | 0 | The passes on the places. 0 takes the setting. |
| `flags` | `ideatek_articulation_flags_t` | 6 | 0 | The options. |

Byte 7 holds no field.

## ideatek:ArticulationLink

`ideatek:ArticulationLink` makes a body a link of an articulation. The frames of `ideatek:BodyJoint` on the entity place the joint.
`ideatek_articulation_link_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `articulation` | `ideatek_entity_t` | 0 | No default | The root. |
| `parent` | `ideatek_entity_t` | 8 | No default | The parent link, or the root. |
| `limits` | `bridge_text_t` | 16 | No default | The 6 limits: *The articulations*. |
| `drives` | `bridge_text_t` | 24 | No default | The 6 drives: *The articulations*. |
| `friction` | `float` | 32 | 0 | The friction of the joint. |
| `max_joint_velocity` | `float` | 36 | 100 | The largest speed of the joint. |
| `motions` | `uint8_t[6]` | 40 | No default | The motion of each part: *The articulations*. |
| `joint` | `ideatek_articulation_joint_t` | 46 | No default | The kind of the joint to the parent. |

Byte 47 holds no field.

## ideatek:ArticulationState

`ideatek:ArticulationState` gives the joints of an articulation after a step.
`ideatek_articulation_state_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `positions` | `bridge_text_t` | 0 | The positions of the joints. |
| `velocities` | `bridge_text_t` | 8 | The speeds of the joints. |
| `count` | `uint32_t` | 16 | The number of floats in each list. |

## ideatek:ArticulationCommand

`ideatek:ArticulationCommand` sets the joints of an articulation. The module detaches it.
`ideatek_articulation_command_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `positions` | `bridge_text_t` | 0 | The positions to set, or no bytes. |
| `velocities` | `bridge_text_t` | 8 | The speeds to set, or no bytes. |
| `count` | `uint32_t` | 16 | The number of floats in each list. |

## ideatek:ideatek_ground_state_t

`ideatek_ground_state_t` tells what holds a character, as in Jolt.
`ideatek_ground_state_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_GROUND_STATE_ON_GROUND` | The character stands and can move. |
| 1 | `IDEATEK_GROUND_STATE_ON_STEEP_GROUND` | The character is on a slope that is too steep. |
| 2 | `IDEATEK_GROUND_STATE_NOT_SUPPORTED` | The character touches a shape that does not hold it. |
| 3 | `IDEATEK_GROUND_STATE_IN_AIR` | The character touches nothing below. |

## ideatek:ideatek_non_walkable_t

`ideatek_non_walkable_t` tells what a steep slope does to a character, as in PhysX.
`ideatek_non_walkable_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_NON_WALKABLE_SLIDE` | The character slides down a steep slope. |
| 1 | `IDEATEK_NON_WALKABLE_PREVENT_CLIMB` | The character cannot climb a steep slope, and does not slide. |

## ideatek:ideatek_character_flags_t

`ideatek_character_flags_t` names the options of a character.
`ideatek_character_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_CHARACTER_FLAGS_INNER_BODY` | A kinematic body follows the character. |
| 0x02 | `IDEATEK_CHARACTER_FLAGS_CAN_PUSH` | The character pushes dynamic bodies. |
| 0x04 | `IDEATEK_CHARACTER_FLAGS_CAN_BE_PUSHED` | Moving bodies push the character. |

## ideatek:ideatek_character_collision_t

`ideatek_character_collision_t` names the sides that a character touched in the step, as in PhysX.
`ideatek_character_collision_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_CHARACTER_COLLISION_SIDES` | The character touched a wall. |
| 0x02 | `IDEATEK_CHARACTER_COLLISION_UP` | The character touched a ceiling. |
| 0x04 | `IDEATEK_CHARACTER_COLLISION_DOWN` | The character touched a floor. |

## ideatek:Character

`ideatek:Character` makes an entity a character: *The characters*.
`ideatek_character_t` takes 52 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `up` | `ideatek_vec3_t` | 0 | 0, 1, 0 | The up direction of the character. |
| `max_slope` | `float` | 12 | 50 degrees | The steepest slope that holds the character, in rad. |
| `step_up` | `float` | 16 | 0.4 | The highest step that the character climbs. |
| `stick_down` | `float` | 20 | 0.5 | The largest drop that the character follows on the floor. |
| `min_step_forward` | `float` | 24 | 0.02 | The least forward move of a climb of a step. |
| `padding` | `float` | 28 | 0.02 | The gap that the character keeps from the shapes. |
| `predictive_distance` | `float` | 32 | 0.1 | The distance at which the module looks for contacts. |
| `mass` | `float` | 36 | 70 | The mass, for the pushes, in kg. |
| `max_strength` | `float` | 40 | 100 | The largest force that the character pushes with, in N. |
| `drag` | `float` | 44 | 0 | The drag area in the air, in m^2: *The fields*. |
| `non_walkable` | `ideatek_non_walkable_t` | 48 | `IDEATEK_NON_WALKABLE_SLIDE` | What a steep slope does. |
| `flags` | `ideatek_character_flags_t` | 49 | INNER_BODY, CAN_PUSH and CAN_BE_PUSHED | The options. |

Bytes 50 to 51 hold no field.

## ideatek:CharacterMove

`ideatek:CharacterMove` moves a character in the next step.
`ideatek_character_move_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `velocity` | `ideatek_vec3_t` | 0 | The speed that the game wants, in m / s, in the world. |

## ideatek:CharacterState

`ideatek:CharacterState` gives the ground of a character after a step.
`ideatek_character_state_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `ground` | `ideatek_entity_t` | 0 | The shape under the character, or 0. |
| `ground_material` | `ideatek_entity_t` | 8 | The material under the character, or 0. |
| `ground_normal` | `ideatek_vec3_t` | 16 | The normal of the ground. |
| `ground_velocity` | `ideatek_vec3_t` | 28 | The speed of the ground, for a lift or a ship. |
| `ground_state` | `ideatek_ground_state_t` | 40 | What holds the character. |
| `collision` | `ideatek_character_collision_t` | 41 | The sides that the character touched. |
| `rate` | `float` | 44 | The time rate at the place of the character: *The time of an area*. |

Bytes 42 to 43 hold no field.

## ideatek:ideatek_snapshot_action_t

`ideatek_snapshot_action_t` tells the action of a snapshot request.
`ideatek_snapshot_action_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SNAPSHOT_ACTION_SAVE` | Save the world. |
| 1 | `IDEATEK_SNAPSHOT_ACTION_RESTORE` | Put the world back to `data`. |

## ideatek:ideatek_snapshot_status_t

`ideatek_snapshot_status_t` tells the result of a snapshot request.
`ideatek_snapshot_status_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SNAPSHOT_STATUS_OK` | The action is done. |
| 1 | `IDEATEK_SNAPSHOT_STATUS_TOO_BIG` | The snapshot does not fit in `snapshot_bytes`. |
| 2 | `IDEATEK_SNAPSHOT_STATUS_FAILED` | The bytes are not a snapshot of this module. |

## ideatek:PhysicsSnapshotRequest

`ideatek:PhysicsSnapshotRequest` asks for a snapshot: *The snapshots*.
`ideatek_physics_snapshot_request_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `categories` | `uint64_t` | 0 | All bits | The bodies to save. |
| `data` | `bridge_text_t` | 8 | No default | With a restore: the bytes of a snapshot, in the share of the game. |
| `action` | `ideatek_snapshot_action_t` | 16 | No default | The action. |

Bytes 17 to 23 hold no field.

## ideatek:PhysicsSnapshot

`ideatek:PhysicsSnapshot` answers a snapshot request.
`ideatek_physics_snapshot_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `data` | `bridge_text_t` | 0 | With a save: the bytes of the snapshot. They stay good for 1 round. |
| `status` | `ideatek_snapshot_status_t` | 8 | The result. |

Bytes 9 to 11 hold no field.

## ideatek:ideatek_vehicle_kind_t

`ideatek_vehicle_kind_t` tells the kind of a vehicle.
`ideatek_vehicle_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_VEHICLE_KIND_WHEELED` | A car or a truck. |
| 1 | `IDEATEK_VEHICLE_KIND_TRACKED` | A tank or a bulldozer. |
| 2 | `IDEATEK_VEHICLE_KIND_MOTORCYCLE` | A vehicle with 2 wheels that leans. |

## ideatek:ideatek_wheel_test_t

`ideatek_wheel_test_t` tells how a wheel finds the ground.
`ideatek_wheel_test_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_WHEEL_TEST_RAY` | A ray finds the ground. |
| 1 | `IDEATEK_WHEEL_TEST_SPHERE` | A sphere cast finds the ground. |
| 2 | `IDEATEK_WHEEL_TEST_CYLINDER` | A cylinder cast finds the ground. |

## ideatek:ideatek_transmission_mode_t

`ideatek_transmission_mode_t` tells who shifts the gears.
`ideatek_transmission_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TRANSMISSION_MODE_AUTO` | The module shifts the gears. |
| 1 | `IDEATEK_TRANSMISSION_MODE_MANUAL` | The game shifts the gears with `gear` and `clutch`. |

## ideatek:Vehicle

`ideatek:Vehicle` makes a dynamic body a vehicle with wheels.
`ideatek_vehicle_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `max_drive_torque` | `float` | 0 | No default | The largest drive torque, in N m. |
| `max_pitch_roll` | `float` | 4 | pi | The largest tilt before the module stops the wheels. |
| `kind` | `ideatek_vehicle_kind_t` | 8 | `IDEATEK_VEHICLE_KIND_WHEELED` | The kind. |
| `test` | `ideatek_wheel_test_t` | 9 | `IDEATEK_WHEEL_TEST_RAY` | How the wheels find the ground. |

Bytes 10 to 11 hold no field.

## ideatek:VehicleEngine

`ideatek:VehicleEngine` is the engine of a vehicle, on its entity.
`ideatek_vehicle_engine_t` takes 28 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `torque_curve` | `bridge_text_t` | 0 | No default | Pairs of 2 floats: a part of the RPM range and a part of `max_torque`. No bytes is a flat curve. |
| `max_torque` | `float` | 8 | 500 | The largest torque, in N m. |
| `min_rpm` | `float` | 12 | 1000 | The lowest RPM before a stall. |
| `max_rpm` | `float` | 16 | 6000 | The highest RPM. |
| `inertia` | `float` | 20 | 0.5 | The inertia of the engine, in kg m^2. |
| `damping` | `float` | 24 | 0.2 | The loss of speed of the engine. |

## ideatek:VehicleTransmission

`ideatek:VehicleTransmission` is the gearbox of a vehicle, on its entity.
`ideatek_vehicle_transmission_t` takes 44 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `gears` | `bridge_text_t` | 0 | No default | The ratio of each gear, as floats. No bytes is 2.66, 1.78, 1.3, 1 and 0.74. |
| `reverse` | `bridge_text_t` | 8 | No default | The ratio of each reverse gear. No bytes is -2.9. |
| `shift_up_rpm` | `float` | 16 | 4000 | The RPM above which the auto mode shifts up. |
| `shift_down_rpm` | `float` | 20 | 2000 | The RPM under which the auto mode shifts down. |
| `switch_time` | `float` | 24 | 0.5 | The seconds of a shift. |
| `clutch_release_time` | `float` | 28 | 0.3 | The seconds of the release of the clutch. |
| `switch_latency` | `float` | 32 | 0.5 | The seconds after a shift before the next shift. |
| `clutch_strength` | `float` | 36 | 10 | The strength of the clutch. |
| `mode` | `ideatek_transmission_mode_t` | 40 | `IDEATEK_TRANSMISSION_MODE_AUTO` | Who shifts. |

Bytes 41 to 43 hold no field.

## ideatek:VehicleDifferential

`ideatek:VehicleDifferential` shares the torque between 2 wheels, on an entity of its own.
`ideatek_vehicle_differential_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `vehicle` | `ideatek_entity_t` | 0 | No default | The vehicle. |
| `left` | `ideatek_entity_t` | 8 | No default | The left wheel. |
| `right` | `ideatek_entity_t` | 16 | No default | The right wheel. |
| `ratio` | `float` | 24 | 3.42 | The ratio between the gearbox and the wheels. |
| `split` | `float` | 28 | 0.5 | The part of the torque on the right wheel. |
| `limited_slip` | `float` | 32 | 1.4 | The largest ratio of the 2 wheel speeds. `FLT_MAX` is an open differential. |
| `engine_torque_ratio` | `float` | 36 | 1 | The part of the torque of the engine on this differential. |

## ideatek:AntiRollBar

`ideatek:AntiRollBar` keeps a vehicle level in a turn, on an entity of its own.
`ideatek_anti_roll_bar_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `vehicle` | `ideatek_entity_t` | 0 | No default | The vehicle. |
| `left` | `ideatek_entity_t` | 8 | No default | The left wheel. |
| `right` | `ideatek_entity_t` | 16 | No default | The right wheel. |
| `stiffness` | `float` | 24 | 1000 | The stiffness, in N / m. |

Bytes 28 to 31 hold no field.

## ideatek:ideatek_wheel_flags_t

`ideatek_wheel_flags_t` names the roles of a wheel.
`ideatek_wheel_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_WHEEL_FLAGS_DRIVEN` | The wheel takes drive torque. |
| 0x02 | `IDEATEK_WHEEL_FLAGS_STEERS` | The wheel steers. |

## ideatek:Wheel

`ideatek:Wheel` is 1 wheel of a vehicle, on an entity of its own.
`ideatek_wheel_t` takes 128 bytes. Its alignment is 8.

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
| `wheel_up` | `ideatek_vec3_t` | 80 | 0, 1, 0 | The up direction of the wheel, for the camber. |
| `wheel_forward` | `ideatek_vec3_t` | 92 | 0, 0, 1 | The forward direction of the wheel, for the toe. |
| `longitudinal_friction` | `bridge_text_t` | 104 | No default | The curve of the friction along the wheel: *The vehicles*. |
| `lateral_friction` | `bridge_text_t` | 112 | No default | The curve of the friction across the wheel. |
| `flags` | `ideatek_wheel_flags_t` | 120 | 0 | The roles. |
| `track` | `uint8_t` | 121 | 0 | For a tracked vehicle: 0 for the left track, 1 for the right track. |

Bytes 122 to 127 hold no field.

## ideatek:VehicleInput

`ideatek:VehicleInput` drives a vehicle. The game writes it.
`ideatek_vehicle_input_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `forward` | `float` | 0 | The drive, from -1 to 1. |
| `right` | `float` | 4 | The steer, from -1 to 1. |
| `brake` | `float` | 8 | The brake, from 0 to 1. |
| `hand_brake` | `float` | 12 | The hand brake, from 0 to 1. |
| `clutch` | `float` | 16 | With the manual mode: the clutch, from 0 to 1. |
| `left_ratio` | `float` | 20 | For a tracked vehicle: the drive of the left track, from -1 to 1. |
| `right_ratio` | `float` | 24 | For a tracked vehicle: the drive of the right track, from -1 to 1. |
| `gear` | `int32_t` | 28 | With the manual mode: the gear. 0 is neutral, a negative gear is a reverse gear. |

## ideatek:WheelState

`ideatek:WheelState` is the state of a wheel. The physics module writes it.
`ideatek_wheel_state_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `steer_angle` | `float` | 0 | The steer angle. |
| `angle` | `float` | 4 | The turn of the wheel about its axle. |
| `angular_velocity` | `float` | 8 | The speed of the turn. |
| `suspension_length` | `float` | 12 | The length of the suspension. |
| `longitudinal_slip` | `float` | 16 | The slip along the wheel. |
| `lateral_slip` | `float` | 20 | The slip across the wheel. |
| `contact_body` | `ideatek_entity_t` | 24 | The shape under the wheel, or 0. |
| `contact_material` | `ideatek_entity_t` | 32 | The material under the wheel, or 0. |
| `contact` | `bool` | 40 | `true` if the wheel touches the ground. |
| `hit_hard_point` | `bool` | 41 | `true` if the suspension is at its end. |

Bytes 42 to 47 hold no field.

## ideatek:VehicleState

`ideatek:VehicleState` is the state of the engine of a vehicle. The physics module writes it.
`ideatek_vehicle_state_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `rpm` | `float` | 0 | The RPM of the engine. |
| `clutch` | `float` | 4 | The friction of the clutch, from 0 to 1. |
| `gear` | `int32_t` | 8 | The current gear. |

## ideatek:PoseDrive

`ideatek:PoseDrive` drives a dynamic joint of a ragdoll to its animated place.
`ideatek_pose_drive_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `position_strength` | `float` | 0 | 0 | The pull to the animated position. |
| `orientation_strength` | `float` | 4 | 0 | The pull to the animated rotation. |

## ideatek:Destructible

`ideatek:Destructible` gives the inside of a body that breaks.
`ideatek_destructible_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `interior` | `ideatek_entity_t` | 0 | The material of the faces inside. 0 means the material of the surface. |
| `asset` | `ideatek_entity_t` | 8 | The resource entity of the chunks, or 0: *The destruction*. |

## ideatek:DestructibleAsset

`ideatek:DestructibleAsset` holds the chunks of a body that a tool cut, on a resource entity.
`ideatek_destructible_asset_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `chunks` | `bridge_text_t` | 0 | The records of the chunks: *The chunks*. |
| `bonds` | `bridge_text_t` | 8 | The records of the bonds: *The chunks*. |
| `mesh` | `ideatek_entity_t` | 16 | The mesh with 1 sub-mesh for each chunk. |
| `chunk_count` | `uint32_t` | 24 | The number of chunks. |
| `bond_count` | `uint32_t` | 28 | The number of bonds. |
| `revision` | `uint32_t` | 32 | Raised at each change. |

Bytes 36 to 39 hold no field.

## ideatek:ideatek_damage_shape_t

`ideatek_damage_shape_t` tells the shape of a damage, as the damage shaders of Blast.
`ideatek_damage_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_DAMAGE_SHAPE_RADIAL` | A sphere around `point`. |
| 1 | `IDEATEK_DAMAGE_SHAPE_CAPSULE` | A capsule from `point` to `point_b`. |
| 2 | `IDEATEK_DAMAGE_SHAPE_SHEAR` | A sphere that loads the bonds along `normal`. |
| 3 | `IDEATEK_DAMAGE_SHAPE_TRIANGLE` | The bonds that the triangle of the 3 points cuts. |
| 4 | `IDEATEK_DAMAGE_SHAPE_IMPACT` | A hit at `point` that spreads through the bonds. |

## ideatek:Damage

`ideatek:Damage` damages the bodies near a point. The game spawns it as an event.
`ideatek_damage_t` takes 104 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `point` | `ideatek_position_t` | 0 | The center. |
| `point_b` | `ideatek_position_t` | 24 | The second point of a capsule or a triangle. |
| `point_c` | `ideatek_position_t` | 48 | The third point of a triangle. |
| `normal` | `ideatek_vec3_t` | 72 | The direction of a shear. |
| `min_radius` | `float` | 84 | The radius of the full pressure. |
| `radius` | `float` | 88 | The radius. |
| `pressure` | `float` | 92 | The pressure at the center, in Pa. |
| `order` | `uint32_t` | 96 | The rank in the round. |
| `shape` | `ideatek_damage_shape_t` | 100 | The shape. |

Bytes 101 to 103 hold no field.

## ideatek:BreakEvent

`ideatek:BreakEvent` tells that a body lost a piece.
`ideatek_break_event_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `body` | `ideatek_entity_t` | 0 | The body that broke. |
| `piece` | `ideatek_entity_t` | 8 | The new piece. |
| `point` | `ideatek_position_t` | 16 | The point of the break. |
| `excess_force` | `ideatek_vec3_t` | 40 | The force on the bonds that broke, which pushes the piece. 0 for a piece of a terrain. |
| `chunks` | `bridge_text_t` | 52 | With an asset: the `uint32_t` index of each chunk of the piece. Empty for a piece of a terrain. |
| `order` | `uint32_t` | 60 | The rank in the step. |

## ideatek:ideatek_bend_t

`ideatek_bend_t` tells how a soft body resists a bend, as in Jolt.
`ideatek_bend_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_BEND_NONE` | No bend constraint. |
| 1 | `IDEATEK_BEND_DISTANCE` | A distance across each pair of faces. |
| 2 | `IDEATEK_BEND_DIHEDRAL` | The angle between each pair of faces. |

## ideatek:ideatek_lra_t

`ideatek_lra_t` tells the kind of the long range attachments, as in Jolt.
`ideatek_lra_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LRA_NONE` | No long range attachment. |
| 1 | `IDEATEK_LRA_EUCLIDEAN` | The straight distance to the nearest pin. |
| 2 | `IDEATEK_LRA_GEODESIC` | The distance along the surface to the nearest pin. |

## ideatek:SoftBody

`ideatek:SoftBody` makes a drawn entity a cloth or a soft body: *The soft bodies*.
`ideatek_soft_body_t` takes 88 bytes. Its alignment is 8.

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
| `volume_compliance` | `float` | 64 | 0 | The give of the volume of the tetrahedra. |
| `lra_scale` | `float` | 68 | 1 | The factor of the longest distance of the long range attachments. |
| `skin_distance_scale` | `float` | 72 | 1 | The factor of the distances of the pins to the skin. |
| `iterations` | `uint32_t` | 76 | 0 | The passes of the solver. 0 takes `soft_iterations`. |
| `bend` | `ideatek_bend_t` | 80 | `IDEATEK_BEND_DISTANCE` | The kind of the bend constraints. |
| `lra` | `ideatek_lra_t` | 81 | `IDEATEK_LRA_NONE` | The kind of the long range attachments. |
| `double_sided` | `bool` | 82 | `false` | `true` if both sides of a face collide. |

Bytes 83 to 87 hold no field.

The defaults are the defaults of Jolt. A bend compliance of `FLT_MAX` turns the bend off.

## ideatek:SoftBodyAttachment

`ideatek:SoftBodyAttachment` pins 1 vertex of a soft body to a body, on an entity of its own.
`ideatek_soft_body_attachment_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `soft_body` | `ideatek_entity_t` | 0 | The soft body. |
| `body` | `ideatek_entity_t` | 8 | The body that holds the vertex. |
| `offset` | `ideatek_vec3_t` | 16 | The point in the space of the body. |
| `vertex` | `uint32_t` | 28 | The vertex of the source mesh. |

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
`ideatek_rope_t` takes 96 bytes. Its alignment is 8.

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
| `stretch_compliance` | `float` | 72 | 0 | The give of the length. |
| `bend_compliance` | `float` | 76 | 0 | The give of a bend and a twist. |
| `segments` | `uint32_t` | 80 | 10 | The parts along the rope. |
| `sides` | `uint32_t` | 84 | 4 | The sides of the tube. |
| `flags` | `ideatek_rope_flags_t` | 88 | 0 | The options. |

Bytes 89 to 95 hold no field.

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
| 0x02 | `IDEATEK_LIQUID_FLAGS_FOAM` | The module writes the foam and the spray. |

## ideatek:ideatek_liquid_technique_t

`ideatek_liquid_technique_t` tells the method of a liquid: *The liquids*.
`ideatek_liquid_technique_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LIQUID_TECHNIQUE_SPH` | Smoothed particle hydrodynamics, as in SPlisHSPlasH. |
| 1 | `IDEATEK_LIQUID_TECHNIQUE_FLIP` | Particles on a grid, as FLIP and APIC in CubbyFlow. |
| 2 | `IDEATEK_LIQUID_TECHNIQUE_PBD` | Position based particles, as in PhysX. |

## ideatek:ideatek_emitter_shape_t

`ideatek_emitter_shape_t` tells the shape of an emitter of liquid or of gas.
`ideatek_emitter_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_EMITTER_SHAPE_SHAPE` | The `ideatek:Shape` of the entity. |
| 1 | `IDEATEK_EMITTER_SHAPE_BOX` | A box of `size` half extents. |
| 2 | `IDEATEK_EMITTER_SHAPE_DISC` | A disc of radius `size.x`, across the Z axis of the entity. |
| 3 | `IDEATEK_EMITTER_SHAPE_POINT` | The origin of the entity. |

## ideatek:Liquid

`ideatek:Liquid` is a simulated liquid in a domain: *The liquids*.
`ideatek_liquid_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `categories` | `uint64_t` | 0 | 1 | The collision categories. |
| `touches` | `uint64_t` | 8 | All bits | The categories that the liquid touches. |
| `density` | `float` | 16 | 1000 | The density, in kg / m^3. |
| `viscosity` | `float` | 20 | 0.001 | The viscosity, in Pa s. |
| `resolution` | `float` | 24 | 0.1 | The size of the detail, in m. |
| `surface_tension` | `float` | 28 | 0.0728 | The surface tension, in N / m. |
| `cohesion` | `float` | 32 | 0 | The pull between particles of the liquid. |
| `adhesion` | `float` | 36 | 0 | The pull of the liquid to the shapes. |
| `vorticity` | `float` | 40 | 0 | The strength that keeps the swirls of the liquid. |
| `max_particles` | `uint32_t` | 44 | 0 | The most particles. 0 lets the module choose. |
| `phase` | `uint32_t` | 48 | 0 | The phase of the liquid in its domain. |
| `flags` | `ideatek_liquid_flags_t` | 52 | 0 | The options. |
| `technique` | `ideatek_liquid_technique_t` | 53 | `IDEATEK_LIQUID_TECHNIQUE_SPH` | The method. |

Bytes 54 to 55 hold no field.

The defaults are the density, the viscosity and the surface tension of water at 20 degrees Celsius.

## ideatek:LiquidEmitter

`ideatek:LiquidEmitter` adds liquid in its shape.
`ideatek_liquid_emitter_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `liquid` | `ideatek_entity_t` | 0 | No default | The liquid. |
| `velocity` | `ideatek_vec3_t` | 8 | 0, 0, 0 | The speed of the new liquid. |
| `rate` | `float` | 20 | 0 | The volume for each second, in m^3 / s. 0 fills once. |
| `size` | `ideatek_vec3_t` | 24 | No default | The size of a box or a disc. |
| `start` | `float` | 36 | 0 | The seconds of life of the emitter before it starts. |
| `end` | `float` | 40 | 0 | The seconds of life at which it stops. 0 means no end. |
| `jitter` | `float` | 44 | 0 | The random move of the new particles, as a part of `resolution`. |
| `shape` | `ideatek_emitter_shape_t` | 48 | `IDEATEK_EMITTER_SHAPE_SHAPE` | The shape. |

Bytes 49 to 55 hold no field.

## ideatek:LiquidParticles

`ideatek:LiquidParticles` gives the particles of a liquid after a step, on its entity.
`ideatek_liquid_particles_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `positions` | `bridge_text_t` | 0 | 3 floats for each particle, in the world. |
| `velocities` | `bridge_text_t` | 8 | 3 floats for each particle. |
| `count` | `uint32_t` | 16 | The number of particles. |

## ideatek:LiquidFoam

`ideatek:LiquidFoam` gives the foam and the spray of a liquid with `IDEATEK_LIQUID_FLAGS_FOAM`.
`ideatek_liquid_foam_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `positions` | `bridge_text_t` | 0 | 3 floats for each particle of foam or spray. |
| `lifetimes` | `bridge_text_t` | 8 | 1 float for each particle: the seconds that it still lives. |
| `count` | `uint32_t` | 16 | The number of particles. |

## ideatek:ideatek_gas_flags_t

`ideatek_gas_flags_t` names the options of a gas volume.
`ideatek_gas_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_GAS_FLAGS_COLLIDE` | The shapes push the gas. |
| 0x02 | `IDEATEK_GAS_FLAGS_PUSH_BODIES` | The gas pushes the bodies in it. |
| 0x04 | `IDEATEK_GAS_FLAGS_PRESSURE` | The pressure pushes the shapes and loads the bonds. |

## ideatek:GasVolume

`ideatek:GasVolume` is a grid of gases in a sensor box: *The gases*.
`ideatek_gas_volume_t` takes 56 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `species` | `bridge_text_t` | 0 | No default | The records of the gases. |
| `reactions` | `bridge_text_t` | 8 | No default | The records of the reactions. |
| `cell_size` | `float` | 16 | 0.5 | The size of a cell of the grid, in m. |
| `steps_per_second` | `float` | 20 | 60 | The steps of the grid in each second. |
| `ambient_temperature` | `float` | 24 | 293.15 | The temperature of the air, in K. |
| `cooling` | `float` | 28 | 0 | The part of the difference to `ambient_temperature` that a cell loses in each second. |
| `vorticity` | `float` | 32 | 0.6 | The strength that keeps the swirls. |
| `species_count` | `uint32_t` | 36 | No default | The records in `species`. |
| `reaction_count` | `uint32_t` | 40 | No default | The records in `reactions`. |
| `flags` | `ideatek_gas_flags_t` | 44 | 0 | The options. |
| `faces` | `bridge_text_t` | 48 | No default | The 6 records of `ideatek_gas_face_t`: *The gases*. |

Bytes 45 to 47 hold no field.

The default of the temperature is the air at 20 degrees Celsius. The vorticity is the default of NVIDIA Flow. The references do not agree on a cooling, so the game sets it.

## ideatek:GasEmitter

`ideatek:GasEmitter` adds a gas, heat and a speed in its shape.
`ideatek_gas_emitter_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `volume` | `ideatek_entity_t` | 0 | No default | The gas volume. |
| `velocity` | `ideatek_vec3_t` | 8 | 0, 0, 0 | The speed that the cells take. |
| `size` | `ideatek_vec3_t` | 20 | No default | The size of a box or a disc. |
| `rate` | `float` | 32 | 0 | The gas that the emitter adds, in kg / s. |
| `temperature` | `float` | 36 | 293.15 | The temperature of the new gas, in K. |
| `heat` | `float` | 40 | 0 | The heat that the emitter adds, in W. |
| `couple_rate` | `float` | 44 | 2 | How fast the cells take `velocity`. |
| `species` | `uint32_t` | 48 | No default | The gas, as an index of `species`. |
| `shape` | `ideatek_emitter_shape_t` | 52 | `IDEATEK_EMITTER_SHAPE_SHAPE` | The shape. |

Bytes 53 to 55 hold no field.

## ideatek:GasGrid

`ideatek:GasGrid` points a gas volume at its textures. The physics module writes it.
`ideatek_gas_grid_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `temperature` | `ideatek_entity_t` | 0 | The 3D texture of the temperature, in K, in R. |
| `amounts` | `bridge_text_t` | 8 | The `ideatek_entity_t` of 1 3D texture for each 4 gases: the gas 4 n in R, then G, B and A. |
| `count` | `uint32_t` | 16 | The textures in `amounts`. |

Bytes 20 to 23 hold no field.

## ideatek:GasSample

`ideatek:GasSample` answers a query of the kind `IDEATEK_QUERY_KIND_GAS`.
`ideatek_gas_sample_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `query` | `ideatek_entity_t` | 0 | The query. |
| `volume` | `ideatek_entity_t` | 8 | The gas volume. |
| `amounts` | `bridge_text_t` | 16 | 1 float for each gas: its amount at the origin, in kg / m^3. |
| `velocity` | `ideatek_vec3_t` | 24 | The speed of the gas. |
| `temperature` | `float` | 36 | The temperature, in K. |
| `pressure` | `float` | 40 | The pressure, in Pa. |
| `order` | `uint32_t` | 44 | The rank in the step. |

## ideatek:SightSample

`ideatek:SightSample` answers a query of the kind `IDEATEK_QUERY_KIND_SIGHT`.
`ideatek_sight_sample_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `query` | `ideatek_entity_t` | 0 | The query. |
| `transmittance` | `float` | 8 | The part of the light that crosses the gases along the ray, from 0 to 1. |
| `order` | `uint32_t` | 12 | The rank in the step. |

## ideatek:LagHistory

`ideatek:LagHistory` keeps the past poses of a body for the queries of a past tick.
`ideatek_lag_history_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `seconds` | `float` | 0 | 0 | The length of the history. |

The default of 0 keeps no history. The game sets the length.

## ideatek:ideatek_field_shape_t

`ideatek_field_shape_t` tells the place where a field acts.
`ideatek_field_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FIELD_SHAPE_INFINITE` | The field fills the world. |
| 1 | `IDEATEK_FIELD_SHAPE_SPHERE` | A sphere of radius `size.x`. |
| 2 | `IDEATEK_FIELD_SHAPE_BOX` | A box of half extents `size`. |
| 3 | `IDEATEK_FIELD_SHAPE_CYLINDER` | A cylinder along Y of radius `size.x` and half height `size.y`. |

## ideatek:ideatek_force_kind_t

`ideatek_force_kind_t` tells the kind of a force field.
`ideatek_force_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FORCE_KIND_ATTRACTOR` | A pull to the center. A negative acceleration pushes away. |
| 1 | `IDEATEK_FORCE_KIND_VORTEX` | A turn about the Y axis of the entity. |
| 2 | `IDEATEK_FORCE_KIND_DIRECTIONAL` | A push along -Z of the entity. |
| 3 | `IDEATEK_FORCE_KIND_NOISE` | A turbulence: a smooth random direction. |
| 4 | `IDEATEK_FORCE_KIND_DRAG` | A pull of the speed toward 0. |
| 5 | `IDEATEK_FORCE_KIND_VECTOR_FIELD` | The direction and the strength from `texture`. |

## ideatek:ideatek_field_target_t

`ideatek_field_target_t` tells what a force field changes, as the targets of the fields of Chaos.
`ideatek_field_target_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FIELD_TARGET_FORCE` | The field adds an acceleration. |
| 1 | `IDEATEK_FIELD_TARGET_VELOCITY` | The field sets a speed. |
| 2 | `IDEATEK_FIELD_TARGET_STRAIN` | The field loads the bonds of a destructible body. |
| 3 | `IDEATEK_FIELD_TARGET_SLEEP` | The rigid bodies in the field sleep. |
| 4 | `IDEATEK_FIELD_TARGET_WAKE` | The rigid bodies in the field wake. |

## ideatek:ideatek_gravity_mode_t

`ideatek_gravity_mode_t` tells how a gravity area applies.
`ideatek_gravity_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_GRAVITY_MODE_REPLACE` | The area takes the place of the gravity of the world. |
| 1 | `IDEATEK_GRAVITY_MODE_ADD` | The area adds to the gravity of the world. |

## ideatek:Wind

`ideatek:Wind` is a wind along -Z of its entity: *The fields*.
`ideatek_wind_t` takes 40 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The size of the shape. |
| `inner` | `float` | 12 | 0 | The distance under which the strength is full. |
| `speed` | `float` | 16 | 0 | The speed of the air, in m / s. |
| `gust_min` | `float` | 20 | 0 | The lowest gust, as a part of `speed`. |
| `gust_max` | `float` | 24 | 0 | The highest gust, as a part of `speed`. |
| `gust_frequency` | `float` | 28 | 0 | The changes of the gust in each second. |
| `falloff` | `float` | 32 | 0 | The exponent of the falloff. |
| `shape` | `ideatek_field_shape_t` | 36 | `IDEATEK_FIELD_SHAPE_INFINITE` | The place where it blows. |

Bytes 37 to 39 hold no field.

## ideatek:ForceField

`ideatek:ForceField` pulls, turns or stirs the things in its shape: *The fields*.
`ideatek_force_field_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_entity_t` | 0 | 0 | With `IDEATEK_FORCE_KIND_VECTOR_FIELD`: the 3D texture of the vectors. |
| `size` | `ideatek_vec3_t` | 8 | 0, 0, 0 | The size of the shape. |
| `inner` | `float` | 20 | 0 | The distance under which the strength is full. |
| `acceleration` | `float` | 24 | 0 | The strength, in m / s^2. |
| `frequency` | `float` | 28 | 1 | With `IDEATEK_FORCE_KIND_NOISE`: the changes in each second and in each meter. |
| `falloff` | `float` | 32 | 0 | The exponent of the falloff. |
| `kind` | `ideatek_force_kind_t` | 36 | No default | The kind. |
| `shape` | `ideatek_field_shape_t` | 37 | `IDEATEK_FIELD_SHAPE_INFINITE` | The place where it acts. |
| `target` | `ideatek_field_target_t` | 38 | `IDEATEK_FIELD_TARGET_FORCE` | What it changes. |

Byte 39 holds no field.

## The air of an area

Where a gas grid covers a point, the air of the point is the gas of its cell: *The gases*.
Where no grid covers a point, an entity with `ideatek:AirArea` gives the pressure in its shape. A pressure of 0 is a vacuum.
Where neither acts, the pressure is `air_pressure`. An area of the shape `IDEATEK_FIELD_SHAPE_INFINITE` with the lowest `priority` gives the air of a level.

`IDEATEK_AIR_MODE_REPLACE` takes the place of the pressure, and `IDEATEK_AIR_MODE_MULTIPLY` multiplies it.
Where areas overlap, the replacing area with the highest `priority` wins, and each multiplying area multiplies.
Past `inner`, the pressure goes toward the pressure without the area, as the strength of a field: *The fields*.

A reader reads the air at a point through `ideatek:AirProbe`. The sound module reads it for the silence of a sound in a vacuum: `docs/extern/engine_subsystems/ideatek_sound.h.md`, *The air*.
A reader with no body reads the areas at the origin of its `ideatek:WorldTransform`.

## ideatek:GravityArea

`ideatek:GravityArea` changes the gravity in its shape: *The gravity areas*.
`ideatek_gravity_area_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The size of the shape. |
| `gravity` | `float` | 12 | 9.81 | The strength of the gravity, in m / s^2. |
| `inner` | `float` | 16 | 0 | The distance under which the strength is full. |
| `falloff` | `float` | 20 | 0 | The exponent of the falloff. |
| `priority` | `int32_t` | 24 | 0 | The rank among the areas that replace. |
| `mode` | `ideatek_gravity_mode_t` | 28 | `IDEATEK_GRAVITY_MODE_REPLACE` | How the area applies. |
| `shape` | `ideatek_field_shape_t` | 29 | `IDEATEK_FIELD_SHAPE_SPHERE` | The place where it acts. |
| `point` | `bool` | 30 | `false` | `true` if the gravity pulls to the center. |

Byte 31 holds no field.

## ideatek:ideatek_time_mode_t

`ideatek_time_mode_t` tells how a time area applies.
`ideatek_time_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TIME_MODE_REPLACE` | The area takes the place of the time rate of the world. |
| 1 | `IDEATEK_TIME_MODE_MULTIPLY` | The area multiplies the time rate. |

## ideatek:TimeArea

`ideatek:TimeArea` changes the time rate in its shape: *The time of an area*.
`ideatek_time_area_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The size of the shape. |
| `rate` | `float` | 12 | 1 | The time rate, 0 or more. |
| `inner` | `float` | 16 | 0 | The distance under which the rate is full. |
| `falloff` | `float` | 20 | 0 | The exponent of the falloff. |
| `priority` | `int32_t` | 24 | 0 | The rank among the areas that replace. |
| `mode` | `ideatek_time_mode_t` | 28 | `IDEATEK_TIME_MODE_REPLACE` | How the area applies. |
| `shape` | `ideatek_field_shape_t` | 29 | `IDEATEK_FIELD_SHAPE_SPHERE` | The place where it acts. |

Bytes 30 to 31 hold no field.

## ideatek:ideatek_air_mode_t

`ideatek_air_mode_t` tells how an air area applies.
`ideatek_air_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_AIR_MODE_REPLACE` | The area takes the place of the pressure of the air. |
| 1 | `IDEATEK_AIR_MODE_MULTIPLY` | The area multiplies the pressure of the air. |

## ideatek:AirArea

`ideatek:AirArea` changes the pressure of the air in its shape: *The air of an area*.
`ideatek_air_area_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The size of the shape. |
| `pressure` | `float` | 12 | 101325 | The pressure of the air, in Pa, 0 or more. |
| `inner` | `float` | 16 | 0 | The distance under which the pressure is full. |
| `falloff` | `float` | 20 | 0 | The exponent of the falloff. |
| `priority` | `int32_t` | 24 | 0 | The rank among the areas that replace. |
| `mode` | `ideatek_air_mode_t` | 28 | `IDEATEK_AIR_MODE_REPLACE` | How the area applies. |
| `shape` | `ideatek_field_shape_t` | 29 | `IDEATEK_FIELD_SHAPE_SPHERE` | The place where it acts. |

Bytes 30 to 31 hold no field.

## ideatek:ideatek_gas_face_kind_t

`ideatek_gas_face_kind_t` tells what is outside a face of a gas grid. It is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_GAS_FACE_KIND_WALL` | The face stops the gas. |
| 1 | `IDEATEK_GAS_FACE_KIND_SPACE` | The face takes each gas that reaches it. |
| 2 | `IDEATEK_GAS_FACE_KIND_GAS` | The face gives and takes 1 gas at its temperature and pressure. |

## ideatek:ideatek_gas_face_t

`ideatek_gas_face_t` is the outside of 1 face, in the bytes of `faces` of `ideatek:GasVolume`.
`ideatek_gas_face_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `kind` | `ideatek_gas_face_kind_t` | 0 | What is outside. |
| `species` | `uint32_t` | 4 | With `IDEATEK_GAS_FACE_KIND_GAS`: the gas outside. |
| `temperature` | `float` | 8 | With `IDEATEK_GAS_FACE_KIND_GAS`: its temperature, in K. |
| `pressure` | `float` | 12 | With `IDEATEK_GAS_FACE_KIND_GAS`: its pressure, in Pa. |

Bytes 1 to 3 hold no field.

## ideatek:GasFill

`ideatek:GasFill` is the start state of an area of a gas grid: *The gases*.
`ideatek_gas_fill_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `volume` | `ideatek_entity_t` | 0 | 0 | The entity of the `ideatek:GasVolume`. |
| `size` | `ideatek_vec3_t` | 8 | 0, 0, 0 | The size of the shape. |
| `temperature` | `float` | 20 | 293.15 | The temperature of the gas, in K. |
| `pressure` | `float` | 24 | 101325 | The pressure of the gas, in Pa. |
| `species` | `uint32_t` | 28 | 0 | The gas that fills the shape. |
| `shape` | `ideatek_field_shape_t` | 32 | `IDEATEK_FIELD_SHAPE_BOX` | The shape, at the place of the entity. |

Bytes 33 to 39 hold no field.

## ideatek:AirProbe

`ideatek:AirProbe` asks the air at the place of its entity. The physics module writes it in place after each step: *The air of an area*.
`ideatek_air_probe_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `pressure` | `float` | 0 | 0 | The pressure, in Pa. |
| `temperature` | `float` | 4 | 0 | The temperature of the gas, in K. 0 where no grid covers the place. |
| `density` | `float` | 8 | 0 | The density of the gas, in kg / m^3. |
| `covered` | `bool` | 12 | `false` | `true` if a gas grid covers the place. |

Bytes 13 to 15 hold no field.

## ideatek:SpinArea

`ideatek:SpinArea` spins the things in its shape: *The spin of an area*.
`ideatek_spin_area_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | 0, 0, 0 | The size of the shape. |
| `rate` | `float` | 12 | 0 | The spin about +Y of the entity, in rad / s. |
| `inner` | `float` | 16 | 0 | The distance under which the spin is full. |
| `falloff` | `float` | 20 | 0 | The exponent of the falloff. |
| `priority` | `int32_t` | 24 | 0 | The rank among the areas that overlap. |
| `shape` | `ideatek_field_shape_t` | 28 | `IDEATEK_FIELD_SHAPE_INFINITE` | The place where it acts. |

Bytes 29 to 31 hold no field.

## ideatek:ideatek_contact_work_t

`ideatek_contact_work_t` is 1 contact of a step, in the bytes of `ideatek:ContactWork`.
`ideatek_contact_work_t` takes 64 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `a` | `ideatek_entity_t` | 0 | The first shape. |
| `b` | `ideatek_entity_t` | 8 | The second shape. |
| `point` | `ideatek_position_t` | 16 | The middle of the points of the contact. |
| `energy` | `float` | 40 | The energy that a hit gave, in J. |
| `work` | `float` | 44 | The work of the slide in the step, in J. |
| `points` | `uint32_t` | 48 | The count of points of the contact. |
| `chunk_a` | `uint32_t` | 52 | The chunk of `a`, or `UINT32_MAX` for a body with no chunks. |
| `chunk_b` | `uint32_t` | 56 | The chunk of `b`, or `UINT32_MAX` for a body with no chunks. |

Bytes 60 to 63 hold no field.

## ideatek:ContactWork

`ideatek:ContactWork` is the singleton of the contacts of the last step: *The work of the contacts*.
`ideatek_contact_work_list_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `records` | `bridge_text_t` | 0 | No default | The records of `ideatek_contact_work_t`. |
| `count` | `uint32_t` | 8 | No default | The records in `records`. |

## ideatek:ideatek_particle_set_flags_t

`ideatek_particle_set_flags_t` names the options of a particle set. It is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_PARTICLE_SET_FLAGS_TEARS` | The set takes the particles that a tear makes. |

## ideatek:ideatek_particle_t

`ideatek_particle_t` is 1 particle, in the bytes of `ideatek:ParticleSet`.
`ideatek_particle_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `position` | `float[3]` | 0 | The place, relative to the entity of the set. |
| `radius` | `float` | 12 | The radius, in m. |
| `velocity` | `float[3]` | 16 | The speed, in m / s. |
| `mass` | `float` | 28 | The mass, in kg. |

## ideatek:ParticleSet

`ideatek:ParticleSet` is a set of particles: *The particle sets*.
`ideatek_particle_set_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `particles` | `bridge_text_t` | 0 | No default | The records of `ideatek_particle_t`. |
| `count` | `uint32_t` | 8 | No default | The records in `particles`. |
| `cap` | `uint32_t` | 12 | 65536 | The most particles of the set. |
| `material` | `ideatek_entity_t` | 16 | 0 | The `ideatek:PhysicsMaterial` of the particles. |
| `flags` | `ideatek_particle_set_flags_t` | 24 | 0 | The options. |

Bytes 25 to 31 hold no field.

## ideatek:ideatek_solver_t

`ideatek_solver_t` names a solver of the physics part. It is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SOLVER_RIGID` | The rigid solver. |
| 1 | `IDEATEK_SOLVER_SOFT` | The solver of soft bodies. |
| 2 | `IDEATEK_SOLVER_LIQUID` | The solver of liquids. |
| 3 | `IDEATEK_SOLVER_GAS` | The gas grid. |

## ideatek:ideatek_phase_request_t

`ideatek_phase_request_t` moves 1 chunk or 1 particle to a solver, in the bytes of `ideatek:PhaseRequest`.
`ideatek_phase_request_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `entity` | `ideatek_entity_t` | 0 | The body, or the particle set. |
| `index` | `uint32_t` | 8 | The chunk of the body, or the particle of the set. |
| `solver` | `ideatek_solver_t` | 12 | The new solver. |
| `particle` | `bool` | 13 | `true` if `index` names a particle. |
| `species` | `uint32_t` | 16 | With `IDEATEK_SOLVER_GAS`: the gas that the mass becomes. |

Bytes 14 to 15 and 20 to 23 hold no field.

## ideatek:PhaseRequest

`ideatek:PhaseRequest` is the singleton of the requests of the phases: *The requests of the phases*.
`ideatek_phase_request_list_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `records` | `bridge_text_t` | 0 | No default | The records of `ideatek_phase_request_t`. |
| `count` | `uint32_t` | 8 | No default | The records in `records`. |

## ideatek:ideatek_surface_material_t

`ideatek_surface_material_t` gives 1 chunk another surface, in the bytes of `ideatek:SurfaceMaterial`.
`ideatek_surface_material_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `body` | `ideatek_entity_t` | 0 | The body. |
| `material` | `ideatek_entity_t` | 8 | The `ideatek:PhysicsMaterial` of the surface, or 0 for the material of the shape. |
| `chunk` | `uint32_t` | 16 | The chunk. |

Bytes 20 to 23 hold no field.

## ideatek:SurfaceMaterial

`ideatek:SurfaceMaterial` is the singleton of the surfaces: *The surfaces*.
`ideatek_surface_material_list_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `records` | `bridge_text_t` | 0 | No default | The records of `ideatek_surface_material_t`. |
| `count` | `uint32_t` | 8 | No default | The records in `records`. |

## ideatek:ideatek_heat_source_t

`ideatek_heat_source_t` is 1 surface that meets the gas, in the bytes of `ideatek:HeatSource`.
`ideatek_heat_source_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `position` | `ideatek_position_t` | 0 | The place of the surface. |
| `area` | `float` | 24 | The area, in m^2. |
| `temperature` | `float` | 28 | The temperature of the surface, in K. |
| `rate` | `float` | 32 | The flow of heat, in W / (m^2 K). |
| `smoke` | `float` | 36 | The kg of smoke that the surface gives in this step. |
| `ash` | `float` | 40 | The kg of ash that the surface gives in this step. |
| `smoke_species` | `uint32_t` | 44 | The gas of the smoke. |

## ideatek:HeatSource

`ideatek:HeatSource` is the singleton of the sources of heat: *The heat of the gas*.
`ideatek_heat_source_list_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `records` | `bridge_text_t` | 0 | No default | The records of `ideatek_heat_source_t`. |
| `count` | `uint32_t` | 8 | No default | The records in `records`. |

## ideatek:HeatTaken

`ideatek:HeatTaken` is the singleton of the heat that each source took: *The heat of the gas*.
`ideatek_heat_taken_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `heats` | `bridge_text_t` | 0 | No default | 1 `float` for each source, in J, in the order of the sources. |
| `count` | `uint32_t` | 8 | No default | The floats in `heats`. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_body_kind_t` | `ideatek_body_kind_t` | `ideatek_body_kind_t_desc` | `IDEATEK_BODY_KIND_VALUES` |
| `ideatek:ideatek_motion_quality_t` | `ideatek_motion_quality_t` | `ideatek_motion_quality_t_desc` | `IDEATEK_MOTION_QUALITY_VALUES` |
| `ideatek:ideatek_mass_mode_t` | `ideatek_mass_mode_t` | `ideatek_mass_mode_t_desc` | `IDEATEK_MASS_MODE_VALUES` |
| `ideatek:ideatek_locked_t` | `ideatek_locked_t` | `ideatek_locked_t_desc` | `IDEATEK_LOCKED_VALUES` |
| `ideatek:ideatek_body_flags_t` | `ideatek_body_flags_t` | `ideatek_body_flags_t_desc` | `IDEATEK_BODY_FLAGS_VALUES` |
| `ideatek:Body` | `ideatek_body_t` | `ideatek_body_t_desc` | `IDEATEK_BODY_FIELDS` |
| `ideatek:KinematicTarget` | `ideatek_kinematic_target_t` | `ideatek_kinematic_target_t_desc` | `IDEATEK_KINEMATIC_TARGET_FIELDS` |
| `ideatek:WakeEvent` | `ideatek_wake_event_t` | `ideatek_wake_event_t_desc` | `IDEATEK_WAKE_EVENT_FIELDS` |
| `ideatek:Motion` | `ideatek_motion_t` | `ideatek_motion_t_desc` | `IDEATEK_MOTION_FIELDS` |
| `ideatek:Push` | `ideatek_push_t` | `ideatek_push_t_desc` | `IDEATEK_PUSH_FIELDS` |
| `ideatek:ideatek_shape_kind_t` | `ideatek_shape_kind_t` | `ideatek_shape_kind_t_desc` | `IDEATEK_SHAPE_KIND_VALUES` |
| `ideatek:ideatek_shape_flags_t` | `ideatek_shape_flags_t` | `ideatek_shape_flags_t_desc` | `IDEATEK_SHAPE_FLAGS_VALUES` |
| `ideatek:Shape` | `ideatek_shape_t` | `ideatek_shape_t_desc` | `IDEATEK_SHAPE_FIELDS` |
| `ideatek:ShapeMaterials` | `ideatek_shape_materials_t` | `ideatek_shape_materials_t_desc` | `IDEATEK_SHAPE_MATERIALS_FIELDS` |
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
| `ideatek:ideatek_drive_mode_t` | `ideatek_drive_mode_t` | `ideatek_drive_mode_t_desc` | `IDEATEK_DRIVE_MODE_VALUES` |
| `ideatek:ideatek_spring_mode_t` | `ideatek_spring_mode_t` | `ideatek_spring_mode_t_desc` | `IDEATEK_SPRING_MODE_VALUES` |
| `ideatek:ideatek_joint_drive_t` | `ideatek_joint_drive_t` | `ideatek_joint_drive_t_desc` | `IDEATEK_JOINT_DRIVE_FIELDS` |
| `ideatek:HingeJoint` | `ideatek_hinge_joint_t` | `ideatek_hinge_joint_t_desc` | `IDEATEK_HINGE_JOINT_FIELDS` |
| `ideatek:SliderJoint` | `ideatek_slider_joint_t` | `ideatek_slider_joint_t_desc` | `IDEATEK_SLIDER_JOINT_FIELDS` |
| `ideatek:DistanceJoint` | `ideatek_distance_joint_t` | `ideatek_distance_joint_t_desc` | `IDEATEK_DISTANCE_JOINT_FIELDS` |
| `ideatek:SwingTwistJoint` | `ideatek_swing_twist_joint_t` | `ideatek_swing_twist_joint_t_desc` | `IDEATEK_SWING_TWIST_JOINT_FIELDS` |
| `ideatek:SixDofJoint` | `ideatek_six_dof_joint_t` | `ideatek_six_dof_joint_t_desc` | `IDEATEK_SIX_DOF_JOINT_FIELDS` |
| `ideatek:GearJoint` | `ideatek_gear_joint_t` | `ideatek_gear_joint_t_desc` | `IDEATEK_GEAR_JOINT_FIELDS` |
| `ideatek:RackAndPinionJoint` | `ideatek_rack_and_pinion_joint_t` | `ideatek_rack_and_pinion_joint_t_desc` | `IDEATEK_RACK_AND_PINION_JOINT_FIELDS` |
| `ideatek:PulleyJoint` | `ideatek_pulley_joint_t` | `ideatek_pulley_joint_t_desc` | `IDEATEK_PULLEY_JOINT_FIELDS` |
| `ideatek:ideatek_path_rotation_t` | `ideatek_path_rotation_t` | `ideatek_path_rotation_t_desc` | `IDEATEK_PATH_ROTATION_VALUES` |
| `ideatek:PathJoint` | `ideatek_path_joint_t` | `ideatek_path_joint_t_desc` | `IDEATEK_PATH_JOINT_FIELDS` |
| `ideatek:ConeJoint` | `ideatek_cone_joint_t` | `ideatek_cone_joint_t_desc` | `IDEATEK_CONE_JOINT_FIELDS` |
| `ideatek:MotorJoint` | `ideatek_motor_joint_t` | `ideatek_motor_joint_t_desc` | `IDEATEK_MOTOR_JOINT_FIELDS` |
| `ideatek:WheelJoint` | `ideatek_wheel_joint_t` | `ideatek_wheel_joint_t_desc` | `IDEATEK_WHEEL_JOINT_FIELDS` |
| `ideatek:BodyJointEvent` | `ideatek_body_joint_event_t` | `ideatek_body_joint_event_t_desc` | `IDEATEK_BODY_JOINT_EVENT_FIELDS` |
| `ideatek:BodyJointState` | `ideatek_body_joint_state_t` | `ideatek_body_joint_state_t_desc` | `IDEATEK_BODY_JOINT_STATE_FIELDS` |
| `ideatek:ideatek_articulation_joint_t` | `ideatek_articulation_joint_t` | `ideatek_articulation_joint_t_desc` | `IDEATEK_ARTICULATION_JOINT_VALUES` |
| `ideatek:ideatek_articulation_flags_t` | `ideatek_articulation_flags_t` | `ideatek_articulation_flags_t_desc` | `IDEATEK_ARTICULATION_FLAGS_VALUES` |
| `ideatek:Articulation` | `ideatek_articulation_t` | `ideatek_articulation_t_desc` | `IDEATEK_ARTICULATION_FIELDS` |
| `ideatek:ArticulationLink` | `ideatek_articulation_link_t` | `ideatek_articulation_link_t_desc` | `IDEATEK_ARTICULATION_LINK_FIELDS` |
| `ideatek:ArticulationState` | `ideatek_articulation_state_t` | `ideatek_articulation_state_t_desc` | `IDEATEK_ARTICULATION_STATE_FIELDS` |
| `ideatek:ArticulationCommand` | `ideatek_articulation_command_t` | `ideatek_articulation_command_t_desc` | `IDEATEK_ARTICULATION_COMMAND_FIELDS` |
| `ideatek:ideatek_ground_state_t` | `ideatek_ground_state_t` | `ideatek_ground_state_t_desc` | `IDEATEK_GROUND_STATE_VALUES` |
| `ideatek:ideatek_non_walkable_t` | `ideatek_non_walkable_t` | `ideatek_non_walkable_t_desc` | `IDEATEK_NON_WALKABLE_VALUES` |
| `ideatek:ideatek_character_flags_t` | `ideatek_character_flags_t` | `ideatek_character_flags_t_desc` | `IDEATEK_CHARACTER_FLAGS_VALUES` |
| `ideatek:ideatek_character_collision_t` | `ideatek_character_collision_t` | `ideatek_character_collision_t_desc` | `IDEATEK_CHARACTER_COLLISION_VALUES` |
| `ideatek:Character` | `ideatek_character_t` | `ideatek_character_t_desc` | `IDEATEK_CHARACTER_FIELDS` |
| `ideatek:CharacterMove` | `ideatek_character_move_t` | `ideatek_character_move_t_desc` | `IDEATEK_CHARACTER_MOVE_FIELDS` |
| `ideatek:CharacterState` | `ideatek_character_state_t` | `ideatek_character_state_t_desc` | `IDEATEK_CHARACTER_STATE_FIELDS` |
| `ideatek:ideatek_snapshot_action_t` | `ideatek_snapshot_action_t` | `ideatek_snapshot_action_t_desc` | `IDEATEK_SNAPSHOT_ACTION_VALUES` |
| `ideatek:ideatek_snapshot_status_t` | `ideatek_snapshot_status_t` | `ideatek_snapshot_status_t_desc` | `IDEATEK_SNAPSHOT_STATUS_VALUES` |
| `ideatek:PhysicsSnapshotRequest` | `ideatek_physics_snapshot_request_t` | `ideatek_physics_snapshot_request_t_desc` | `IDEATEK_PHYSICS_SNAPSHOT_REQUEST_FIELDS` |
| `ideatek:PhysicsSnapshot` | `ideatek_physics_snapshot_t` | `ideatek_physics_snapshot_t_desc` | `IDEATEK_PHYSICS_SNAPSHOT_FIELDS` |
| `ideatek:ideatek_vehicle_kind_t` | `ideatek_vehicle_kind_t` | `ideatek_vehicle_kind_t_desc` | `IDEATEK_VEHICLE_KIND_VALUES` |
| `ideatek:ideatek_wheel_test_t` | `ideatek_wheel_test_t` | `ideatek_wheel_test_t_desc` | `IDEATEK_WHEEL_TEST_VALUES` |
| `ideatek:ideatek_transmission_mode_t` | `ideatek_transmission_mode_t` | `ideatek_transmission_mode_t_desc` | `IDEATEK_TRANSMISSION_MODE_VALUES` |
| `ideatek:Vehicle` | `ideatek_vehicle_t` | `ideatek_vehicle_t_desc` | `IDEATEK_VEHICLE_FIELDS` |
| `ideatek:VehicleEngine` | `ideatek_vehicle_engine_t` | `ideatek_vehicle_engine_t_desc` | `IDEATEK_VEHICLE_ENGINE_FIELDS` |
| `ideatek:VehicleTransmission` | `ideatek_vehicle_transmission_t` | `ideatek_vehicle_transmission_t_desc` | `IDEATEK_VEHICLE_TRANSMISSION_FIELDS` |
| `ideatek:VehicleDifferential` | `ideatek_vehicle_differential_t` | `ideatek_vehicle_differential_t_desc` | `IDEATEK_VEHICLE_DIFFERENTIAL_FIELDS` |
| `ideatek:AntiRollBar` | `ideatek_anti_roll_bar_t` | `ideatek_anti_roll_bar_t_desc` | `IDEATEK_ANTI_ROLL_BAR_FIELDS` |
| `ideatek:ideatek_wheel_flags_t` | `ideatek_wheel_flags_t` | `ideatek_wheel_flags_t_desc` | `IDEATEK_WHEEL_FLAGS_VALUES` |
| `ideatek:Wheel` | `ideatek_wheel_t` | `ideatek_wheel_t_desc` | `IDEATEK_WHEEL_FIELDS` |
| `ideatek:VehicleInput` | `ideatek_vehicle_input_t` | `ideatek_vehicle_input_t_desc` | `IDEATEK_VEHICLE_INPUT_FIELDS` |
| `ideatek:WheelState` | `ideatek_wheel_state_t` | `ideatek_wheel_state_t_desc` | `IDEATEK_WHEEL_STATE_FIELDS` |
| `ideatek:VehicleState` | `ideatek_vehicle_state_t` | `ideatek_vehicle_state_t_desc` | `IDEATEK_VEHICLE_STATE_FIELDS` |
| `ideatek:PoseDrive` | `ideatek_pose_drive_t` | `ideatek_pose_drive_t_desc` | `IDEATEK_POSE_DRIVE_FIELDS` |
| `ideatek:Destructible` | `ideatek_destructible_t` | `ideatek_destructible_t_desc` | `IDEATEK_DESTRUCTIBLE_FIELDS` |
| `ideatek:DestructibleAsset` | `ideatek_destructible_asset_t` | `ideatek_destructible_asset_t_desc` | `IDEATEK_DESTRUCTIBLE_ASSET_FIELDS` |
| `ideatek:ideatek_damage_shape_t` | `ideatek_damage_shape_t` | `ideatek_damage_shape_t_desc` | `IDEATEK_DAMAGE_SHAPE_VALUES` |
| `ideatek:Damage` | `ideatek_damage_t` | `ideatek_damage_t_desc` | `IDEATEK_DAMAGE_FIELDS` |
| `ideatek:BreakEvent` | `ideatek_break_event_t` | `ideatek_break_event_t_desc` | `IDEATEK_BREAK_EVENT_FIELDS` |
| `ideatek:ideatek_bend_t` | `ideatek_bend_t` | `ideatek_bend_t_desc` | `IDEATEK_BEND_VALUES` |
| `ideatek:ideatek_lra_t` | `ideatek_lra_t` | `ideatek_lra_t_desc` | `IDEATEK_LRA_VALUES` |
| `ideatek:SoftBody` | `ideatek_soft_body_t` | `ideatek_soft_body_t_desc` | `IDEATEK_SOFT_BODY_FIELDS` |
| `ideatek:SoftBodyAttachment` | `ideatek_soft_body_attachment_t` | `ideatek_soft_body_attachment_t_desc` | `IDEATEK_SOFT_BODY_ATTACHMENT_FIELDS` |
| `ideatek:SoftBodyReset` | `ideatek_soft_body_reset_t` | `ideatek_soft_body_reset_t_desc` | `IDEATEK_SOFT_BODY_RESET_FIELDS` |
| `ideatek:ideatek_rope_flags_t` | `ideatek_rope_flags_t` | `ideatek_rope_flags_t_desc` | `IDEATEK_ROPE_FLAGS_VALUES` |
| `ideatek:Rope` | `ideatek_rope_t` | `ideatek_rope_t_desc` | `IDEATEK_ROPE_FIELDS` |
| `ideatek:Water` | `ideatek_water_t` | `ideatek_water_t_desc` | `IDEATEK_WATER_FIELDS` |
| `ideatek:Wave` | `ideatek_wave_t` | `ideatek_wave_t_desc` | `IDEATEK_WAVE_FIELDS` |
| `ideatek:WaterSample` | `ideatek_water_sample_t` | `ideatek_water_sample_t_desc` | `IDEATEK_WATER_SAMPLE_FIELDS` |
| `ideatek:Buoyancy` | `ideatek_buoyancy_t` | `ideatek_buoyancy_t_desc` | `IDEATEK_BUOYANCY_FIELDS` |
| `ideatek:ideatek_liquid_flags_t` | `ideatek_liquid_flags_t` | `ideatek_liquid_flags_t_desc` | `IDEATEK_LIQUID_FLAGS_VALUES` |
| `ideatek:ideatek_liquid_technique_t` | `ideatek_liquid_technique_t` | `ideatek_liquid_technique_t_desc` | `IDEATEK_LIQUID_TECHNIQUE_VALUES` |
| `ideatek:ideatek_emitter_shape_t` | `ideatek_emitter_shape_t` | `ideatek_emitter_shape_t_desc` | `IDEATEK_EMITTER_SHAPE_VALUES` |
| `ideatek:Liquid` | `ideatek_liquid_t` | `ideatek_liquid_t_desc` | `IDEATEK_LIQUID_FIELDS` |
| `ideatek:LiquidEmitter` | `ideatek_liquid_emitter_t` | `ideatek_liquid_emitter_t_desc` | `IDEATEK_LIQUID_EMITTER_FIELDS` |
| `ideatek:LiquidParticles` | `ideatek_liquid_particles_t` | `ideatek_liquid_particles_t_desc` | `IDEATEK_LIQUID_PARTICLES_FIELDS` |
| `ideatek:LiquidFoam` | `ideatek_liquid_foam_t` | `ideatek_liquid_foam_t_desc` | `IDEATEK_LIQUID_FOAM_FIELDS` |
| `ideatek:ideatek_gas_flags_t` | `ideatek_gas_flags_t` | `ideatek_gas_flags_t_desc` | `IDEATEK_GAS_FLAGS_VALUES` |
| `ideatek:GasVolume` | `ideatek_gas_volume_t` | `ideatek_gas_volume_t_desc` | `IDEATEK_GAS_VOLUME_FIELDS` |
| `ideatek:GasEmitter` | `ideatek_gas_emitter_t` | `ideatek_gas_emitter_t_desc` | `IDEATEK_GAS_EMITTER_FIELDS` |
| `ideatek:GasGrid` | `ideatek_gas_grid_t` | `ideatek_gas_grid_t_desc` | `IDEATEK_GAS_GRID_FIELDS` |
| `ideatek:GasSample` | `ideatek_gas_sample_t` | `ideatek_gas_sample_t_desc` | `IDEATEK_GAS_SAMPLE_FIELDS` |
| `ideatek:SightSample` | `ideatek_sight_sample_t` | `ideatek_sight_sample_t_desc` | `IDEATEK_SIGHT_SAMPLE_FIELDS` |
| `ideatek:LagHistory` | `ideatek_lag_history_t` | `ideatek_lag_history_t_desc` | `IDEATEK_LAG_HISTORY_FIELDS` |
| `ideatek:ideatek_field_shape_t` | `ideatek_field_shape_t` | `ideatek_field_shape_t_desc` | `IDEATEK_FIELD_SHAPE_VALUES` |
| `ideatek:ideatek_force_kind_t` | `ideatek_force_kind_t` | `ideatek_force_kind_t_desc` | `IDEATEK_FORCE_KIND_VALUES` |
| `ideatek:ideatek_field_target_t` | `ideatek_field_target_t` | `ideatek_field_target_t_desc` | `IDEATEK_FIELD_TARGET_VALUES` |
| `ideatek:ideatek_gravity_mode_t` | `ideatek_gravity_mode_t` | `ideatek_gravity_mode_t_desc` | `IDEATEK_GRAVITY_MODE_VALUES` |
| `ideatek:Wind` | `ideatek_wind_t` | `ideatek_wind_t_desc` | `IDEATEK_WIND_FIELDS` |
| `ideatek:ForceField` | `ideatek_force_field_t` | `ideatek_force_field_t_desc` | `IDEATEK_FORCE_FIELD_FIELDS` |
| `ideatek:GravityArea` | `ideatek_gravity_area_t` | `ideatek_gravity_area_t_desc` | `IDEATEK_GRAVITY_AREA_FIELDS` |
| `ideatek:ideatek_time_mode_t` | `ideatek_time_mode_t` | `ideatek_time_mode_t_desc` | `IDEATEK_TIME_MODE_VALUES` |
| `ideatek:TimeArea` | `ideatek_time_area_t` | `ideatek_time_area_t_desc` | `IDEATEK_TIME_AREA_FIELDS` |
| `ideatek:ideatek_air_mode_t` | `ideatek_air_mode_t` | `ideatek_air_mode_t_desc` | `IDEATEK_AIR_MODE_VALUES` |
| `ideatek:AirArea` | `ideatek_air_area_t` | `ideatek_air_area_t_desc` | `IDEATEK_AIR_AREA_FIELDS` |
| `ideatek:ideatek_gas_face_kind_t` | `ideatek_gas_face_kind_t` | `ideatek_gas_face_kind_t_desc` | `IDEATEK_GAS_FACE_KIND_VALUES` |
| `ideatek:ideatek_gas_face_t` | `ideatek_gas_face_t` | `ideatek_gas_face_t_desc` | `IDEATEK_GAS_FACE_FIELDS` |
| `ideatek:GasFill` | `ideatek_gas_fill_t` | `ideatek_gas_fill_t_desc` | `IDEATEK_GAS_FILL_FIELDS` |
| `ideatek:AirProbe` | `ideatek_air_probe_t` | `ideatek_air_probe_t_desc` | `IDEATEK_AIR_PROBE_FIELDS` |
| `ideatek:SpinArea` | `ideatek_spin_area_t` | `ideatek_spin_area_t_desc` | `IDEATEK_SPIN_AREA_FIELDS` |
| `ideatek:ideatek_contact_work_t` | `ideatek_contact_work_t` | `ideatek_contact_work_t_desc` | `IDEATEK_CONTACT_WORK_FIELDS` |
| `ideatek:ContactWork` | `ideatek_contact_work_list_t` | `ideatek_contact_work_list_t_desc` | `IDEATEK_CONTACT_WORK_LIST_FIELDS` |
| `ideatek:ideatek_particle_set_flags_t` | `ideatek_particle_set_flags_t` | `ideatek_particle_set_flags_t_desc` | `IDEATEK_PARTICLE_SET_FLAGS_VALUES` |
| `ideatek:ideatek_particle_t` | `ideatek_particle_t` | `ideatek_particle_t_desc` | `IDEATEK_PARTICLE_FIELDS` |
| `ideatek:ParticleSet` | `ideatek_particle_set_t` | `ideatek_particle_set_t_desc` | `IDEATEK_PARTICLE_SET_FIELDS` |
| `ideatek:ideatek_solver_t` | `ideatek_solver_t` | `ideatek_solver_t_desc` | `IDEATEK_SOLVER_VALUES` |
| `ideatek:ideatek_phase_request_t` | `ideatek_phase_request_t` | `ideatek_phase_request_t_desc` | `IDEATEK_PHASE_REQUEST_FIELDS` |
| `ideatek:PhaseRequest` | `ideatek_phase_request_list_t` | `ideatek_phase_request_list_t_desc` | `IDEATEK_PHASE_REQUEST_LIST_FIELDS` |
| `ideatek:ideatek_surface_material_t` | `ideatek_surface_material_t` | `ideatek_surface_material_t_desc` | `IDEATEK_SURFACE_MATERIAL_FIELDS` |
| `ideatek:SurfaceMaterial` | `ideatek_surface_material_list_t` | `ideatek_surface_material_list_t_desc` | `IDEATEK_SURFACE_MATERIAL_LIST_FIELDS` |
| `ideatek:ideatek_heat_source_t` | `ideatek_heat_source_t` | `ideatek_heat_source_t_desc` | `IDEATEK_HEAT_SOURCE_FIELDS` |
| `ideatek:HeatSource` | `ideatek_heat_source_list_t` | `ideatek_heat_source_list_t_desc` | `IDEATEK_HEAT_SOURCE_LIST_FIELDS` |
| `ideatek:HeatTaken` | `ideatek_heat_taken_t` | `ideatek_heat_taken_t_desc` | `IDEATEK_HEAT_TAKEN_FIELDS` |

```text
{ IDEATEK_BODY_KIND_STATIC = 0, IDEATEK_BODY_KIND_KINEMATIC = 1, IDEATEK_BODY_KIND_DYNAMIC = 2 }
{ IDEATEK_MOTION_QUALITY_DISCRETE = 0, IDEATEK_MOTION_QUALITY_LINEAR_CAST = 1 }
{ IDEATEK_MASS_MODE_SHAPES = 0, IDEATEK_MASS_MODE_CUSTOM = 1 }
{ IDEATEK_LOCKED_X = 0x01, IDEATEK_LOCKED_Y = 0x02, IDEATEK_LOCKED_Z = 0x04, IDEATEK_LOCKED_ROLL = 0x08, IDEATEK_LOCKED_PITCH = 0x10, IDEATEK_LOCKED_YAW = 0x20 }
{ IDEATEK_BODY_FLAGS_CAN_SLEEP = 0x01, IDEATEK_BODY_FLAGS_ENHANCED_EDGES = 0x02, IDEATEK_BODY_FLAGS_DESTRUCTIBLE = 0x04, IDEATEK_BODY_FLAGS_GYROSCOPIC = 0x08 }
{ ideatek_vec3_t center_of_mass; ideatek_vec3_t inertia; float mass; float gravity_scale; float linear_damping; float angular_damping; float max_linear_speed; float max_angular_speed; float sleep_threshold; float inertia_scale; float drag; ideatek_body_kind_t kind; ideatek_mass_mode_t mass_mode; ideatek_locked_t locked; ideatek_body_flags_t flags; ideatek_motion_quality_t motion_quality; uint8_t velocity_steps; uint8_t position_steps; }
{ ideatek_position_t position; ideatek_quat_t rotation; }
{ ideatek_entity_t body; uint32_t order; bool asleep; }
{ ideatek_vec3_t linear; ideatek_vec3_t angular; bool sleeping; float rate; }
{ ideatek_position_t point; ideatek_vec3_t force; ideatek_vec3_t torque; ideatek_vec3_t impulse; ideatek_vec3_t angular_impulse; }
{ IDEATEK_SHAPE_KIND_SPHERE = 0, IDEATEK_SHAPE_KIND_BOX = 1, IDEATEK_SHAPE_KIND_CAPSULE = 2, IDEATEK_SHAPE_KIND_CYLINDER = 3, IDEATEK_SHAPE_KIND_CONVEX = 4, IDEATEK_SHAPE_KIND_TRIANGLES = 5, IDEATEK_SHAPE_KIND_PLANE = 6, IDEATEK_SHAPE_KIND_HEIGHT_FIELD = 7 }
{ IDEATEK_SHAPE_FLAGS_SENSOR = 0x01, IDEATEK_SHAPE_FLAGS_SENSOR_EVENTS = 0x02, IDEATEK_SHAPE_FLAGS_CONTACT_EVENTS = 0x04, IDEATEK_SHAPE_FLAGS_HIT_EVENTS = 0x08 }
{ ideatek_entity_t body; ideatek_entity_t material; ideatek_entity_t source; uint64_t categories; uint64_t touches; ideatek_vec3_t size; uint32_t part; ideatek_shape_kind_t kind; ideatek_shape_flags_t flags; }
{ bridge_text_t materials; bridge_text_t indices; }
{ IDEATEK_COMBINE_AVERAGE = 0, IDEATEK_COMBINE_MIN = 1, IDEATEK_COMBINE_MULTIPLY = 2, IDEATEK_COMBINE_MAX = 3 }
{ float friction; float restitution; float density; float tensile; float compression; float shear; ideatek_combine_t friction_combine; ideatek_combine_t restitution_combine; }
{ IDEATEK_TOUCH_KIND_BEGAN = 0, IDEATEK_TOUCH_KIND_ENDED = 1 }
{ ideatek_entity_t sensor; ideatek_entity_t visitor; uint32_t order; ideatek_touch_kind_t kind; }
{ ideatek_entity_t a; ideatek_entity_t b; uint32_t order; ideatek_touch_kind_t kind; }
{ ideatek_entity_t a; ideatek_entity_t b; ideatek_position_t point; ideatek_vec3_t normal; float speed; uint32_t order; }
{ IDEATEK_QUERY_KIND_RAY = 0, IDEATEK_QUERY_KIND_SHAPE_CAST = 1, IDEATEK_QUERY_KIND_OVERLAP = 2, IDEATEK_QUERY_KIND_WATER = 3, IDEATEK_QUERY_KIND_GAS = 4, IDEATEK_QUERY_KIND_SIGHT = 5 }
{ IDEATEK_QUERY_MODE_TEST = 0, IDEATEK_QUERY_MODE_CLOSEST = 1, IDEATEK_QUERY_MODE_ALL = 2 }
{ ideatek_position_t origin; uint64_t categories; uint64_t touches; ideatek_quat_t rotation; ideatek_vec3_t translation; ideatek_vec3_t size; uint32_t max; uint32_t tick; ideatek_query_kind_t kind; ideatek_shape_kind_t shape; ideatek_query_mode_t mode; bool back_faces; }
{ ideatek_entity_t query; ideatek_entity_t shape; ideatek_position_t point; ideatek_vec3_t normal; float fraction; ideatek_entity_t material; float uv[2]; float depth; uint32_t face; uint32_t sub_shape; uint32_t order; }
{ IDEATEK_JOINT_FLAGS_COLLIDE_CONNECTED = 0x01, IDEATEK_JOINT_FLAGS_BREAKABLE = 0x02, IDEATEK_JOINT_FLAGS_REPORT = 0x04 }
{ ideatek_entity_t a; ideatek_entity_t b; ideatek_quat_t rotation_a; ideatek_quat_t rotation_b; ideatek_vec3_t position_a; ideatek_vec3_t position_b; float force_threshold; float torque_threshold; ideatek_joint_flags_t flags; }
{ IDEATEK_JOINT_FEATURES_LIMIT = 0x01, IDEATEK_JOINT_FEATURES_SPRING = 0x02, IDEATEK_JOINT_FEATURES_MOTOR = 0x04 }
{ IDEATEK_DRIVE_MODE_OFF = 0, IDEATEK_DRIVE_MODE_VELOCITY = 1, IDEATEK_DRIVE_MODE_POSITION = 2 }
{ IDEATEK_SPRING_MODE_FREQUENCY = 0, IDEATEK_SPRING_MODE_STIFFNESS = 1 }
{ float target; float min_force; float max_force; float spring; float damping; ideatek_drive_mode_t mode; ideatek_spring_mode_t spring_mode; }
{ float lower; float upper; float limit_hertz; float limit_damping; float friction; ideatek_joint_drive_t drive; ideatek_joint_features_t features; }
{ float lower; float upper; float limit_hertz; float limit_damping; float friction; ideatek_joint_drive_t drive; ideatek_joint_features_t features; }
{ float length; float min_length; float max_length; float spring_hertz; float spring_damping; float motor_speed; float max_motor_force; ideatek_joint_features_t features; }
{ float swing_y; float swing_z; float twist_min; float twist_max; }
{ ideatek_vec3_t linear_min; ideatek_vec3_t linear_max; ideatek_vec3_t angular_min; ideatek_vec3_t angular_max; bridge_text_t drives; bridge_text_t friction; }
{ ideatek_entity_t hinge_a; ideatek_entity_t hinge_b; float ratio; }
{ ideatek_entity_t hinge; ideatek_entity_t slider; float ratio; }
{ ideatek_position_t fixed_a; ideatek_position_t fixed_b; float ratio; float min_length; float max_length; }
{ IDEATEK_PATH_ROTATION_FREE = 0, IDEATEK_PATH_ROTATION_TANGENT = 1, IDEATEK_PATH_ROTATION_NORMAL = 2, IDEATEK_PATH_ROTATION_BINORMAL = 3, IDEATEK_PATH_ROTATION_PATH = 4, IDEATEK_PATH_ROTATION_FIXED = 5 }
{ bridge_text_t path; float fraction; float max_friction; ideatek_joint_drive_t drive; ideatek_path_rotation_t rotation; bool looping; }
{ float half_angle; }
{ ideatek_vec3_t linear_velocity; ideatek_vec3_t angular_velocity; float max_force; float max_torque; float linear_hertz; float linear_damping; float max_spring_force; float angular_hertz; float angular_damping; float max_spring_torque; }
{ float lower; float upper; float suspension_hertz; float suspension_damping; float max_steer; float steer_target; ideatek_joint_drive_t spin; }
{ ideatek_entity_t joint; float force; float torque; uint32_t order; }
{ ideatek_vec3_t force; ideatek_vec3_t torque; }
{ IDEATEK_ARTICULATION_JOINT_FIX = 0, IDEATEK_ARTICULATION_JOINT_PRISMATIC = 1, IDEATEK_ARTICULATION_JOINT_REVOLUTE = 2, IDEATEK_ARTICULATION_JOINT_SPHERICAL = 3 }
{ IDEATEK_ARTICULATION_FLAGS_FIX_BASE = 0x01, IDEATEK_ARTICULATION_FLAGS_SELF_COLLISION = 0x02 }
{ float sleep_threshold; uint8_t velocity_steps; uint8_t position_steps; ideatek_articulation_flags_t flags; }
{ ideatek_entity_t articulation; ideatek_entity_t parent; bridge_text_t limits; bridge_text_t drives; float friction; float max_joint_velocity; uint8_t motions[6]; ideatek_articulation_joint_t joint; }
{ bridge_text_t positions; bridge_text_t velocities; uint32_t count; }
{ bridge_text_t positions; bridge_text_t velocities; uint32_t count; }
{ IDEATEK_GROUND_STATE_ON_GROUND = 0, IDEATEK_GROUND_STATE_ON_STEEP_GROUND = 1, IDEATEK_GROUND_STATE_NOT_SUPPORTED = 2, IDEATEK_GROUND_STATE_IN_AIR = 3 }
{ IDEATEK_NON_WALKABLE_SLIDE = 0, IDEATEK_NON_WALKABLE_PREVENT_CLIMB = 1 }
{ IDEATEK_CHARACTER_FLAGS_INNER_BODY = 0x01, IDEATEK_CHARACTER_FLAGS_CAN_PUSH = 0x02, IDEATEK_CHARACTER_FLAGS_CAN_BE_PUSHED = 0x04 }
{ IDEATEK_CHARACTER_COLLISION_SIDES = 0x01, IDEATEK_CHARACTER_COLLISION_UP = 0x02, IDEATEK_CHARACTER_COLLISION_DOWN = 0x04 }
{ ideatek_vec3_t up; float max_slope; float step_up; float stick_down; float min_step_forward; float padding; float predictive_distance; float mass; float max_strength; float drag; ideatek_non_walkable_t non_walkable; ideatek_character_flags_t flags; }
{ ideatek_vec3_t velocity; }
{ ideatek_entity_t ground; ideatek_entity_t ground_material; ideatek_vec3_t ground_normal; ideatek_vec3_t ground_velocity; ideatek_ground_state_t ground_state; ideatek_character_collision_t collision; float rate; }
{ IDEATEK_SNAPSHOT_ACTION_SAVE = 0, IDEATEK_SNAPSHOT_ACTION_RESTORE = 1 }
{ IDEATEK_SNAPSHOT_STATUS_OK = 0, IDEATEK_SNAPSHOT_STATUS_TOO_BIG = 1, IDEATEK_SNAPSHOT_STATUS_FAILED = 2 }
{ uint64_t categories; bridge_text_t data; ideatek_snapshot_action_t action; }
{ bridge_text_t data; ideatek_snapshot_status_t status; }
{ IDEATEK_VEHICLE_KIND_WHEELED = 0, IDEATEK_VEHICLE_KIND_TRACKED = 1, IDEATEK_VEHICLE_KIND_MOTORCYCLE = 2 }
{ IDEATEK_WHEEL_TEST_RAY = 0, IDEATEK_WHEEL_TEST_SPHERE = 1, IDEATEK_WHEEL_TEST_CYLINDER = 2 }
{ IDEATEK_TRANSMISSION_MODE_AUTO = 0, IDEATEK_TRANSMISSION_MODE_MANUAL = 1 }
{ float max_drive_torque; float max_pitch_roll; ideatek_vehicle_kind_t kind; ideatek_wheel_test_t test; }
{ bridge_text_t torque_curve; float max_torque; float min_rpm; float max_rpm; float inertia; float damping; }
{ bridge_text_t gears; bridge_text_t reverse; float shift_up_rpm; float shift_down_rpm; float switch_time; float clutch_release_time; float switch_latency; float clutch_strength; ideatek_transmission_mode_t mode; }
{ ideatek_entity_t vehicle; ideatek_entity_t left; ideatek_entity_t right; float ratio; float split; float limited_slip; float engine_torque_ratio; }
{ ideatek_entity_t vehicle; ideatek_entity_t left; ideatek_entity_t right; float stiffness; }
{ IDEATEK_WHEEL_FLAGS_DRIVEN = 0x01, IDEATEK_WHEEL_FLAGS_STEERS = 0x02 }
{ ideatek_entity_t vehicle; ideatek_vec3_t position; ideatek_vec3_t suspension_direction; ideatek_vec3_t steering_axis; float radius; float width; float suspension_min; float suspension_max; float spring_hertz; float spring_damping; float max_steer; float max_brake_torque; float max_hand_brake_torque; ideatek_vec3_t wheel_up; ideatek_vec3_t wheel_forward; bridge_text_t longitudinal_friction; bridge_text_t lateral_friction; ideatek_wheel_flags_t flags; uint8_t track; }
{ float forward; float right; float brake; float hand_brake; float clutch; float left_ratio; float right_ratio; int32_t gear; }
{ float steer_angle; float angle; float angular_velocity; float suspension_length; float longitudinal_slip; float lateral_slip; ideatek_entity_t contact_body; ideatek_entity_t contact_material; bool contact; bool hit_hard_point; }
{ float rpm; float clutch; int32_t gear; }
{ float position_strength; float orientation_strength; }
{ ideatek_entity_t interior; ideatek_entity_t asset; }
{ bridge_text_t chunks; bridge_text_t bonds; ideatek_entity_t mesh; uint32_t chunk_count; uint32_t bond_count; uint32_t revision; }
{ IDEATEK_DAMAGE_SHAPE_RADIAL = 0, IDEATEK_DAMAGE_SHAPE_CAPSULE = 1, IDEATEK_DAMAGE_SHAPE_SHEAR = 2, IDEATEK_DAMAGE_SHAPE_TRIANGLE = 3, IDEATEK_DAMAGE_SHAPE_IMPACT = 4 }
{ ideatek_position_t point; ideatek_position_t point_b; ideatek_position_t point_c; ideatek_vec3_t normal; float min_radius; float radius; float pressure; uint32_t order; ideatek_damage_shape_t shape; }
{ ideatek_entity_t body; ideatek_entity_t piece; ideatek_position_t point; ideatek_vec3_t excess_force; bridge_text_t chunks; uint32_t order; }
{ IDEATEK_BEND_NONE = 0, IDEATEK_BEND_DISTANCE = 1, IDEATEK_BEND_DIHEDRAL = 2 }
{ IDEATEK_LRA_NONE = 0, IDEATEK_LRA_EUCLIDEAN = 1, IDEATEK_LRA_GEODESIC = 2 }
{ ideatek_entity_t source; uint64_t categories; uint64_t touches; float edge_compliance; float shear_compliance; float bend_compliance; float vertex_mass; float linear_damping; float friction; float gravity_scale; float vertex_radius; float pressure; float restitution; float volume_compliance; float lra_scale; float skin_distance_scale; uint32_t iterations; ideatek_bend_t bend; ideatek_lra_t lra; bool double_sided; }
{ ideatek_entity_t soft_body; ideatek_entity_t body; ideatek_vec3_t offset; uint32_t vertex; }
{ ideatek_entity_t body; uint32_t order; }
{ IDEATEK_ROPE_FLAGS_STIFF = 0x01, IDEATEK_ROPE_FLAGS_COLLIDE = 0x02 }
{ ideatek_entity_t start; ideatek_entity_t end; uint64_t categories; uint64_t touches; ideatek_vec3_t start_offset; ideatek_vec3_t end_offset; float length; float width; float gravity_scale; float friction; float stretch_compliance; float bend_compliance; uint32_t segments; uint32_t sides; ideatek_rope_flags_t flags; }
{ ideatek_vec3_t flow; float density; }
{ ideatek_entity_t water; ideatek_vec3_t direction; float wavelength; float amplitude; float steepness; }
{ ideatek_entity_t query; ideatek_entity_t water; ideatek_position_t point; ideatek_vec3_t normal; ideatek_vec3_t velocity; float depth; uint32_t order; }
{ float linear_drag; float angular_drag; }
{ IDEATEK_LIQUID_FLAGS_PUSH_BODIES = 0x01, IDEATEK_LIQUID_FLAGS_FOAM = 0x02 }
{ IDEATEK_LIQUID_TECHNIQUE_SPH = 0, IDEATEK_LIQUID_TECHNIQUE_FLIP = 1, IDEATEK_LIQUID_TECHNIQUE_PBD = 2 }
{ IDEATEK_EMITTER_SHAPE_SHAPE = 0, IDEATEK_EMITTER_SHAPE_BOX = 1, IDEATEK_EMITTER_SHAPE_DISC = 2, IDEATEK_EMITTER_SHAPE_POINT = 3 }
{ uint64_t categories; uint64_t touches; float density; float viscosity; float resolution; float surface_tension; float cohesion; float adhesion; float vorticity; uint32_t max_particles; uint32_t phase; ideatek_liquid_flags_t flags; ideatek_liquid_technique_t technique; }
{ ideatek_entity_t liquid; ideatek_vec3_t velocity; float rate; ideatek_vec3_t size; float start; float end; float jitter; ideatek_emitter_shape_t shape; }
{ bridge_text_t positions; bridge_text_t velocities; uint32_t count; }
{ bridge_text_t positions; bridge_text_t lifetimes; uint32_t count; }
{ IDEATEK_GAS_FLAGS_COLLIDE = 0x01, IDEATEK_GAS_FLAGS_PUSH_BODIES = 0x02, IDEATEK_GAS_FLAGS_PRESSURE = 0x04 }
{ bridge_text_t species; bridge_text_t reactions; float cell_size; float steps_per_second; float ambient_temperature; float cooling; float vorticity; uint32_t species_count; uint32_t reaction_count; ideatek_gas_flags_t flags; bridge_text_t faces; }
{ ideatek_entity_t volume; ideatek_vec3_t velocity; ideatek_vec3_t size; float rate; float temperature; float heat; float couple_rate; uint32_t species; ideatek_emitter_shape_t shape; }
{ ideatek_entity_t temperature; bridge_text_t amounts; uint32_t count; }
{ ideatek_entity_t query; ideatek_entity_t volume; bridge_text_t amounts; ideatek_vec3_t velocity; float temperature; float pressure; uint32_t order; }
{ ideatek_entity_t query; float transmittance; uint32_t order; }
{ float seconds; }
{ IDEATEK_FIELD_SHAPE_INFINITE = 0, IDEATEK_FIELD_SHAPE_SPHERE = 1, IDEATEK_FIELD_SHAPE_BOX = 2, IDEATEK_FIELD_SHAPE_CYLINDER = 3 }
{ IDEATEK_FORCE_KIND_ATTRACTOR = 0, IDEATEK_FORCE_KIND_VORTEX = 1, IDEATEK_FORCE_KIND_DIRECTIONAL = 2, IDEATEK_FORCE_KIND_NOISE = 3, IDEATEK_FORCE_KIND_DRAG = 4, IDEATEK_FORCE_KIND_VECTOR_FIELD = 5 }
{ IDEATEK_FIELD_TARGET_FORCE = 0, IDEATEK_FIELD_TARGET_VELOCITY = 1, IDEATEK_FIELD_TARGET_STRAIN = 2, IDEATEK_FIELD_TARGET_SLEEP = 3, IDEATEK_FIELD_TARGET_WAKE = 4 }
{ IDEATEK_GRAVITY_MODE_REPLACE = 0, IDEATEK_GRAVITY_MODE_ADD = 1 }
{ ideatek_vec3_t size; float inner; float speed; float gust_min; float gust_max; float gust_frequency; float falloff; ideatek_field_shape_t shape; }
{ ideatek_entity_t texture; ideatek_vec3_t size; float inner; float acceleration; float frequency; float falloff; ideatek_force_kind_t kind; ideatek_field_shape_t shape; ideatek_field_target_t target; }
{ ideatek_vec3_t size; float gravity; float inner; float falloff; int32_t priority; ideatek_gravity_mode_t mode; ideatek_field_shape_t shape; bool point; }
{ IDEATEK_TIME_MODE_REPLACE = 0, IDEATEK_TIME_MODE_MULTIPLY = 1 }
{ ideatek_vec3_t size; float rate; float inner; float falloff; int32_t priority; ideatek_time_mode_t mode; ideatek_field_shape_t shape; }
{ IDEATEK_AIR_MODE_REPLACE = 0, IDEATEK_AIR_MODE_MULTIPLY = 1 }
{ ideatek_vec3_t size; float pressure; float inner; float falloff; int32_t priority; ideatek_air_mode_t mode; ideatek_field_shape_t shape; }
{ IDEATEK_GAS_FACE_KIND_WALL = 0, IDEATEK_GAS_FACE_KIND_SPACE = 1, IDEATEK_GAS_FACE_KIND_GAS = 2 }
{ ideatek_gas_face_kind_t kind; uint32_t species; float temperature; float pressure; }
{ ideatek_entity_t volume; ideatek_vec3_t size; float temperature; float pressure; uint32_t species; ideatek_field_shape_t shape; }
{ float pressure; float temperature; float density; bool covered; }
{ ideatek_vec3_t size; float rate; float inner; float falloff; int32_t priority; ideatek_field_shape_t shape; }
{ ideatek_entity_t a; ideatek_entity_t b; ideatek_position_t point; float energy; float work; uint32_t points; uint32_t chunk_a; uint32_t chunk_b; }
{ bridge_text_t records; uint32_t count; }
{ IDEATEK_PARTICLE_SET_FLAGS_TEARS = 0x01 }
{ float position[3]; float radius; float velocity[3]; float mass; }
{ bridge_text_t particles; uint32_t count; uint32_t cap; ideatek_entity_t material; ideatek_particle_set_flags_t flags; }
{ IDEATEK_SOLVER_RIGID = 0, IDEATEK_SOLVER_SOFT = 1, IDEATEK_SOLVER_LIQUID = 2, IDEATEK_SOLVER_GAS = 3 }
{ ideatek_entity_t entity; uint32_t index; ideatek_solver_t solver; bool particle; uint32_t species; }
{ bridge_text_t records; uint32_t count; }
{ ideatek_entity_t body; ideatek_entity_t material; uint32_t chunk; }
{ bridge_text_t records; uint32_t count; }
{ ideatek_position_t position; float area; float temperature; float rate; float smoke; float ash; uint32_t smoke_species; }
{ bridge_text_t records; uint32_t count; }
{ bridge_text_t heats; uint32_t count; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_quat_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_position_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_body_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_motion_quality_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
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
| `ideatek:ideatek_drive_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_spring_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_joint_drive_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_path_rotation_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_articulation_joint_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_articulation_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_ground_state_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_non_walkable_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_character_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_character_collision_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_snapshot_action_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_snapshot_status_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vehicle_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_wheel_test_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_transmission_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_wheel_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_damage_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_bend_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_lra_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_rope_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_liquid_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_liquid_technique_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_emitter_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gas_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_field_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_force_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_field_target_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gravity_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_time_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_air_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Body` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:KinematicTarget` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WakeEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Motion` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Push` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Shape` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ShapeMaterials` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
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
| `ideatek:GearJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:RackAndPinionJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PulleyJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PathJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ConeJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MotorJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WheelJoint` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:NoCollideJoint` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BodyJointEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BodyJointState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Articulation` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ArticulationLink` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ArticulationState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ArticulationCommand` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Character` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:CharacterMove` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:CharacterState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PhysicsSnapshotRequest` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PhysicsSnapshot` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Vehicle` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VehicleEngine` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VehicleTransmission` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VehicleDifferential` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AntiRollBar` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Wheel` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VehicleInput` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WheelState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VehicleState` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PoseDrive` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Destructible` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DestructibleAsset` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Damage` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:BreakEvent` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoftBody` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoftBodyAttachment` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SoftBodyReset` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Rope` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Water` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Wave` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:WaterSample` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Buoyancy` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Liquid` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LiquidEmitter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LiquidParticles` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LiquidFoam` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GasVolume` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GasEmitter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GasGrid` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GasSample` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SightSample` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LagHistory` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Wind` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ForceField` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GravityArea` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TimeArea` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AirArea` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gas_face_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gas_face_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:GasFill` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AirProbe` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SpinArea` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_contact_work_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ContactWork` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_particle_set_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_particle_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleSet` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_solver_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_phase_request_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:PhaseRequest` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_surface_material_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:SurfaceMaterial` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_heat_source_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:HeatSource` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:HeatTaken` | `BRIDGE_STRUCT` | `BRIDGE_SINGLETON` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | The bodies, the shapes, the materials, the queries, the joints, the requests and their options, the areas, `ideatek:GasFill`, `ideatek:AirProbe` and `ideatek:ParticleSet` | The types of the fields |
| The physics module | `ideatek:Transform`, `ideatek:Motion`, the events, the states, the snapshots, the particles, the grids, `ideatek:Mesh`, `ideatek:MeshInstance`, `ideatek:Texture`, `ideatek:ContactWork`, `ideatek:HeatTaken`, `ideatek:AirProbe` and `ideatek:ParticleSet` | Each type that the game writes, `ideatek:PhaseRequest`, `ideatek:SurfaceMaterial`, `ideatek:HeatSource`, `ideatek:Tick`, and the types of the fields |
| A module of the matter | `ideatek:PhaseRequest`, `ideatek:SurfaceMaterial` and `ideatek:HeatSource` | `ideatek:ContactWork`, `ideatek:HeatTaken`, `ideatek:AirProbe`, `ideatek:ParticleSet`, `ideatek:DestructibleAsset`, `ideatek:PhysicsMaterial`, `ideatek:Body`, and the types of the fields |
| The render module | No type. | `ideatek:Water`, `ideatek:Wave`, `ideatek:LiquidParticles`, `ideatek:LiquidFoam`, `ideatek:GasVolume`, `ideatek:GasGrid`, and the types of the fields |

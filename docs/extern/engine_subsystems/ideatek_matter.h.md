# include/engine_subsystems/ideatek_matter.h

`include/engine_subsystems/ideatek_matter.h` gives the types of the matter: the families and the materials of the bodies, and the state of matter of each chunk.
`readme.md`, *The contracts*, tells the rules of each contract.
A material names the physics materials of its phases, and a particle set is a type of the physics part. So the header includes `ideatek_physics.h`.

## The roles

- The game is the author. It writes the families and the materials as resources, and the first state of each body.
- The matter module applies the laws of heat, of burning, of phase and of charge in each fixed step. It writes the state of each body in place.
- The physics module gives the work of the contacts, and moves the chunks and the particles between its solvers: `docs/extern/engine_subsystems/ideatek_physics.h.md`.
- The matter module never writes a type of the physics part, except the requests of the physics part that this document names.

## The families and the materials

A family is a class of substance, for example the metals. Its members share the values of `ideatek:MatterFamily`.
A material is 1 substance, with its family and its own values: `ideatek:MatterMaterial`.
The game or its tools derive the values and write them. The matter module only reads them.

A material names 1 `ideatek:PhysicsMaterial` for each phase. A phase with an entity of 0 is a phase that the material cannot reach.
So the density, the friction and the strengths of each phase stay in the physics part.

## The state of a body

A body with matter is an entity with `ideatek:Matter`. It holds 1 record of `ideatek_matter_chunk_t` for each chunk of the body.
A body with no `ideatek:DestructibleAsset` has 1 chunk. A destructible body has 1 record for each chunk of its asset, in the order of the asset.

The game attaches `ideatek:Matter` with the first state of each chunk. Then the matter module writes the records in place in each step.
A write in place raises no event. A game that changes a state attaches the component again, and the module takes the new records.

The charge is 1 value for each body, since a conductor takes 1 potential at once.

## The heat

Each chunk holds 1 temperature, in K. Heat moves along 5 paths in each step:

1. Inside a body, from chunk to chunk across each bond of its asset. The flow is `thermal_conductivity` times the area of the bond, divided by the distance of the 2 centers.
2. Between 2 bodies that touch, through each record of `ideatek:ContactWork`. The flow is the conductance of the 2 families, times `contact_area` for each point, times the difference.
3. Between a chunk and the gas around it, through `ideatek:HeatSource` and `ideatek:HeatTaken` of the physics part. Where no gas grid covers a chunk, the chunk moves toward the temperature of its area at `air_rate`.
4. By light: each chunk gives `emissivity` times `sigma` times its area times (T^4 - S^4). T is its temperature, and S is its sky.
5. From work: the hits and the slides of `ideatek:ContactWork` heat the 2 chunks at the contact, half each.

The sky of a chunk is the gas of its cell. In a vacuum, it is `space_temperature`.
A change of temperature is the heat divided by the mass and `specific_heat` of the chunk. The mass comes from the volume of the chunk and the density of its phase.

With `radiation_between`, a chunk above `source_temperature` also warms the chunks of other bodies within the radius where its flux falls to `flux_bound`.
The module casts 1 ray for each pair, so a wall stops the heat. It updates these pairs `radiation_rate` times in each second.

## The phases

A chunk is in 1 solver of the physics part: `ideatek_solver_t`. Its phase follows its temperature and the points of its material:

| From | To | When |
| --- | --- | --- |
| Solid | Soft | The temperature rises above `softening_point`. |
| Soft | Liquid | The temperature rises above `melting_point`. |
| Liquid | Gas | The temperature rises above `boiling_point`. |
| Gas | Liquid, soft, solid | The temperature falls below the same points, in the other order. |

A phase that the material cannot reach is skipped. When a phase changes, the matter module writes `ideatek:PhaseRequest` of the physics part.
The physics module moves the chunk to the solver of the new phase: `docs/extern/engine_subsystems/ideatek_physics.h.md`, *The requests of the phases*.

## The burn

A chunk of a material with `flammable` burns when its temperature is above `fire_temperature` and the pressure of its gas is above 0.
A burn eats `burn_rate` kg in each second for each m^2 of the surface of the chunk. Each kg gives `burn_heat` J to the chunk and to its gas.
The family gives the parts of each kg that become smoke and ash: `smoke_share` and `ash_share`. The rest leaves as a gas that the module does not follow.

The smoke goes into the gas grid through `ideatek:HeatSource`. The ash becomes particles of the particle set of the level.
A chunk with no mass left goes, and its body breaks as the destruction of the physics part tells. At most `max_fires` chunks burn at 1 time, and the log gets 1 line above it.

## The charge

In each step, a charge flows through each record of `ideatek:ContactWork`. The conductance is that of the 2 materials and of their coatings.
A body that touches the ground loses its charge to it. The ground is each static body and the terrain, with a charge of 0.
A body whose material has an `electric_conductivity` of 0 keeps its charge.

With `arcs`, a body whose voltage is above `arc_voltage` jumps to a conductor within its breakdown distance. That distance is its voltage divided by the breakdown field of its gas.
The breakdown field is `breakdown_field` at the pressure of `breakdown_pressure`, and it follows the law of Paschen with `paschen_a` and `paschen_b` at another pressure. In a vacuum, no arc jumps.
The module casts 1 ray for each candidate. An arc carries the charge at once, and gives light and heat along its path.

## The coating

Each chunk holds 1 coating: the entity of its material and its mass for each m^2, or an entity of 0 for none.
A coating comes from a liquid that touches the chunk, or from a gas that condenses on it. It also comes from particles at rest on it.

A coating of the same material adds its mass. With `coating_rule` at `IDEATEK_COATING_RULE_LARGER`, a coating of another material keeps the larger mass. The smaller one leaves as particles.
A hit, a slide or a wind whose work passes `adhesion` of the coating tears a mass of it into particles.
The friction of the surface is that of the coating: the matter module writes `ideatek:SurfaceMaterial` of the physics part.

## The particles

A particle set of the physics part, `ideatek:ParticleSet`, can carry `ideatek:ParticleMatter`. It holds the material and the temperature of each particle.
The matter module heats and burns each particle as a chunk of 1 particle. A particle of a family with `particles_burn` at `false` does not burn.
A particle that cools and comes to rest becomes a coating of the chunk under it. A particle that boils or burns out goes into the gas grid.

## The settings

| Setting | Default | Range | Value |
| --- | --- | --- | --- |
| `sigma` | 5.670374e-8 | Above 0 | The constant of Stefan and Boltzmann, in W / (m^2 K^4). |
| `space_temperature` | 2.7 | 0 or more | The temperature of the sky in a vacuum, in K. |
| `contact_area` | 0.0001 | Above 0 | The area of 1 point of contact, in m^2. |
| `radiation_between` | `false` | `true` or `false` | The heat by light between bodies. |
| `source_temperature` | 800 | Above 0 | The temperature above which a chunk warms other bodies by light, in K. |
| `flux_bound` | 10 | Above 0 | The flux below which a chunk warms no other body, in W / m^2. |
| `radiation_rate` | 10 | 1 to 1000 | The updates of the heat by light between bodies in each second. |
| `arcs` | `false` | `true` or `false` | The jumps of charge across a gap. |
| `arc_voltage` | 1000 | Above 0 | The voltage above which a body can jump, in V. |
| `breakdown_field` | 3e6 | Above 0 | The field at which the gas breaks down at `breakdown_pressure`, in V / m. |
| `breakdown_pressure` | 101325 | Above 0 | The pressure of `breakdown_field`, in Pa. |
| `paschen_a` | 11.25 | Above 0 | The constant A of the law of Paschen, in 1 / (Pa m). |
| `paschen_b` | 273.8 | Above 0 | The constant B of the law of Paschen, in V / (Pa m). |
| `coating_rule` | `IDEATEK_COATING_RULE_LARGER` | An item of `ideatek_coating_rule_t` | What a coating of another material does. |
| `max_fires` | 256 | 1 to 1048576 | The chunks that burn at 1 time. |
| `max_sources` | 4096 | 1 to 1048576 | The records of `ideatek:HeatSource` in 1 step. |

The defaults of `radiation_between` and `arcs` are `false` until a bench measures their cost on the reference hardware.
The defaults of `sigma`, of the breakdown and of Paschen are the values of physics for air at sea level.
The other defaults are typical values: `air_rate` is the flow of still air, and `contact_area` is 1 cm^2. A bench on the reference hardware can set each of them.

## ideatek:ideatek_coating_rule_t

`ideatek_coating_rule_t` tells what a coating of another material does. It is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_COATING_RULE_LARGER` | The coating with the larger mass stays. |
| 1 | `IDEATEK_COATING_RULE_NEWER` | The new coating takes the place of the old one. |
| 2 | `IDEATEK_COATING_RULE_OLDER` | The old coating stays. |

The coating that goes leaves as particles in each rule.

## ideatek:MatterFamily

`ideatek:MatterFamily` holds the values that the materials of a family share, on a resource entity.
`ideatek_matter_family_t` takes 32 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `thermal_conductivity` | `float` | 0 | 1 | The flow of heat through the matter, in W / (m K). |
| `specific_heat` | `float` | 4 | 1000 | The heat that warms 1 kg by 1 K, in J. |
| `emissivity` | `float` | 8 | 0.9 | The part of the light of a black body that the surface gives, from 0 to 1. |
| `air_rate` | `float` | 12 | 10 | The flow of heat to the area where no gas grid covers the chunk, in W / (m^2 K). |
| `volume_keep` | `float` | 16 | 1 | How much a soft chunk keeps its volume, from 0 to 1. |
| `smoke_share` | `float` | 20 | 0 | The part of a burned kg that becomes smoke. |
| `ash_share` | `float` | 24 | 0 | The part of a burned kg that becomes ash. |
| `particles_burn` | `bool` | 28 | `true` | `true` if the particles of the family burn where their material burns. |

Bytes 29 to 31 hold no field.

## ideatek:MatterMaterial

`ideatek:MatterMaterial` is 1 substance, on a resource entity.
`ideatek_matter_material_t` takes 80 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `family` | `ideatek_entity_t` | 0 | 0 | The entity of its `ideatek:MatterFamily`. |
| `solid` | `ideatek_entity_t` | 8 | 0 | The `ideatek:PhysicsMaterial` of the solid phase, or 0. |
| `soft` | `ideatek_entity_t` | 16 | 0 | The `ideatek:PhysicsMaterial` of the soft phase, or 0. |
| `liquid` | `ideatek_entity_t` | 24 | 0 | The `ideatek:PhysicsMaterial` of the liquid phase, or 0. |
| `gas` | `ideatek_entity_t` | 32 | 0 | The `ideatek:PhysicsMaterial` of the gas phase, or 0. |
| `softening_point` | `float` | 40 | 0 | The temperature above which a solid becomes soft, in K. |
| `melting_point` | `float` | 44 | 0 | The temperature above which it becomes liquid, in K. |
| `boiling_point` | `float` | 48 | 0 | The temperature above which it becomes gas, in K. |
| `fire_temperature` | `float` | 52 | 0 | The temperature above which a flammable material burns, in K. |
| `burn_rate` | `float` | 56 | 0 | The kg that burn in each second for each m^2 of surface. |
| `burn_heat` | `float` | 60 | 0 | The heat that 1 burned kg gives, in J. |
| `electric_conductivity` | `float` | 64 | 0 | The conductivity, in S / m. |
| `adhesion` | `float` | 68 | 0 | The work for each m^2 that tears a coating of this material off a surface, in J / m^2. |
| `flammable` | `bool` | 72 | `false` | `true` if the material burns. |

Bytes 73 to 79 hold no field.

## ideatek:ideatek_matter_chunk_t

`ideatek_matter_chunk_t` is the state of 1 chunk, in the bytes of `ideatek:Matter`.
`ideatek_matter_chunk_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `temperature` | `float` | 0 | The temperature, in K. |
| `coating_mass` | `float` | 4 | The mass of the coating for each m^2, in kg. |
| `coating` | `ideatek_entity_t` | 8 | The `ideatek:MatterMaterial` of the coating, or 0 for none. |
| `phase` | `ideatek_solver_t` | 16 | The solver of its phase. |
| `burning` | `bool` | 17 | `true` while the chunk burns. |

Bytes 18 to 23 hold no field.

## ideatek:Matter

`ideatek:Matter` is the matter of a body: *The state of a body*.
`ideatek_matter_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `material` | `ideatek_entity_t` | 0 | 0 | The `ideatek:MatterMaterial` of the body. |
| `chunks` | `bridge_text_t` | 8 | No default | The records of `ideatek_matter_chunk_t`, 1 for each chunk. |
| `chunk_count` | `uint32_t` | 16 | No default | The records in `chunks`. |
| `charge` | `float` | 20 | 0 | The charge of the body, in C. |

## ideatek:ideatek_particle_state_t

`ideatek_particle_state_t` is the state of 1 particle, in the bytes of `ideatek:ParticleMatter`.
`ideatek_particle_state_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `temperature` | `float` | 0 | The temperature, in K. |
| `material` | `uint32_t` | 4 | The index of its material in `materials`. |

## ideatek:ParticleMatter

`ideatek:ParticleMatter` is the matter of the particles of 1 `ideatek:ParticleSet`: *The particles*.
`ideatek_particle_matter_t` takes 24 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `materials` | `bridge_text_t` | 0 | No default | The entities of the `ideatek:MatterMaterial` of the particles. |
| `states` | `bridge_text_t` | 8 | No default | The records of `ideatek_particle_state_t`, 1 for each particle, in the order of the set. |
| `material_count` | `uint32_t` | 16 | No default | The entities in `materials`. |
| `state_count` | `uint32_t` | 20 | No default | The records in `states`. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_coating_rule_t` | `ideatek_coating_rule_t` | `ideatek_coating_rule_t_desc` | `IDEATEK_COATING_RULE_VALUES` |
| `ideatek:MatterFamily` | `ideatek_matter_family_t` | `ideatek_matter_family_t_desc` | `IDEATEK_MATTER_FAMILY_FIELDS` |
| `ideatek:MatterMaterial` | `ideatek_matter_material_t` | `ideatek_matter_material_t_desc` | `IDEATEK_MATTER_MATERIAL_FIELDS` |
| `ideatek:ideatek_matter_chunk_t` | `ideatek_matter_chunk_t` | `ideatek_matter_chunk_t_desc` | `IDEATEK_MATTER_CHUNK_FIELDS` |
| `ideatek:Matter` | `ideatek_matter_t` | `ideatek_matter_t_desc` | `IDEATEK_MATTER_FIELDS` |
| `ideatek:ideatek_particle_state_t` | `ideatek_particle_state_t` | `ideatek_particle_state_t_desc` | `IDEATEK_PARTICLE_STATE_FIELDS` |
| `ideatek:ParticleMatter` | `ideatek_particle_matter_t` | `ideatek_particle_matter_t_desc` | `IDEATEK_PARTICLE_MATTER_FIELDS` |

```text
{ IDEATEK_COATING_RULE_LARGER = 0, IDEATEK_COATING_RULE_NEWER = 1, IDEATEK_COATING_RULE_OLDER = 2 }
{ float thermal_conductivity; float specific_heat; float emissivity; float air_rate; float volume_keep; float smoke_share; float ash_share; bool particles_burn; }
{ ideatek_entity_t family; ideatek_entity_t solid; ideatek_entity_t soft; ideatek_entity_t liquid; ideatek_entity_t gas; float softening_point; float melting_point; float boiling_point; float fire_temperature; float burn_rate; float burn_heat; float electric_conductivity; float adhesion; bool flammable; }
{ float temperature; float coating_mass; ideatek_entity_t coating; ideatek_solver_t phase; bool burning; }
{ ideatek_entity_t material; bridge_text_t chunks; uint32_t chunk_count; float charge; }
{ float temperature; uint32_t material; }
{ bridge_text_t materials; bridge_text_t states; uint32_t material_count; uint32_t state_count; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_coating_rule_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MatterFamily` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MatterMaterial` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_matter_chunk_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Matter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_particle_state_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ParticleMatter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:MatterFamily`, `ideatek:MatterMaterial`, `ideatek:Matter` and `ideatek:ParticleMatter` | The types of the fields |
| The matter module | `ideatek:Matter`, `ideatek:ParticleMatter`, `ideatek:PhaseRequest`, `ideatek:SurfaceMaterial` and `ideatek:HeatSource` | `ideatek:MatterFamily`, `ideatek:MatterMaterial`, `ideatek:ContactWork`, `ideatek:HeatTaken`, `ideatek:AirProbe`, `ideatek:ParticleSet`, `ideatek:DestructibleAsset`, `ideatek:PhysicsMaterial`, `ideatek:Body`, `ideatek:Tick`, and the types of the fields |
| A reader | No type. | `ideatek:Matter`, `ideatek:ParticleMatter`, and the types of the fields |

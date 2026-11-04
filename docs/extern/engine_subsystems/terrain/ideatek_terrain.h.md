# include/engine_subsystems/terrain/ideatek_terrain.h

`include/engine_subsystems/terrain/ideatek_terrain.h` gives the types of the terrain of volumes: chunks of cells, materials and edits.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the space part: an edit has a position and a size.

## The roles

- The game writes the terrain, its materials, the edits and the fills.
- The terrain module alone writes the chunks. It applies the commands in order and meshes each chunk that changed.
- The render module, the physics module and the navigation module read the meshes and the shapes that the terrain module writes. They need no other path.

## The chunks

A terrain is cut into chunks of `chunk_cells` cells on each axis. A chunk is at `x`, `y` and `z` in chunks from the place of the terrain.
`bytes` of a chunk holds `chunk_cells` to the power 3 cells, X first, then Y, then Z. Each cell is 2 bytes: an `int8_t` density, then a `uint8_t` material.
A density below 0 is solid. The density is the signed distance to the surface, as in Transvoxel.

## The commands

An edit changes the cells inside its shape. ADD and SUBTRACT change the density, PAINT changes the material, and SMOOTH evens the density.
A fill writes the cells of 1 chunk, for a generator or a level file.
The commands are events, and the module applies them in their order. Then it raises the `revision` of each chunk that changed.
A command replicates as an event of the networking part, and a chunk saves as a chunk of the persistence part.

## The meshes

For each chunk, the module writes a mesh of the rendering part with an `ideatek:MeshInstance`. The material of each cell picks the material entity of its index.
With COLLIDE, the module also writes a static body and a shape of the kind TRIANGLES. With NAVIGATE, the navigation reads that shape as a source.
The seams between the levels of detail are a matter of the module.

## The defaults

A chunk of 16 cells is the block of godot_voxel and the section of Minecraft. No reference gives a size of a cell that fits each game, so the game sets it.

## ideatek:ideatek_terrain_flags_t

`ideatek_terrain_flags_t` names the options of a terrain.
`ideatek_terrain_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_TERRAIN_FLAGS_COLLIDE` | The terrain has shapes for the physics. |
| 0x02 | `IDEATEK_TERRAIN_FLAGS_NAVIGATE` | The navigation reads the terrain. |

## ideatek:Terrain

`ideatek:Terrain` makes a terrain of volumes at the place of its entity.
`ideatek_terrain_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `cell_size` | `float` | 0 | No default | The width of a cell. |
| `chunk_cells` | `uint32_t` | 4 | 16 | The cells of a chunk on each axis. |
| `flags` | `ideatek_terrain_flags_t` | 8 | COLLIDE and NAVIGATE | The options. |

Bytes 9 to 11 hold no field.

## ideatek:TerrainMaterial

`ideatek:TerrainMaterial` maps 1 index of the cells to its materials.
`ideatek_terrain_material_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | The terrain. |
| `material` | `ideatek_entity_t` | 8 | The material of the rendering. |
| `physics_material` | `ideatek_entity_t` | 16 | The material of the physics. |
| `index` | `uint8_t` | 24 | The index of the material in the cells. |

Bytes 25 to 31 hold no field.

## ideatek:TerrainChunk

`ideatek:TerrainChunk` is 1 chunk of a terrain. Only the terrain module writes it.
`ideatek_terrain_chunk_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | The terrain. |
| `bytes` | `bridge_text_t` | 8 | The cells: *The chunks*. |
| `x` | `int32_t` | 16 | The place on X, in chunks. |
| `y` | `int32_t` | 20 | The place on Y, in chunks. |
| `z` | `int32_t` | 24 | The place on Z, in chunks. |
| `revision` | `uint32_t` | 28 | Raised at each change. |

## ideatek:ideatek_terrain_shape_t

`ideatek_terrain_shape_t` tells the shape of an edit.
`ideatek_terrain_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TERRAIN_SHAPE_SPHERE` | A sphere of radius `size.x`. |
| 1 | `IDEATEK_TERRAIN_SHAPE_BOX` | A box of half extents `size`. |

## ideatek:ideatek_terrain_op_t

`ideatek_terrain_op_t` tells what an edit does.
`ideatek_terrain_op_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TERRAIN_OP_ADD` | Add matter. |
| 1 | `IDEATEK_TERRAIN_OP_SUBTRACT` | Remove matter. |
| 2 | `IDEATEK_TERRAIN_OP_PAINT` | Change the material. |
| 3 | `IDEATEK_TERRAIN_OP_SMOOTH` | Smooth the surface. |

## ideatek:TerrainEdit

`ideatek:TerrainEdit` changes the cells in a shape. The game spawns it as an event.
`ideatek_terrain_edit_t` takes 56 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | No default | The terrain. |
| `center` | `ideatek_position_t` | 8 | No default | The center, in the world. |
| `size` | `ideatek_vec3_t` | 32 | No default | The size of the shape. |
| `strength` | `float` | 44 | 1 | The strength, from 0 to 1. |
| `order` | `uint32_t` | 48 | No default | The rank in the round. |
| `shape` | `ideatek_terrain_shape_t` | 52 | No default | The shape. |
| `op` | `ideatek_terrain_op_t` | 53 | No default | What it does. |
| `material` | `uint8_t` | 54 | No default | The material of ADD and PAINT. |

Byte 55 holds no field.

## ideatek:TerrainFill

`ideatek:TerrainFill` writes the cells of a chunk. The game spawns it as an event.
`ideatek_terrain_fill_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | The terrain. |
| `data` | `bridge_text_t` | 8 | The cells of the chunk. |
| `x` | `int32_t` | 16 | The place of the chunk on X. |
| `y` | `int32_t` | 20 | The place on Y. |
| `z` | `int32_t` | 24 | The place on Z. |
| `order` | `uint32_t` | 28 | The rank in the round. |

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_terrain_flags_t` | `ideatek_terrain_flags_t` | `ideatek_terrain_flags_t_desc` | `IDEATEK_TERRAIN_FLAGS_VALUES` |
| `ideatek:Terrain` | `ideatek_terrain_t` | `ideatek_terrain_t_desc` | `IDEATEK_TERRAIN_FIELDS` |
| `ideatek:TerrainMaterial` | `ideatek_terrain_material_t` | `ideatek_terrain_material_t_desc` | `IDEATEK_TERRAIN_MATERIAL_FIELDS` |
| `ideatek:TerrainChunk` | `ideatek_terrain_chunk_t` | `ideatek_terrain_chunk_t_desc` | `IDEATEK_TERRAIN_CHUNK_FIELDS` |
| `ideatek:ideatek_terrain_shape_t` | `ideatek_terrain_shape_t` | `ideatek_terrain_shape_t_desc` | `IDEATEK_TERRAIN_SHAPE_VALUES` |
| `ideatek:ideatek_terrain_op_t` | `ideatek_terrain_op_t` | `ideatek_terrain_op_t_desc` | `IDEATEK_TERRAIN_OP_VALUES` |
| `ideatek:TerrainEdit` | `ideatek_terrain_edit_t` | `ideatek_terrain_edit_t_desc` | `IDEATEK_TERRAIN_EDIT_FIELDS` |
| `ideatek:TerrainFill` | `ideatek_terrain_fill_t` | `ideatek_terrain_fill_t_desc` | `IDEATEK_TERRAIN_FILL_FIELDS` |

```text
{ IDEATEK_TERRAIN_FLAGS_COLLIDE = 0x01, IDEATEK_TERRAIN_FLAGS_NAVIGATE = 0x02 }
{ float cell_size; uint32_t chunk_cells; ideatek_terrain_flags_t flags; }
{ ideatek_entity_t terrain; ideatek_entity_t material; ideatek_entity_t physics_material; uint8_t index; }
{ ideatek_entity_t terrain; bridge_text_t bytes; int32_t x; int32_t y; int32_t z; uint32_t revision; }
{ IDEATEK_TERRAIN_SHAPE_SPHERE = 0, IDEATEK_TERRAIN_SHAPE_BOX = 1 }
{ IDEATEK_TERRAIN_OP_ADD = 0, IDEATEK_TERRAIN_OP_SUBTRACT = 1, IDEATEK_TERRAIN_OP_PAINT = 2, IDEATEK_TERRAIN_OP_SMOOTH = 3 }
{ ideatek_entity_t terrain; ideatek_position_t center; ideatek_vec3_t size; float strength; uint32_t order; ideatek_terrain_shape_t shape; ideatek_terrain_op_t op; uint8_t material; }
{ ideatek_entity_t terrain; bridge_text_t data; int32_t x; int32_t y; int32_t z; uint32_t order; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_position_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_op_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Terrain` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainMaterial` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainChunk` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainEdit` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainFill` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:Terrain`, `ideatek:TerrainMaterial`, `ideatek:TerrainEdit`, `ideatek:TerrainFill` | The types of the fields |
| The terrain module | `ideatek:TerrainChunk`, `ideatek:Mesh`, `ideatek:MeshInstance`, `ideatek:Body`, `ideatek:Shape` | `ideatek:Terrain`, `ideatek:TerrainMaterial`, `ideatek:TerrainEdit`, `ideatek:TerrainFill`, and the types of the fields |

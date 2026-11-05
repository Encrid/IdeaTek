# include/engine_subsystems/ideatek_terrain.h

`include/engine_subsystems/ideatek_terrain.h` gives the types of the terrain: voxels in channels, heightmaps, materials and blocks, edits, viewers, pieces and scatter.
`readme.md`, *The contracts*, tells the rules of each contract.
The header includes the space part: an edit has a position and a size.

## The roles

- The game writes the terrain, its materials, the edits, the fills, the viewers and the scatter.
- The terrain module alone writes the chunks. It applies the commands in order, and keeps the chunks near the viewers.
- The terrain module writes the meshes, the copies, the bodies and the shapes that the other modules read. A voxel is data: a mesh, a ray tracer and a collider are each a choice of a reader.

## The kinds

A VOXEL terrain holds chunks of cells. A HEIGHTMAP terrain holds a height for each point of a grid, as the Landscape of Unreal.
A heightmap reads its heights from `heights`, a texture of `R16_UNORM`, as the HEIGHT_FIELD shape of the physics part. `height_scale` is the height of a value of 1.
`weights` holds the weights of 4 layers of materials in RGBA8. `holes` is a mask in R8, and a texel above 0 is a hole.

The module cuts a heightmap into components of `component_quads` quads, for the levels of detail and the streaming. 0 lets the module choose.

## The channels

`channels` of a voxel terrain names the fields of each cell. A chunk holds only the channels of the mask.

| Channel | C type | Value |
| --- | --- | --- |
| SDF | `int16_t` | The signed distance to the surface. Below 0 is solid, as in Transvoxel. |
| TYPE | `uint16_t` | The id of a block, the `index` of an `ideatek:TerrainMaterial`. |
| INDICES | `uint16_t` | 4 indices of materials of 4 bits, the lowest bits first. |
| WEIGHTS | `uint16_t` | The 4 weights of those materials, 4 bits each, in the same order. |
| STATE | `uint8_t` | The state of a block, for example its turn. |
| LIGHT | `uint8_t` | The light of the sky in the low 4 bits, and the light of the blocks in the high 4 bits. |
| DATA | `uint8_t` | A value of the game. |

SDF alone makes a smooth terrain, and TYPE alone makes blocks. Both together make smooth ground with blocks on it.
The terrain module computes LIGHT from the sky and from the blocks that glow, for the game and for a mesher. A renderer with its own light ignores it.

## The chunks

A voxel terrain is cut into chunks of `chunk_cells` cells on each axis. A chunk is at `x`, `y` and `z` in chunks from the place of the terrain.
`bytes` of a chunk holds each channel of the mask in order: a `uint32_t` count of bytes, then the bytes. All numbers are little-endian.
A count of 0 makes the channel uniform, and 1 value of the channel follows. Else the cells follow, X first, then Y, then Z.

The cells of a chunk that does not exist have the type `background_type`, and they are solid only with `background_solid`.

## The blocks

A material with the shape CUBE is a block, and NONE is air. MESH draws the mesh `model` in the cell.
`rotation` reads the turn of a block from its STATE. FACEDIR names 1 of 24 turns, and FOUR_DIR 1 of 4 turns about Y.
A smooth material uses `material` and `physics_material` alone, for the INDICES that name it.

## The commands

An edit changes the cells inside its shape. ADD and SUBTRACT change the SDF, PAINT changes the materials, SMOOTH evens the SDF, and SET writes `type`.
A PATH runs from `center` to `end` with a radius of `size.x`. A MESH shape takes the signed distance of the mesh `mesh`.
A fill writes the cells of 1 chunk, in the form of the bytes of a chunk, for a generator or a level file.

The commands are events, and the module applies them in their order. Then it raises the `revision` of each chunk that changed.

## The viewers

An entity with `ideatek:TerrainViewer` keeps the chunks within `radius` of its place, times `vertical_ratio` on Y. The module drops the other chunks.
With VISUALS, the module also makes the meshes of those chunks, and with COLLISIONS, their shapes.
`peer` names the peer of the networking part whose viewer it is, or 0 for the local viewer.

## The network and the saves

A chunk is rows of the changes part: a STATE when it changes, and a FULL at a join and in a save. It is relevant to the peers whose viewer is near.
An edit goes as an EVENT, so each peer applies the same commands in the same order.

## The meshes

With MESH, the module writes for each chunk a mesh of the rendering part with an `ideatek:MeshInstance`. Without MESH, it writes no mesh, and a renderer of voxels reads the chunks.
With COLLIDE, it also writes a body and a shape of the kind TRIANGLES. With NAVIGATE, the navigation reads that shape as a source.
The seams between the levels of detail are a matter of the module.

## The pieces

A terrain on an entity with a dynamic body moves with that body, as an object of voxels.
When an edit leaves cells that no longer touch the rest, the module moves them to a new entity with a body, and spawns `ideatek:TerrainSplit`.

## The scatter

`ideatek:TerrainScatter` places copies of `mesh` on the surface of the cells of `material`, about `density` copies on each square meter.
Each copy takes a random scale from `min_scale` to `max_scale`, from `seed`. With `align`, it turns with the normal of the surface.
The module writes the copies of each chunk as `ideatek:MeshInstances` of the rendering part.

## The defaults

A chunk of 16 cells is the block of godot_voxel, the chunk of the Voxel Plugin and the block of Luanti. No reference gives a size of a cell that fits each game, so the game sets it.

## ideatek:ideatek_terrain_kind_t

`ideatek_terrain_kind_t` tells the kind of a terrain: *The kinds*.
`ideatek_terrain_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TERRAIN_KIND_VOXEL` | Chunks of cells. |
| 1 | `IDEATEK_TERRAIN_KIND_HEIGHTMAP` | A grid of heights. |

## ideatek:ideatek_terrain_channels_t

`ideatek_terrain_channels_t` names the channels of the cells: *The channels*.
`ideatek_terrain_channels_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_TERRAIN_CHANNELS_SDF` | The signed distance. |
| 0x02 | `IDEATEK_TERRAIN_CHANNELS_TYPE` | The id of a block. |
| 0x04 | `IDEATEK_TERRAIN_CHANNELS_INDICES` | The indices of 4 materials. |
| 0x08 | `IDEATEK_TERRAIN_CHANNELS_WEIGHTS` | The weights of those materials. |
| 0x10 | `IDEATEK_TERRAIN_CHANNELS_STATE` | The state of a block. |
| 0x20 | `IDEATEK_TERRAIN_CHANNELS_LIGHT` | The light of the cell. |
| 0x40 | `IDEATEK_TERRAIN_CHANNELS_DATA` | A value of the game. |

## ideatek:ideatek_terrain_flags_t

`ideatek_terrain_flags_t` names the options of a terrain.
`ideatek_terrain_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_TERRAIN_FLAGS_COLLIDE` | The terrain has shapes for the physics. |
| 0x02 | `IDEATEK_TERRAIN_FLAGS_NAVIGATE` | The navigation reads the terrain. |
| 0x04 | `IDEATEK_TERRAIN_FLAGS_MESH` | The module writes meshes for the rendering. |

## ideatek:Terrain

`ideatek:Terrain` makes a terrain at the place of its entity.
`ideatek_terrain_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `heights` | `ideatek_entity_t` | 0 | 0 | The texture of the heights of a HEIGHTMAP. |
| `weights` | `ideatek_entity_t` | 8 | 0 | The texture of the weights of the layers of a HEIGHTMAP. |
| `holes` | `ideatek_entity_t` | 16 | 0 | The mask of the holes of a HEIGHTMAP. 0 means none. |
| `cell_size` | `float` | 24 | No default | The width of a cell, or of a quad of a heightmap. |
| `height_scale` | `float` | 28 | No default | The height of a value of 1 of a HEIGHTMAP. |
| `chunk_cells` | `uint32_t` | 32 | 16 | The cells of a chunk on each axis. |
| `component_quads` | `uint32_t` | 36 | 0 | The quads of a component of a HEIGHTMAP. 0 lets the module choose. |
| `background_type` | `uint16_t` | 40 | 0 | The type of the cells of a chunk that does not exist. |
| `kind` | `ideatek_terrain_kind_t` | 42 | `IDEATEK_TERRAIN_KIND_VOXEL` | The kind. |
| `channels` | `ideatek_terrain_channels_t` | 43 | SDF, INDICES and WEIGHTS | The channels of the cells. |
| `flags` | `ideatek_terrain_flags_t` | 44 | COLLIDE, NAVIGATE and MESH | The options. |
| `background_solid` | `bool` | 45 | `false` | `true` if the cells of a chunk that does not exist are solid. |

Bytes 46 to 47 hold no field.

## ideatek:ideatek_block_shape_t

`ideatek_block_shape_t` tells the shape of a block: *The blocks*.
`ideatek_block_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_BLOCK_SHAPE_CUBE` | A cube that fills the cell. |
| 1 | `IDEATEK_BLOCK_SHAPE_MESH` | The mesh `model` in the cell. |
| 2 | `IDEATEK_BLOCK_SHAPE_NONE` | Air. |

## ideatek:ideatek_block_rotation_t

`ideatek_block_rotation_t` tells how a block reads its turn from STATE.
`ideatek_block_rotation_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_BLOCK_ROTATION_NONE` | The block does not turn. |
| 1 | `IDEATEK_BLOCK_ROTATION_FACEDIR` | STATE names 1 of 24 turns. |
| 2 | `IDEATEK_BLOCK_ROTATION_FOUR_DIR` | STATE names 1 of 4 turns about Y. |

## ideatek:TerrainMaterial

`ideatek:TerrainMaterial` maps 1 index of the cells to its materials and its block.
`ideatek_terrain_material_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | No default | The terrain. |
| `material` | `ideatek_entity_t` | 8 | No default | The material of the rendering. |
| `physics_material` | `ideatek_entity_t` | 16 | No default | The material of the physics. |
| `model` | `ideatek_entity_t` | 24 | 0 | The mesh of a block of the shape MESH. 0 means none. |
| `index` | `uint16_t` | 32 | No default | The index: a TYPE for a block, or an index of INDICES. |
| `shape` | `ideatek_block_shape_t` | 34 | `IDEATEK_BLOCK_SHAPE_CUBE` | The shape of a block. |
| `rotation` | `ideatek_block_rotation_t` | 35 | `IDEATEK_BLOCK_ROTATION_NONE` | How the block turns. |

Bytes 36 to 39 hold no field.

## ideatek:TerrainChunk

`ideatek:TerrainChunk` is 1 chunk of a terrain. Only the terrain module writes it.
`ideatek_terrain_chunk_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | The terrain. |
| `bytes` | `bridge_text_t` | 8 | The channels: *The chunks*. |
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
| 2 | `IDEATEK_TERRAIN_SHAPE_POINT` | The 1 cell at `center`. |
| 3 | `IDEATEK_TERRAIN_SHAPE_PATH` | A path from `center` to `end`, of radius `size.x`. |
| 4 | `IDEATEK_TERRAIN_SHAPE_MESH` | The volume of the mesh `mesh`. |

## ideatek:ideatek_terrain_op_t

`ideatek_terrain_op_t` tells what an edit does.
`ideatek_terrain_op_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TERRAIN_OP_ADD` | Add matter. |
| 1 | `IDEATEK_TERRAIN_OP_SUBTRACT` | Remove matter. |
| 2 | `IDEATEK_TERRAIN_OP_PAINT` | Change the material. |
| 3 | `IDEATEK_TERRAIN_OP_SMOOTH` | Smooth the surface. |
| 4 | `IDEATEK_TERRAIN_OP_SET` | Write the block `type`. |

## ideatek:TerrainEdit

`ideatek:TerrainEdit` changes the cells in a shape. The game spawns it as an event.
`ideatek_terrain_edit_t` takes 96 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | No default | The terrain. |
| `mesh` | `ideatek_entity_t` | 8 | 0 | The mesh of the shape MESH. 0 means none. |
| `center` | `ideatek_position_t` | 16 | No default | The center, in the world. |
| `end` | `ideatek_position_t` | 40 | No default | The end of a PATH. |
| `size` | `ideatek_vec3_t` | 64 | No default | The size of the shape. |
| `strength` | `float` | 76 | 1 | The strength, from 0 to 1. |
| `order` | `uint32_t` | 80 | No default | The rank in the round. |
| `material` | `uint16_t` | 84 | No default | The material of ADD and PAINT. |
| `type` | `uint16_t` | 86 | No default | The block of SET. |
| `shape` | `ideatek_terrain_shape_t` | 88 | No default | The shape. |
| `op` | `ideatek_terrain_op_t` | 89 | No default | What it does. |

Bytes 90 to 95 hold no field.

## ideatek:TerrainFill

`ideatek:TerrainFill` writes the cells of a chunk. The game spawns it as an event.
`ideatek_terrain_fill_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | The terrain. |
| `data` | `bridge_text_t` | 8 | The channels of the chunk, as the bytes of a chunk. |
| `x` | `int32_t` | 16 | The place of the chunk on X. |
| `y` | `int32_t` | 20 | The place on Y. |
| `z` | `int32_t` | 24 | The place on Z. |
| `order` | `uint32_t` | 28 | The rank in the round. |

## ideatek:ideatek_viewer_flags_t

`ideatek_viewer_flags_t` names what a viewer needs.
`ideatek_viewer_flags_t` is a bitmask of 1 byte.

| Bit | Name | Meaning |
| --- | --- | --- |
| 0x01 | `IDEATEK_VIEWER_FLAGS_VISUALS` | The chunks near the viewer get meshes. |
| 0x02 | `IDEATEK_VIEWER_FLAGS_COLLISIONS` | The chunks near the viewer get shapes. |

## ideatek:TerrainViewer

`ideatek:TerrainViewer` keeps the chunks around its entity: *The viewers*.
`ideatek_terrain_viewer_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `radius` | `float` | 0 | No default | The distance of the chunks that the viewer keeps. |
| `vertical_ratio` | `float` | 4 | 1 | The factor of the radius on Y. |
| `peer` | `uint32_t` | 8 | 0 | The peer of the networking part. 0 means the local viewer. |
| `flags` | `ideatek_viewer_flags_t` | 12 | VISUALS and COLLISIONS | What the viewer needs. |

Bytes 13 to 15 hold no field.

## ideatek:TerrainSplit

`ideatek:TerrainSplit` tells that cells broke off a terrain: *The pieces*. It is an event of 1 round.
`ideatek_terrain_split_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `source` | `ideatek_entity_t` | 0 | The terrain that lost the cells. |
| `piece` | `ideatek_entity_t` | 8 | The new entity of the cells. |
| `order` | `uint32_t` | 16 | The rank in the round. |

Bytes 20 to 23 hold no field.

## ideatek:TerrainScatter

`ideatek:TerrainScatter` scatters copies of a mesh on a terrain: *The scatter*.
`ideatek_terrain_scatter_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `terrain` | `ideatek_entity_t` | 0 | No default | The terrain. |
| `mesh` | `ideatek_entity_t` | 8 | No default | The mesh of the copies. |
| `density` | `float` | 16 | No default | The copies on each square meter. |
| `min_scale` | `float` | 20 | 1 | The smallest scale. |
| `max_scale` | `float` | 24 | 1 | The largest scale. |
| `seed` | `uint32_t` | 28 | 0 | The seed of the places. |
| `material` | `uint16_t` | 32 | No default | The material whose surface takes the copies. |
| `align` | `bool` | 34 | `false` | `true` if a copy turns with the normal. |

Bytes 35 to 39 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_terrain_kind_t` | `ideatek_terrain_kind_t` | `ideatek_terrain_kind_t_desc` | `IDEATEK_TERRAIN_KIND_VALUES` |
| `ideatek:ideatek_terrain_channels_t` | `ideatek_terrain_channels_t` | `ideatek_terrain_channels_t_desc` | `IDEATEK_TERRAIN_CHANNELS_VALUES` |
| `ideatek:ideatek_terrain_flags_t` | `ideatek_terrain_flags_t` | `ideatek_terrain_flags_t_desc` | `IDEATEK_TERRAIN_FLAGS_VALUES` |
| `ideatek:Terrain` | `ideatek_terrain_t` | `ideatek_terrain_t_desc` | `IDEATEK_TERRAIN_FIELDS` |
| `ideatek:ideatek_block_shape_t` | `ideatek_block_shape_t` | `ideatek_block_shape_t_desc` | `IDEATEK_BLOCK_SHAPE_VALUES` |
| `ideatek:ideatek_block_rotation_t` | `ideatek_block_rotation_t` | `ideatek_block_rotation_t_desc` | `IDEATEK_BLOCK_ROTATION_VALUES` |
| `ideatek:TerrainMaterial` | `ideatek_terrain_material_t` | `ideatek_terrain_material_t_desc` | `IDEATEK_TERRAIN_MATERIAL_FIELDS` |
| `ideatek:TerrainChunk` | `ideatek_terrain_chunk_t` | `ideatek_terrain_chunk_t_desc` | `IDEATEK_TERRAIN_CHUNK_FIELDS` |
| `ideatek:ideatek_terrain_shape_t` | `ideatek_terrain_shape_t` | `ideatek_terrain_shape_t_desc` | `IDEATEK_TERRAIN_SHAPE_VALUES` |
| `ideatek:ideatek_terrain_op_t` | `ideatek_terrain_op_t` | `ideatek_terrain_op_t_desc` | `IDEATEK_TERRAIN_OP_VALUES` |
| `ideatek:TerrainEdit` | `ideatek_terrain_edit_t` | `ideatek_terrain_edit_t_desc` | `IDEATEK_TERRAIN_EDIT_FIELDS` |
| `ideatek:TerrainFill` | `ideatek_terrain_fill_t` | `ideatek_terrain_fill_t_desc` | `IDEATEK_TERRAIN_FILL_FIELDS` |
| `ideatek:ideatek_viewer_flags_t` | `ideatek_viewer_flags_t` | `ideatek_viewer_flags_t_desc` | `IDEATEK_VIEWER_FLAGS_VALUES` |
| `ideatek:TerrainViewer` | `ideatek_terrain_viewer_t` | `ideatek_terrain_viewer_t_desc` | `IDEATEK_TERRAIN_VIEWER_FIELDS` |
| `ideatek:TerrainSplit` | `ideatek_terrain_split_t` | `ideatek_terrain_split_t_desc` | `IDEATEK_TERRAIN_SPLIT_FIELDS` |
| `ideatek:TerrainScatter` | `ideatek_terrain_scatter_t` | `ideatek_terrain_scatter_t_desc` | `IDEATEK_TERRAIN_SCATTER_FIELDS` |

```text
{ IDEATEK_TERRAIN_KIND_VOXEL = 0, IDEATEK_TERRAIN_KIND_HEIGHTMAP = 1 }
{ IDEATEK_TERRAIN_CHANNELS_SDF = 0x01, IDEATEK_TERRAIN_CHANNELS_TYPE = 0x02, IDEATEK_TERRAIN_CHANNELS_INDICES = 0x04, IDEATEK_TERRAIN_CHANNELS_WEIGHTS = 0x08, IDEATEK_TERRAIN_CHANNELS_STATE = 0x10, IDEATEK_TERRAIN_CHANNELS_LIGHT = 0x20, IDEATEK_TERRAIN_CHANNELS_DATA = 0x40 }
{ IDEATEK_TERRAIN_FLAGS_COLLIDE = 0x01, IDEATEK_TERRAIN_FLAGS_NAVIGATE = 0x02, IDEATEK_TERRAIN_FLAGS_MESH = 0x04 }
{ ideatek_entity_t heights; ideatek_entity_t weights; ideatek_entity_t holes; float cell_size; float height_scale; uint32_t chunk_cells; uint32_t component_quads; uint16_t background_type; ideatek_terrain_kind_t kind; ideatek_terrain_channels_t channels; ideatek_terrain_flags_t flags; bool background_solid; }
{ IDEATEK_BLOCK_SHAPE_CUBE = 0, IDEATEK_BLOCK_SHAPE_MESH = 1, IDEATEK_BLOCK_SHAPE_NONE = 2 }
{ IDEATEK_BLOCK_ROTATION_NONE = 0, IDEATEK_BLOCK_ROTATION_FACEDIR = 1, IDEATEK_BLOCK_ROTATION_FOUR_DIR = 2 }
{ ideatek_entity_t terrain; ideatek_entity_t material; ideatek_entity_t physics_material; ideatek_entity_t model; uint16_t index; ideatek_block_shape_t shape; ideatek_block_rotation_t rotation; }
{ ideatek_entity_t terrain; bridge_text_t bytes; int32_t x; int32_t y; int32_t z; uint32_t revision; }
{ IDEATEK_TERRAIN_SHAPE_SPHERE = 0, IDEATEK_TERRAIN_SHAPE_BOX = 1, IDEATEK_TERRAIN_SHAPE_POINT = 2, IDEATEK_TERRAIN_SHAPE_PATH = 3, IDEATEK_TERRAIN_SHAPE_MESH = 4 }
{ IDEATEK_TERRAIN_OP_ADD = 0, IDEATEK_TERRAIN_OP_SUBTRACT = 1, IDEATEK_TERRAIN_OP_PAINT = 2, IDEATEK_TERRAIN_OP_SMOOTH = 3, IDEATEK_TERRAIN_OP_SET = 4 }
{ ideatek_entity_t terrain; ideatek_entity_t mesh; ideatek_position_t center; ideatek_position_t end; ideatek_vec3_t size; float strength; uint32_t order; uint16_t material; uint16_t type; ideatek_terrain_shape_t shape; ideatek_terrain_op_t op; }
{ ideatek_entity_t terrain; bridge_text_t data; int32_t x; int32_t y; int32_t z; uint32_t order; }
{ IDEATEK_VIEWER_FLAGS_VISUALS = 0x01, IDEATEK_VIEWER_FLAGS_COLLISIONS = 0x02 }
{ float radius; float vertical_ratio; uint32_t peer; ideatek_viewer_flags_t flags; }
{ ideatek_entity_t source; ideatek_entity_t piece; uint32_t order; }
{ ideatek_entity_t terrain; ideatek_entity_t mesh; float density; float min_scale; float max_scale; uint32_t seed; uint16_t material; bool align; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_position_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_channels_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_block_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_block_rotation_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_terrain_op_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_viewer_flags_t` | `BRIDGE_BITMASK` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Terrain` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainMaterial` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainChunk` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainEdit` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainFill` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainViewer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainSplit` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:TerrainScatter` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| The game | `ideatek:Terrain`, `ideatek:TerrainMaterial`, `ideatek:TerrainEdit`, `ideatek:TerrainFill`, `ideatek:TerrainViewer`, `ideatek:TerrainScatter` | The types of the fields |
| The terrain module | `ideatek:TerrainChunk`, `ideatek:TerrainSplit`, `ideatek:Mesh`, `ideatek:MeshInstance`, `ideatek:MeshInstances`, `ideatek:Body`, `ideatek:Shape` | `ideatek:Terrain`, `ideatek:TerrainMaterial`, `ideatek:TerrainEdit`, `ideatek:TerrainFill`, `ideatek:TerrainViewer`, `ideatek:TerrainScatter`, and the types of the fields |

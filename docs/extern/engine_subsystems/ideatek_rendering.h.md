# include/engine_subsystems/ideatek_rendering.h

`include/engine_subsystems/ideatek_rendering.h` gives the types of the rendering: cameras, meshes, materials, textures, lights, decals, fog, the sky and the effects of a camera.
`readme.md`, *The contracts*, tells the rules of each contract.
Its fields use `ideatek_entity_t` of the world part and `ideatek_vec3_t` of the space part, so the header includes both.

## The roles

- The game, a loader or a tool writes each type of the part. It is the author.
- The render module reads each type, and `ideatek:WorldTransform` of the space part. It draws the cameras.
- The render module chooses its techniques: the global illumination, the shadows, the order of the passes. No type of the part names a technique.

## The units

- A length is in meters, an angle is in radians, and a time is in seconds.
- A color is linear, not sRGB. A texture can hold sRGB data: its format tells.
- A light gives its intensity in candela, or in lux for a directional light.
- A camera looks along -Z of its `ideatek:WorldTransform`. A light and a decal point along -Z too.

## The resources

A mesh, a material and a texture are resource entities: the resources part of layer 3 loads them, or the game spawns them.
Their writer owns their bytes. To change a resource, the writer writes new bytes and raises `revision`.
The render module then uploads the resource again. A drawn entity names its resources by entity.

## The bytes of a mesh

`bytes` of `ideatek:Mesh` holds, in this order and with no bytes between them:

1. `stream_count` records of `ideatek_mesh_stream_t`.
2. `submesh_count` records of `ideatek_submesh_t`.
3. For each sub-mesh, `lod_count` minus 1 records of `ideatek_mesh_lod_t`: the coarser levels.
4. The data of the vertices, at the offset of each stream.
5. The indices, at `index_offset`, in the format of `index_format`.

All numbers are little-endian. A reader copies each record with `memcpy`.
Level 0 of a sub-mesh is the sub-mesh itself. The other levels use the same vertices and other indices of the index data.

## The bytes of a texture

`bytes` of `ideatek:Texture` holds each mip level from `top_mip`, the largest that it holds.
Each level holds each layer, or each face of a cube in the order +X, -X, +Y, -Y, +Z, -Z.
The data of a level is packed in rows of texels or of blocks, as the format of Vulkan gives.

## The selection of a level

The render module chooses for each sub-mesh the coarsest level whose screen error is below its own threshold.
The threshold is a setting of the render module. `lod_bias` of `ideatek:MeshInstance` multiplies the error.
`ideatek:VisibilityRange` fades an entity in and out by distance, for a far proxy of a group of meshes.

## The texture slots

A material names each texture with `ideatek_texture_slot_t`: the texture, the set of texture coordinates and a transform.
The transform first scales the coordinates, then turns them by `rotation`, then moves them by `offset`, as `KHR_texture_transform` of glTF tells.
A slot whose `texture` is 0 has no texture.

## The extensions of a material

Each extension of a material is a type of its own on the entity of the material, as each `KHR_materials_*` extension of glTF.
A material with the type gets the effect, and a material without it does not.
A renderer that does not know a type draws the material without it.

## The baked light

A tool can bake light into probes and lightmaps. The render module uses them for the light that it does not compute.
A renderer of real-time global illumination can ignore them, or use them where its own light does not reach.
Each probe and each volume is an entity at a place. Where 2 of them overlap, the one with the higher `priority` wins.

## The streaming of a texture

`bytes` of `ideatek:Texture` holds the levels from `top_mip` to the smallest level. `width` and `height` stay the size of level 0.
A loader that streams a texture first gives the small levels. It lowers `top_mip` and raises `revision` when a larger level comes.
The render module draws with the levels that it has.

## The global illumination

`gi_mode` of `ideatek:Visibility` tells how a mesh takes part in the global illumination.
A renderer can keep the meshes of `IDEATEK_GI_MODE_STATIC` in a cache, because they do not move.
`IDEATEK_GI_MODE_DISABLED` fits a mesh that must not light the world, for example the weapon of a first-person view.

## The effects of a camera

Each effect of a camera is a type of its own on the entity of the camera.
A camera with the type gets the effect, and a camera without it does not.
`ideatek:AutoExposure` replaces `ev100` of `ideatek:Exposure`.

## The sky and the fog

The sky with the highest `priority` is the active sky. The render module lights the scene from it.
All fogs add: their densities sum where they overlap.
A sky of the kind `IDEATEK_SKY_KIND_ATMOSPHERE` takes the sun from each directional light.

## ideatek:ideatek_color_t

`ideatek_color_t` is a linear color with alpha.
`ideatek_color_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `r` | `float` | 0 | The red part. |
| `g` | `float` | 4 | The green part. |
| `b` | `float` | 8 | The blue part. |
| `a` | `float` | 12 | The alpha part. 0 is transparent. |

## ideatek:ideatek_rgb_t

`ideatek_rgb_t` is a linear color with no alpha.
`ideatek_rgb_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `r` | `float` | 0 | The red part. |
| `g` | `float` | 4 | The green part. |
| `b` | `float` | 8 | The blue part. |

## ideatek:ideatek_rect_t

`ideatek_rect_t` is a part of an image, as fractions of its size.
`ideatek_rect_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `x` | `float` | 0 | The left edge, from 0 to 1. |
| `y` | `float` | 4 | The top edge, from 0 to 1. |
| `width` | `float` | 8 | The width, from 0 to 1. |
| `height` | `float` | 12 | The height, from 0 to 1. |

## ideatek:ideatek_projection_kind_t

`ideatek_projection_kind_t` tells the shape of a projection.
`ideatek_projection_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_PROJECTION_KIND_PERSPECTIVE` | A frustum with a vertical angle. |
| 1 | `IDEATEK_PROJECTION_KIND_ORTHOGRAPHIC` | A box with a height. |
| 2 | `IDEATEK_PROJECTION_KIND_ASYMMETRIC` | A frustum with 4 angles: `ideatek:ProjectionAngles`. |

## ideatek:ideatek_topology_t

`ideatek_topology_t` tells how the indices make primitives.
`ideatek_topology_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TOPOLOGY_POINT_LIST` | Each index is a point. |
| 1 | `IDEATEK_TOPOLOGY_LINE_LIST` | Each 2 indices are a line. |
| 2 | `IDEATEK_TOPOLOGY_LINE_STRIP` | Each index ends a line from the index before. |
| 3 | `IDEATEK_TOPOLOGY_TRIANGLE_LIST` | Each 3 indices are a triangle. |
| 4 | `IDEATEK_TOPOLOGY_TRIANGLE_STRIP` | Each index ends a triangle with the 2 indices before. |

## ideatek:ideatek_index_format_t

`ideatek_index_format_t` tells the size of an index.
`ideatek_index_format_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_INDEX_FORMAT_NONE` | The mesh has no indices. The vertices are in order. |
| 1 | `IDEATEK_INDEX_FORMAT_U16` | Each index is a `uint16_t`. |
| 2 | `IDEATEK_INDEX_FORMAT_U32` | Each index is a `uint32_t`. |

## ideatek:ideatek_semantic_t

`ideatek_semantic_t` tells what a stream of a mesh holds.
`ideatek_semantic_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SEMANTIC_POSITION` | The position of a vertex. |
| 1 | `IDEATEK_SEMANTIC_NORMAL` | The normal. |
| 2 | `IDEATEK_SEMANTIC_TANGENT` | The tangent, with the sign of the bitangent in W. |
| 3 | `IDEATEK_SEMANTIC_TEXCOORD` | A texture coordinate. `set` tells which. |
| 4 | `IDEATEK_SEMANTIC_COLOR` | A color of a vertex. `set` tells which. |
| 5 | `IDEATEK_SEMANTIC_JOINTS` | The indices of 4 joints. `set` tells the group of 4. |
| 6 | `IDEATEK_SEMANTIC_WEIGHTS` | The weights of the 4 joints of the same `set`. |
| 7 | `IDEATEK_SEMANTIC_MORPH_POSITION` | The change of the position in the morph target `target`. |
| 8 | `IDEATEK_SEMANTIC_MORPH_NORMAL` | The change of the normal in the morph target `target`. |
| 9 | `IDEATEK_SEMANTIC_MORPH_TANGENT` | The change of the tangent in the morph target `target`. |
| 10 | `IDEATEK_SEMANTIC_SOFT_MAX_DISTANCE` | The most that a soft body vertex moves from its skinned place. 0 pins it. |
| 11 | `IDEATEK_SEMANTIC_SOFT_BACKSTOP_DISTANCE` | The distance of the backstop sphere behind a soft body vertex. |
| 12 | `IDEATEK_SEMANTIC_SOFT_BACKSTOP_RADIUS` | The radius of that backstop sphere. |

A soft body vertex with no `IDEATEK_SEMANTIC_SOFT_MAX_DISTANCE` is free. With no backstop stream, it has no backstop.
The soft body streams hold 1 float for each vertex, as in Jolt.

## ideatek:ideatek_mesh_stream_t

`ideatek_mesh_stream_t` is 1 stream of the vertices of a mesh.
`ideatek_mesh_stream_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `format` | `uint32_t` | 0 | The format of each value: a `VkFormat` of Vulkan. |
| `offset` | `uint32_t` | 4 | The byte of the first value in `bytes`. |
| `stride` | `uint32_t` | 8 | The bytes from 1 value to the next. |
| `target` | `uint16_t` | 12 | The morph target, for a morph stream. |
| `semantic` | `ideatek_semantic_t` | 14 | What the stream holds. |
| `set` | `uint8_t` | 15 | The rank among the streams of 1 semantic. |

## ideatek:ideatek_submesh_t

`ideatek_submesh_t` is a range of the indices with 1 material.
`ideatek_submesh_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `first` | `uint32_t` | 0 | The first index of the sub-mesh. |
| `count` | `uint32_t` | 4 | The number of its indices. |

## ideatek:ideatek_mesh_lod_t

`ideatek_mesh_lod_t` is a coarser level of a sub-mesh.
`ideatek_mesh_lod_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `error` | `float` | 0 | The error of the level, in meters. |
| `first` | `uint32_t` | 4 | The first index of the level. |
| `count` | `uint32_t` | 8 | The number of its indices. |

## ideatek:ideatek_texture_slot_t

`ideatek_texture_slot_t` is a texture of a material: *The texture slots*.
`ideatek_texture_slot_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_entity_t` | 0 | 0 | The texture entity. 0 means none. |
| `offset` | `float[2]` | 8 | 0, 0 | The move of the coordinates, on U and V. |
| `scale` | `float[2]` | 16 | 1, 1 | The scale of the coordinates, on U and V. |
| `rotation` | `float` | 24 | 0 | The turn of the coordinates, in radians, counter-clockwise. |
| `texcoord` | `uint8_t` | 28 | 0 | The set of texture coordinates: the `set` of `IDEATEK_SEMANTIC_TEXCOORD`. |

Bytes 29 to 31 hold no field.

## ideatek:ideatek_gi_mode_t

`ideatek_gi_mode_t` tells the part of a mesh in the global illumination: *The global illumination*.
`ideatek_gi_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_GI_MODE_DYNAMIC` | The mesh takes part, and it can move. |
| 1 | `IDEATEK_GI_MODE_STATIC` | The mesh takes part, and it does not move. |
| 2 | `IDEATEK_GI_MODE_DISABLED` | The mesh does not take part. |

## ideatek:ideatek_alpha_mode_t

`ideatek_alpha_mode_t` tells the use of the alpha of a material, as in glTF 2.0.
`ideatek_alpha_mode_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_ALPHA_MODE_OPAQUE` | The alpha is not used. |
| 1 | `IDEATEK_ALPHA_MODE_MASK` | A texel with an alpha below `alpha_cutoff` is not drawn. |
| 2 | `IDEATEK_ALPHA_MODE_BLEND` | The color blends with the color behind. |

## ideatek:ideatek_dimension_t

`ideatek_dimension_t` tells the shape of a texture.
`ideatek_dimension_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_DIMENSION_2D` | A 2D image. |
| 1 | `IDEATEK_DIMENSION_2D_ARRAY` | Layers of 2D images. |
| 2 | `IDEATEK_DIMENSION_CUBE` | The 6 faces of a cube. |
| 3 | `IDEATEK_DIMENSION_3D` | A volume. |

## ideatek:ideatek_filter_t

`ideatek_filter_t` tells how a sampler reads between texels.
`ideatek_filter_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FILTER_NEAREST` | The nearest texel. |
| 1 | `IDEATEK_FILTER_LINEAR` | A blend of the near texels. |

## ideatek:ideatek_wrap_t

`ideatek_wrap_t` tells what a sampler reads outside 0 to 1, as in glTF 2.0.
`ideatek_wrap_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_WRAP_REPEAT` | The texture repeats. |
| 1 | `IDEATEK_WRAP_MIRRORED_REPEAT` | The texture repeats, and each second copy is a mirror. |
| 2 | `IDEATEK_WRAP_CLAMP_TO_EDGE` | The edge texel stays. |

## ideatek:ideatek_light_kind_t

`ideatek_light_kind_t` tells the kind of a light, as in `KHR_lights_punctual` of glTF.
`ideatek_light_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_LIGHT_KIND_DIRECTIONAL` | Parallel light, in lux. |
| 1 | `IDEATEK_LIGHT_KIND_POINT` | Light from a point to all sides, in candela. |
| 2 | `IDEATEK_LIGHT_KIND_SPOT` | Light from a point in a cone, in candela. |

## ideatek:ideatek_cast_shadow_t

`ideatek_cast_shadow_t` tells the shadow of a mesh.
`ideatek_cast_shadow_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_CAST_SHADOW_OFF` | The mesh casts no shadow. |
| 1 | `IDEATEK_CAST_SHADOW_ON` | The mesh casts a shadow and is drawn. |
| 2 | `IDEATEK_CAST_SHADOW_ONLY` | The mesh casts a shadow and is not drawn. |

## ideatek:ideatek_fog_shape_t

`ideatek_fog_shape_t` tells the place of a fog.
`ideatek_fog_shape_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_FOG_SHAPE_INFINITE` | The fog fills the world. |
| 1 | `IDEATEK_FOG_SHAPE_BOX` | A box of `size` half extents. |
| 2 | `IDEATEK_FOG_SHAPE_ELLIPSOID` | An ellipsoid of `size` radii. |

## ideatek:ideatek_sky_kind_t

`ideatek_sky_kind_t` tells the source of a sky.
`ideatek_sky_kind_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_SKY_KIND_ATMOSPHERE` | The sky comes from `ideatek:Atmosphere` and the lights. |
| 1 | `IDEATEK_SKY_KIND_TEXTURE` | The sky comes from a texture. |

## ideatek:ideatek_tonemap_curve_t

`ideatek_tonemap_curve_t` tells the curve that maps light to the screen.
`ideatek_tonemap_curve_t` is an enum of 1 byte.

| Value | Name | Meaning |
| --- | --- | --- |
| 0 | `IDEATEK_TONEMAP_CURVE_AGX` | AgX, the default view of Blender since its version 4.0. |
| 1 | `IDEATEK_TONEMAP_CURVE_NEUTRAL` | The neutral curve of Khronos for PBR. |
| 2 | `IDEATEK_TONEMAP_CURVE_ACES` | The filmic curve of ACES. |
| 3 | `IDEATEK_TONEMAP_CURVE_LINEAR` | No curve: the values are clamped. |

## ideatek:Camera

`ideatek:Camera` draws the world into a target.
`ideatek_camera_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `target` | `ideatek_entity_t` | 0 | No default | A window, or a texture entity. |
| `groups` | `uint64_t` | 8 | All bits | The render groups that the camera draws. |
| `viewport` | `ideatek_rect_t` | 16 | No default | The part of the target. A width or a height of 0 means the whole target. |
| `order` | `int32_t` | 32 | 0 | Cameras on 1 target draw by rising order. |
| `active` | `bool` | 36 | `true` | `true` if the camera draws. |

Bytes 37 to 39 hold no field.

The place of the camera is its `ideatek:WorldTransform`. The motion between 2 steps comes from `ideatek:PreviousWorldTransform`.
A weapon in front of a first-person view is a second camera with a higher `order`, its own projection, and its own groups.

## ideatek:Projection

`ideatek:Projection` tells the shape of the view of a camera.
`ideatek_projection_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `fov` | `float` | 0 | The vertical angle of a perspective. |
| `height` | `float` | 4 | The height of an orthographic box. |
| `near` | `float` | 8 | The distance of the near plane. |
| `far` | `float` | 12 | The distance of the far plane. |
| `kind` | `ideatek_projection_kind_t` | 16 | The shape. |

Bytes 17 to 19 hold no field.

The references do not agree on the angle and the planes, so the game sets each value.

## ideatek:ProjectionAngles

`ideatek:ProjectionAngles` gives the 4 angles of a projection of the kind `IDEATEK_PROJECTION_KIND_ASYMMETRIC`.
`ideatek_projection_angles_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `left` | `float` | 0 | The angle of the left plane. It is negative on the left. |
| `right` | `float` | 4 | The angle of the right plane. |
| `up` | `float` | 8 | The angle of the top plane. |
| `down` | `float` | 12 | The angle of the bottom plane. It is negative below. |

An XR module writes it for the view of each eye. A portal or a screen of many displays uses it too.

## ideatek:Mesh

`ideatek:Mesh` is the geometry of a resource entity.
`ideatek_mesh_t` takes 44 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The tables and the data: *The bytes of a mesh*. |
| `vertex_count` | `uint32_t` | 8 | The number of vertices. |
| `index_count` | `uint32_t` | 12 | The number of indices. |
| `index_offset` | `uint32_t` | 16 | The byte of the first index in `bytes`. |
| `stream_count` | `uint32_t` | 20 | The number of streams. |
| `submesh_count` | `uint32_t` | 24 | The number of sub-meshes. |
| `lod_count` | `uint32_t` | 28 | The number of levels of each sub-mesh, with level 0. |
| `target_count` | `uint32_t` | 32 | The number of morph targets. |
| `revision` | `uint32_t` | 36 | Raised at each change. |
| `topology` | `ideatek_topology_t` | 40 | How the indices make primitives. |
| `index_format` | `ideatek_index_format_t` | 41 | The size of an index. |

Bytes 42 to 43 hold no field.

A mesh can change at each step, for example a cut or a terrain chunk. The writer raises `revision` each time.
A renderer that cannot read a stream, a format or the number of joint sets logs the entity once.

## ideatek:MeshInstance

`ideatek:MeshInstance` draws a mesh at the place of its entity.
`ideatek_mesh_instance_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `mesh` | `ideatek_entity_t` | 0 | No default | The resource entity of the mesh. |
| `lod_bias` | `float` | 8 | 1 | The factor of the error of each level. |

Bytes 12 to 15 hold no field.

## ideatek:MeshMaterials

`ideatek:MeshMaterials` gives the material of each sub-mesh of a drawn entity.
`ideatek_mesh_materials_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `materials` | `bridge_text_t` | 0 | The `ideatek_entity_t` of a material for each sub-mesh, in the order of the sub-meshes. |

A renderer draws the first type on a material entity that it knows. If it knows none, it draws `ideatek:Material`.

## ideatek:Visibility

`ideatek:Visibility` places a drawn mesh in render groups.
`ideatek_visibility_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | 1 | The render groups of the mesh. |
| `cast_shadow` | `ideatek_cast_shadow_t` | 8 | `IDEATEK_CAST_SHADOW_ON` | The shadow of the mesh. |
| `gi_mode` | `ideatek_gi_mode_t` | 9 | `IDEATEK_GI_MODE_DYNAMIC` | The part of the mesh in the global illumination. |
| `visible` | `bool` | 10 | `true` | `false` hides the mesh and its shadow, as `KHR_node_visibility` of glTF. |

Bytes 11 to 15 hold no field.

A camera draws the mesh when `groups` of the mesh and `groups` of the camera share a bit.
The render module draws only an entity with `ideatek:Visibility`. It logs each `ideatek:MeshInstance` without it once.

## ideatek:Affects

`ideatek:Affects` limits a light or a decal to render groups.
`ideatek_affects_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `groups` | `uint64_t` | 0 | All bits | The render groups that the light or the decal reaches. |

The render module logs each light or decal without it once.

## ideatek:VisibilityRange

`ideatek:VisibilityRange` shows an entity only between 2 distances from the camera.
`ideatek_visibility_range_t` takes 16 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `begin` | `float` | 0 | The distance where the entity starts to show. |
| `end` | `float` | 4 | The distance where it stops. 0 means no end. |
| `begin_margin` | `float` | 8 | The length of the fade at `begin`. |
| `end_margin` | `float` | 12 | The length of the fade at `end`. |

The fade is a dither, so the 2 entities of a change never blend their colors.

## ideatek:Material

`ideatek:Material` is the metallic-roughness material of glTF on a resource entity.
`ideatek_material_t` takes 216 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `base_color_texture` | `ideatek_texture_slot_t` | 0 | No default | The texture of the base color. |
| `metallic_roughness_texture` | `ideatek_texture_slot_t` | 32 | No default | Roughness in G, metallic in B. |
| `normal_texture` | `ideatek_texture_slot_t` | 64 | No default | The normals, in tangent space. |
| `occlusion_texture` | `ideatek_texture_slot_t` | 96 | No default | The occlusion, in R. |
| `emissive_texture` | `ideatek_texture_slot_t` | 128 | No default | The emitted color. |
| `base_color` | `ideatek_color_t` | 160 | 1, 1, 1, 1 | The base color. |
| `emissive` | `ideatek_rgb_t` | 176 | 0, 0, 0 | The emitted color. |
| `metallic` | `float` | 188 | 1 | The metal part, from 0 to 1. |
| `roughness` | `float` | 192 | 1 | The roughness, from 0 to 1. |
| `emissive_strength` | `float` | 196 | 1 | The factor of `emissive`. |
| `normal_scale` | `float` | 200 | 1 | The factor of the X and Y of the normals. |
| `occlusion_strength` | `float` | 204 | 1 | The strength of the occlusion. |
| `alpha_cutoff` | `float` | 208 | 0.5 | The alpha limit of `IDEATEK_ALPHA_MODE_MASK`. |
| `alpha_mode` | `ideatek_alpha_mode_t` | 212 | `IDEATEK_ALPHA_MODE_OPAQUE` | The use of the alpha. |
| `double_sided` | `bool` | 213 | `false` | `true` if both sides draw. |

Bytes 214 to 215 hold no field.

The defaults are the defaults of glTF 2.1, whose core holds `KHR_materials_emissive_strength`.
A custom material is another type on the same entity: *ideatek:MeshMaterials*.

## ideatek:Texture

`ideatek:Texture` is an image on a resource entity.
`ideatek_texture_t` takes 40 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The texels: *The bytes of a texture*. |
| `format` | `uint32_t` | 8 | The `VkFormat` of Vulkan, as `vkFormat` of KTX 2.0. |
| `width` | `uint32_t` | 12 | The width of level 0, in texels. |
| `height` | `uint32_t` | 16 | The height of level 0. |
| `depth_or_layers` | `uint32_t` | 20 | The depth of a volume, or the number of layers. |
| `mip_count` | `uint32_t` | 24 | The number of levels. |
| `top_mip` | `uint32_t` | 28 | The largest level in `bytes`: *The streaming of a texture*. |
| `revision` | `uint32_t` | 32 | Raised at each change. |
| `dimension` | `ideatek_dimension_t` | 36 | The shape. |

Bytes 37 to 39 hold no field.

A format of 0, a scaled format and a multi-planar format are not allowed.
Appendix B of KTX 2.0 maps each format to DXGI, Metal and OpenGL.
A renderer that cannot read the format logs the entity once. It does not convert the texels.

## ideatek:Sampler

`ideatek:Sampler` tells how a texture is read. It is on the entity of the texture.
`ideatek_sampler_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `max_anisotropy` | `float` | 0 | 1 | The most texels of a slanted read. 1 means no anisotropy. |
| `min_filter` | `ideatek_filter_t` | 4 | `IDEATEK_FILTER_LINEAR` | The filter of a small view. |
| `mag_filter` | `ideatek_filter_t` | 5 | `IDEATEK_FILTER_LINEAR` | The filter of a large view. |
| `mip_filter` | `ideatek_filter_t` | 6 | `IDEATEK_FILTER_LINEAR` | The filter between levels. |
| `wrap_u` | `ideatek_wrap_t` | 7 | `IDEATEK_WRAP_REPEAT` | The wrap on U. |
| `wrap_v` | `ideatek_wrap_t` | 8 | `IDEATEK_WRAP_REPEAT` | The wrap on V. |
| `wrap_w` | `ideatek_wrap_t` | 9 | `IDEATEK_WRAP_REPEAT` | The wrap on W. |

Bytes 10 to 11 hold no field.

## ideatek:Light

`ideatek:Light` is a light at the place of its entity.
`ideatek_light_t` takes 48 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `cookie` | `ideatek_entity_t` | 0 | 0 | A texture that shapes the light. 0 means none. |
| `color` | `ideatek_rgb_t` | 8 | 1, 1, 1 | The color. |
| `intensity` | `float` | 20 | 1 | Candela, or lux for a directional light. |
| `range` | `float` | 24 | 0 | The distance where the light ends. 0 means no end. |
| `source_radius` | `float` | 28 | 0 | The radius of the source, for soft shadows and the disk of a sun. |
| `inner_cone` | `float` | 32 | 0 | The angle of the full light of a spot. |
| `outer_cone` | `float` | 36 | pi / 4 | The angle where the light of a spot ends. |
| `kind` | `ideatek_light_kind_t` | 40 | No default | The kind. |
| `shadow` | `bool` | 41 | `false` | `true` if the light casts shadows. |

Bytes 42 to 47 hold no field.

The defaults of `color`, `intensity` and the cones are the defaults of `KHR_lights_punctual`.
The render module tunes the shadows with its own settings. A loader turns a temperature into `color`.

## ideatek:Decal

`ideatek:Decal` projects a material on the surfaces inside a box, along -Z.
`ideatek_decal_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `material` | `ideatek_entity_t` | 0 | No default | The material entity, with `ideatek:Material`. |
| `half_size` | `ideatek_vec3_t` | 8 | No default | The half extents of the box. |
| `normal_fade` | `float` | 20 | 0 | The fade of the decal on surfaces that face away, from 0 to 1. |
| `sort_order` | `int32_t` | 24 | 0 | Decals draw by rising order. |

Bytes 28 to 31 hold no field.

The decal lives as long as its entity. The render module limits the count and the distance with its settings.

## ideatek:Fog

`ideatek:Fog` is a medium of the air: distance fog, height fog, or a volume.
`ideatek_fog_t` takes 56 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `size` | `ideatek_vec3_t` | 0 | No default | The half extents or the radii of the shape. |
| `albedo` | `ideatek_rgb_t` | 12 | 1, 1, 1 | The color that the fog scatters. |
| `emission` | `ideatek_rgb_t` | 24 | 0, 0, 0 | The light that the fog gives. |
| `density` | `float` | 36 | 0 | The extinction, in 1 / m. |
| `height_falloff` | `float` | 40 | 0 | The thinning above the Y of the entity, in 1 / m. 0 means uniform. |
| `anisotropy` | `float` | 44 | 0.2 | The forward part of the scatter, from -1 to 1. |
| `edge` | `float` | 48 | 0 | The fade at the border of a shape. |
| `shape` | `ideatek_fog_shape_t` | 52 | `IDEATEK_FOG_SHAPE_INFINITE` | The place. |

Bytes 53 to 55 hold no field.

`albedo` and `anisotropy` take the shared defaults of Godot and Unreal. The 2 do not agree on a density, so the game sets it.

## ideatek:Sky

`ideatek:Sky` is the background of the world and a source of light.
`ideatek_sky_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_entity_t` | 0 | 0 | A cube, or a 2D texture of a full panorama. |
| `intensity` | `float` | 8 | 1 | The factor of the light of the sky. |
| `priority` | `int32_t` | 12 | 0 | The sky with the highest priority is active. |
| `kind` | `ideatek_sky_kind_t` | 16 | `IDEATEK_SKY_KIND_ATMOSPHERE` | The source. |

Bytes 17 to 23 hold no field.

## ideatek:Atmosphere

`ideatek:Atmosphere` is the air of a planet. It is on the entity of a sky.
`ideatek_atmosphere_t` takes 56 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `rayleigh` | `ideatek_rgb_t` | 0 | 5.802e-6, 13.558e-6, 33.1e-6 | The Rayleigh scatter, in 1 / m. |
| `ozone` | `ideatek_rgb_t` | 12 | 6.5e-7, 1.881e-6, 8.5e-8 | The absorption of the ozone, in 1 / m. |
| `bottom_radius` | `float` | 24 | 6360000 | The radius of the planet. |
| `height` | `float` | 28 | 60000 | The height of the air. |
| `rayleigh_height` | `float` | 32 | 8000 | The scale height of the Rayleigh scatter. |
| `mie` | `float` | 36 | 3.996e-6 | The Mie scatter, in 1 / m. |
| `mie_absorption` | `float` | 40 | 4.44e-6 | The Mie absorption, in 1 / m. |
| `mie_height` | `float` | 44 | 1200 | The scale height of the Mie scatter. |
| `mie_anisotropy` | `float` | 48 | 0.8 | The forward part of the Mie scatter. |
| `ground_albedo` | `float` | 52 | 0.4 | The part of the light that the ground gives back. |

The defaults are the values of the Earth of Hillaire 2020 and Bruneton 2017.
The top of the planet is at the origin of the world.

## ideatek:CloudLayer

`ideatek:CloudLayer` is a layer of clouds. It is on the entity of a sky.
`ideatek_cloud_layer_t` takes 24 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `coverage_texture` | `ideatek_entity_t` | 0 | 0 | A 2D map of the coverage. 0 means uniform. |
| `bottom` | `float` | 8 | 0 | The height of the bottom of the layer. |
| `thickness` | `float` | 12 | 0 | The height of the layer. |
| `coverage` | `float` | 16 | 0 | The part of the sky with clouds, from 0 to 1. |
| `density` | `float` | 20 | 0 | The extinction in a cloud, in 1 / m. |

The wind of the physics part moves the coverage.
Only 1 reference has clouds, so each default is 0.

## ideatek:Exposure

`ideatek:Exposure` sets the exposure of a camera.
`ideatek_exposure_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `ev100` | `float` | 0 | 15 | The exposure value at ISO 100. |

The default is the sunny-16 rule of photography. It suits a sun in lux.

## ideatek:AutoExposure

`ideatek:AutoExposure` adapts the exposure of a camera to the light of its view.
`ideatek_auto_exposure_t` takes 20 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `min_ev` | `float` | 0 | -10 | The lowest exposure value. |
| `max_ev` | `float` | 4 | 20 | The highest exposure value. |
| `compensation` | `float` | 8 | 0 | The offset of the result. |
| `speed_up` | `float` | 12 | 3 | The change to a brighter view, in EV / s. |
| `speed_down` | `float` | 16 | 1 | The change to a darker view, in EV / s. |

The range covers the ranges of Godot and Unreal. Unreal and Bevy share the speeds.

## ideatek:Tonemap

`ideatek:Tonemap` maps the light of a camera to the range of the screen.
`ideatek_tonemap_t` takes 1 byte. Its alignment is 1.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `curve` | `ideatek_tonemap_curve_t` | 0 | `IDEATEK_TONEMAP_CURVE_AGX` | The curve. |

## ideatek:Bloom

`ideatek:Bloom` makes bright light glow.
`ideatek_bloom_t` takes 8 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `intensity` | `float` | 0 | 0 | The strength of the glow. |
| `threshold` | `float` | 4 | 0 | The light under which nothing glows. |

The references do not agree on a strength, so the default is 0.

## ideatek:ColorGrade

`ideatek:ColorGrade` changes the colors of a camera.
`ideatek_color_grade_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `lut` | `ideatek_entity_t` | 0 | 0 | A 3D texture that maps colors. 0 means none. |
| `saturation` | `float` | 8 | 1 | The factor of the saturation. |
| `contrast` | `float` | 12 | 1 | The factor of the contrast. |

## ideatek:DepthOfField

`ideatek:DepthOfField` blurs what is not at the focus distance.
`ideatek_depth_of_field_t` takes 12 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `focus_distance` | `float` | 0 | No default | The distance that is sharp. |
| `f_stop` | `float` | 4 | No default | The aperture, as an f-number. |
| `sensor_width` | `float` | 8 | 24.576 | The width of the sensor, in mm. |

The focal length comes from `fov` of `ideatek:Projection` and from `sensor_width`.
The default sensor is APS-C. The game sets the focus and the f-number.

## ideatek:MotionBlur

`ideatek:MotionBlur` blurs what moves.
`ideatek_motion_blur_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `shutter` | `float` | 0 | 0.5 | The part of a frame in which the shutter is open. |

## ideatek:Vignette

`ideatek:Vignette` darkens the edges of the view.
`ideatek_vignette_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `intensity` | `float` | 0 | 0 | The darkness at the corners. |

## ideatek:ChromaticAberration

`ideatek:ChromaticAberration` splits the colors as a cheap lens does.
`ideatek_chromatic_aberration_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `intensity` | `float` | 0 | 0 | The split of the colors at the edges. |

## ideatek:FilmGrain

`ideatek:FilmGrain` adds the noise of a film.
`ideatek_film_grain_t` takes 4 bytes. Its alignment is 4.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `intensity` | `float` | 0 | 0 | The strength of the noise. |

## ideatek:LensFlare

`ideatek:LensFlare` gives the ghosts that bright light makes in a lens.
`ideatek_lens_flare_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `bokeh` | `ideatek_entity_t` | 0 | 0 | A texture of the shape. 0 means round. |
| `tint` | `ideatek_rgb_t` | 8 | 1, 1, 1 | The color of the ghosts. |
| `intensity` | `float` | 20 | 0 | The strength. |
| `threshold` | `float` | 24 | 0 | The light under which nothing flares. |

Bytes 28 to 31 hold no field.

Only 1 reference has a lens flare, so the default strength is 0.

## ideatek:MaterialClearcoat

`ideatek:MaterialClearcoat` adds a clear coat on the material, as `KHR_materials_clearcoat`.
`ideatek_material_clearcoat_t` takes 104 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_texture_slot_t` | 0 | No default | The strength of the coat, in R. |
| `roughness_texture` | `ideatek_texture_slot_t` | 32 | No default | The roughness of the coat, in G. |
| `normal_texture` | `ideatek_texture_slot_t` | 64 | No default | The normals of the coat, in tangent space. |
| `factor` | `float` | 96 | 0 | The strength of the coat, from 0 to 1. |
| `roughness` | `float` | 100 | 0 | The roughness of the coat, from 0 to 1. |

## ideatek:MaterialSpecular

`ideatek:MaterialSpecular` sets the specular light, as `KHR_materials_specular` and `KHR_materials_ior`.
`ideatek_material_specular_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_texture_slot_t` | 0 | No default | The strength of the specular light, in A. |
| `color_texture` | `ideatek_texture_slot_t` | 32 | No default | The color of the specular light, in RGB. |
| `color` | `ideatek_rgb_t` | 64 | 1, 1, 1 | The color of the specular light. |
| `factor` | `float` | 76 | 1 | The strength of the specular light, from 0 to 1. |
| `ior` | `float` | 80 | 1.5 | The index of refraction. |

Bytes 84 to 87 hold no field.

## ideatek:MaterialTransmission

`ideatek:MaterialTransmission` lets light go through the material, as `KHR_materials_transmission` and `KHR_materials_volume`.
`ideatek_material_transmission_t` takes 88 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_texture_slot_t` | 0 | No default | The part of the light that goes through, in R. |
| `thickness_texture` | `ideatek_texture_slot_t` | 32 | No default | The thickness of the volume, in G. |
| `attenuation_color` | `ideatek_rgb_t` | 64 | 1, 1, 1 | The color that the light gets at `attenuation_distance`. |
| `factor` | `float` | 76 | 0 | The part of the light that goes through, from 0 to 1. |
| `thickness` | `float` | 80 | 0 | The thickness of the volume, in meters. 0 means a thin surface. |
| `attenuation_distance` | `float` | 84 | 0 | The distance in the volume where the light gets `attenuation_color`. 0 means no attenuation. |

## ideatek:MaterialAnisotropy

`ideatek:MaterialAnisotropy` stretches the highlights along a direction, as `KHR_materials_anisotropy`.
`ideatek_material_anisotropy_t` takes 40 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_texture_slot_t` | 0 | No default | The direction in RG and the strength in B. |
| `strength` | `float` | 32 | 0 | The strength, from 0 to 1. |
| `rotation` | `float` | 36 | 0 | The turn of the direction, in radians. |

## ideatek:MaterialDiffuseTransmission

`ideatek:MaterialDiffuseTransmission` lets light scatter through a thin material, as leaves, as `KHR_materials_diffuse_transmission`.
`ideatek_material_diffuse_transmission_t` takes 80 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_texture_slot_t` | 0 | No default | The part of the light that scatters through, in A. |
| `color_texture` | `ideatek_texture_slot_t` | 32 | No default | The color of that light, in RGB. |
| `color` | `ideatek_rgb_t` | 64 | 1, 1, 1 | The color of that light. |
| `factor` | `float` | 76 | 0 | The part of the light that scatters through, from 0 to 1. |

## ideatek:MaterialUnlit

`ideatek:MaterialUnlit` is a tag. The material shows its base color, with no light, as `KHR_materials_unlit`.

## ideatek:ReflectionProbe

`ideatek:ReflectionProbe` gives the reflected light in a box: *The baked light*.
`ideatek_reflection_probe_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `cube` | `ideatek_entity_t` | 0 | No default | A cube texture of the light around the place, with a level for each roughness. |
| `half_size` | `ideatek_vec3_t` | 8 | No default | The half extents of the box that the probe lights. |
| `falloff` | `float` | 20 | 0.25 | The part of the box at its border where the probe fades, from 0 to 1. |
| `intensity` | `float` | 24 | 1 | The factor of the light of the probe. |
| `priority` | `int32_t` | 28 | 0 | The rank among the probes that overlap. |

## ideatek:IrradianceVolume

`ideatek:IrradianceVolume` gives the diffuse light in a box, from a grid of probes: *The baked light*.
`ideatek_irradiance_volume_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_entity_t` | 0 | No default | A 3D texture of the probes of a grid. |
| `half_size` | `ideatek_vec3_t` | 8 | No default | The half extents of the box of the grid. |
| `intensity` | `float` | 20 | 1 | The factor of the light of the volume. |
| `priority` | `int32_t` | 24 | 0 | The rank among the volumes that overlap. |

Bytes 28 to 31 hold no field.

The texture is 6 times the probes wide on X. The texel (6 i + f, j, k) holds the face f of the probe (i, j, k).
The faces are in the order +X, -X, +Y, -Y, +Z, -Z. The probes fill the box evenly.

## ideatek:Lightmap

`ideatek:Lightmap` gives the baked light of a drawn mesh. It is on the entity of `ideatek:MeshInstance`.
`ideatek_lightmap_t` takes 32 bytes. Its alignment is 8.

| Field | C type | Offset | Default | Value |
| --- | --- | --- | --- | --- |
| `texture` | `ideatek_entity_t` | 0 | No default | The texture of the baked light. |
| `uv_rect` | `ideatek_rect_t` | 8 | No default | The part of the texture that the mesh uses. |
| `texcoord` | `uint8_t` | 24 | 1 | The set of texture coordinates of the lightmap. |

Bytes 25 to 31 hold no field.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_color_t` | `ideatek_color_t` | `ideatek_color_t_desc` | `IDEATEK_COLOR_FIELDS` |
| `ideatek:ideatek_rgb_t` | `ideatek_rgb_t` | `ideatek_rgb_t_desc` | `IDEATEK_RGB_FIELDS` |
| `ideatek:ideatek_rect_t` | `ideatek_rect_t` | `ideatek_rect_t_desc` | `IDEATEK_RECT_FIELDS` |
| `ideatek:ideatek_projection_kind_t` | `ideatek_projection_kind_t` | `ideatek_projection_kind_t_desc` | `IDEATEK_PROJECTION_KIND_VALUES` |
| `ideatek:ideatek_topology_t` | `ideatek_topology_t` | `ideatek_topology_t_desc` | `IDEATEK_TOPOLOGY_VALUES` |
| `ideatek:ideatek_index_format_t` | `ideatek_index_format_t` | `ideatek_index_format_t_desc` | `IDEATEK_INDEX_FORMAT_VALUES` |
| `ideatek:ideatek_semantic_t` | `ideatek_semantic_t` | `ideatek_semantic_t_desc` | `IDEATEK_SEMANTIC_VALUES` |
| `ideatek:ideatek_mesh_stream_t` | `ideatek_mesh_stream_t` | `ideatek_mesh_stream_t_desc` | `IDEATEK_MESH_STREAM_FIELDS` |
| `ideatek:ideatek_submesh_t` | `ideatek_submesh_t` | `ideatek_submesh_t_desc` | `IDEATEK_SUBMESH_FIELDS` |
| `ideatek:ideatek_mesh_lod_t` | `ideatek_mesh_lod_t` | `ideatek_mesh_lod_t_desc` | `IDEATEK_MESH_LOD_FIELDS` |
| `ideatek:ideatek_texture_slot_t` | `ideatek_texture_slot_t` | `ideatek_texture_slot_t_desc` | `IDEATEK_TEXTURE_SLOT_FIELDS` |
| `ideatek:ideatek_gi_mode_t` | `ideatek_gi_mode_t` | `ideatek_gi_mode_t_desc` | `IDEATEK_GI_MODE_VALUES` |
| `ideatek:ideatek_alpha_mode_t` | `ideatek_alpha_mode_t` | `ideatek_alpha_mode_t_desc` | `IDEATEK_ALPHA_MODE_VALUES` |
| `ideatek:ideatek_dimension_t` | `ideatek_dimension_t` | `ideatek_dimension_t_desc` | `IDEATEK_DIMENSION_VALUES` |
| `ideatek:ideatek_filter_t` | `ideatek_filter_t` | `ideatek_filter_t_desc` | `IDEATEK_FILTER_VALUES` |
| `ideatek:ideatek_wrap_t` | `ideatek_wrap_t` | `ideatek_wrap_t_desc` | `IDEATEK_WRAP_VALUES` |
| `ideatek:ideatek_light_kind_t` | `ideatek_light_kind_t` | `ideatek_light_kind_t_desc` | `IDEATEK_LIGHT_KIND_VALUES` |
| `ideatek:ideatek_cast_shadow_t` | `ideatek_cast_shadow_t` | `ideatek_cast_shadow_t_desc` | `IDEATEK_CAST_SHADOW_VALUES` |
| `ideatek:ideatek_fog_shape_t` | `ideatek_fog_shape_t` | `ideatek_fog_shape_t_desc` | `IDEATEK_FOG_SHAPE_VALUES` |
| `ideatek:ideatek_sky_kind_t` | `ideatek_sky_kind_t` | `ideatek_sky_kind_t_desc` | `IDEATEK_SKY_KIND_VALUES` |
| `ideatek:ideatek_tonemap_curve_t` | `ideatek_tonemap_curve_t` | `ideatek_tonemap_curve_t_desc` | `IDEATEK_TONEMAP_CURVE_VALUES` |
| `ideatek:Camera` | `ideatek_camera_t` | `ideatek_camera_t_desc` | `IDEATEK_CAMERA_FIELDS` |
| `ideatek:Projection` | `ideatek_projection_t` | `ideatek_projection_t_desc` | `IDEATEK_PROJECTION_FIELDS` |
| `ideatek:ProjectionAngles` | `ideatek_projection_angles_t` | `ideatek_projection_angles_t_desc` | `IDEATEK_PROJECTION_ANGLES_FIELDS` |
| `ideatek:Mesh` | `ideatek_mesh_t` | `ideatek_mesh_t_desc` | `IDEATEK_MESH_FIELDS` |
| `ideatek:MeshInstance` | `ideatek_mesh_instance_t` | `ideatek_mesh_instance_t_desc` | `IDEATEK_MESH_INSTANCE_FIELDS` |
| `ideatek:MeshMaterials` | `ideatek_mesh_materials_t` | `ideatek_mesh_materials_t_desc` | `IDEATEK_MESH_MATERIALS_FIELDS` |
| `ideatek:Visibility` | `ideatek_visibility_t` | `ideatek_visibility_t_desc` | `IDEATEK_VISIBILITY_FIELDS` |
| `ideatek:Affects` | `ideatek_affects_t` | `ideatek_affects_t_desc` | `IDEATEK_AFFECTS_FIELDS` |
| `ideatek:VisibilityRange` | `ideatek_visibility_range_t` | `ideatek_visibility_range_t_desc` | `IDEATEK_VISIBILITY_RANGE_FIELDS` |
| `ideatek:Material` | `ideatek_material_t` | `ideatek_material_t_desc` | `IDEATEK_MATERIAL_FIELDS` |
| `ideatek:Texture` | `ideatek_texture_t` | `ideatek_texture_t_desc` | `IDEATEK_TEXTURE_FIELDS` |
| `ideatek:Sampler` | `ideatek_sampler_t` | `ideatek_sampler_t_desc` | `IDEATEK_SAMPLER_FIELDS` |
| `ideatek:Light` | `ideatek_light_t` | `ideatek_light_t_desc` | `IDEATEK_LIGHT_FIELDS` |
| `ideatek:Decal` | `ideatek_decal_t` | `ideatek_decal_t_desc` | `IDEATEK_DECAL_FIELDS` |
| `ideatek:Fog` | `ideatek_fog_t` | `ideatek_fog_t_desc` | `IDEATEK_FOG_FIELDS` |
| `ideatek:Sky` | `ideatek_sky_t` | `ideatek_sky_t_desc` | `IDEATEK_SKY_FIELDS` |
| `ideatek:Atmosphere` | `ideatek_atmosphere_t` | `ideatek_atmosphere_t_desc` | `IDEATEK_ATMOSPHERE_FIELDS` |
| `ideatek:CloudLayer` | `ideatek_cloud_layer_t` | `ideatek_cloud_layer_t_desc` | `IDEATEK_CLOUD_LAYER_FIELDS` |
| `ideatek:Exposure` | `ideatek_exposure_t` | `ideatek_exposure_t_desc` | `IDEATEK_EXPOSURE_FIELDS` |
| `ideatek:AutoExposure` | `ideatek_auto_exposure_t` | `ideatek_auto_exposure_t_desc` | `IDEATEK_AUTO_EXPOSURE_FIELDS` |
| `ideatek:Tonemap` | `ideatek_tonemap_t` | `ideatek_tonemap_t_desc` | `IDEATEK_TONEMAP_FIELDS` |
| `ideatek:Bloom` | `ideatek_bloom_t` | `ideatek_bloom_t_desc` | `IDEATEK_BLOOM_FIELDS` |
| `ideatek:ColorGrade` | `ideatek_color_grade_t` | `ideatek_color_grade_t_desc` | `IDEATEK_COLOR_GRADE_FIELDS` |
| `ideatek:DepthOfField` | `ideatek_depth_of_field_t` | `ideatek_depth_of_field_t_desc` | `IDEATEK_DEPTH_OF_FIELD_FIELDS` |
| `ideatek:MotionBlur` | `ideatek_motion_blur_t` | `ideatek_motion_blur_t_desc` | `IDEATEK_MOTION_BLUR_FIELDS` |
| `ideatek:Vignette` | `ideatek_vignette_t` | `ideatek_vignette_t_desc` | `IDEATEK_VIGNETTE_FIELDS` |
| `ideatek:ChromaticAberration` | `ideatek_chromatic_aberration_t` | `ideatek_chromatic_aberration_t_desc` | `IDEATEK_CHROMATIC_ABERRATION_FIELDS` |
| `ideatek:FilmGrain` | `ideatek_film_grain_t` | `ideatek_film_grain_t_desc` | `IDEATEK_FILM_GRAIN_FIELDS` |
| `ideatek:LensFlare` | `ideatek_lens_flare_t` | `ideatek_lens_flare_t_desc` | `IDEATEK_LENS_FLARE_FIELDS` |
| `ideatek:MaterialClearcoat` | `ideatek_material_clearcoat_t` | `ideatek_material_clearcoat_t_desc` | `IDEATEK_MATERIAL_CLEARCOAT_FIELDS` |
| `ideatek:MaterialSpecular` | `ideatek_material_specular_t` | `ideatek_material_specular_t_desc` | `IDEATEK_MATERIAL_SPECULAR_FIELDS` |
| `ideatek:MaterialTransmission` | `ideatek_material_transmission_t` | `ideatek_material_transmission_t_desc` | `IDEATEK_MATERIAL_TRANSMISSION_FIELDS` |
| `ideatek:MaterialAnisotropy` | `ideatek_material_anisotropy_t` | `ideatek_material_anisotropy_t_desc` | `IDEATEK_MATERIAL_ANISOTROPY_FIELDS` |
| `ideatek:MaterialDiffuseTransmission` | `ideatek_material_diffuse_transmission_t` | `ideatek_material_diffuse_transmission_t_desc` | `IDEATEK_MATERIAL_DIFFUSE_TRANSMISSION_FIELDS` |
| `ideatek:ReflectionProbe` | `ideatek_reflection_probe_t` | `ideatek_reflection_probe_t_desc` | `IDEATEK_REFLECTION_PROBE_FIELDS` |
| `ideatek:IrradianceVolume` | `ideatek_irradiance_volume_t` | `ideatek_irradiance_volume_t_desc` | `IDEATEK_IRRADIANCE_VOLUME_FIELDS` |
| `ideatek:Lightmap` | `ideatek_lightmap_t` | `ideatek_lightmap_t_desc` | `IDEATEK_LIGHTMAP_FIELDS` |

```text
{ float r; float g; float b; float a; }
{ float r; float g; float b; }
{ float x; float y; float width; float height; }
{ IDEATEK_PROJECTION_KIND_PERSPECTIVE = 0, IDEATEK_PROJECTION_KIND_ORTHOGRAPHIC = 1, IDEATEK_PROJECTION_KIND_ASYMMETRIC = 2 }
{ IDEATEK_TOPOLOGY_POINT_LIST = 0, IDEATEK_TOPOLOGY_LINE_LIST = 1, IDEATEK_TOPOLOGY_LINE_STRIP = 2, IDEATEK_TOPOLOGY_TRIANGLE_LIST = 3, IDEATEK_TOPOLOGY_TRIANGLE_STRIP = 4 }
{ IDEATEK_INDEX_FORMAT_NONE = 0, IDEATEK_INDEX_FORMAT_U16 = 1, IDEATEK_INDEX_FORMAT_U32 = 2 }
{ IDEATEK_SEMANTIC_POSITION = 0, IDEATEK_SEMANTIC_NORMAL = 1, IDEATEK_SEMANTIC_TANGENT = 2, IDEATEK_SEMANTIC_TEXCOORD = 3, IDEATEK_SEMANTIC_COLOR = 4, IDEATEK_SEMANTIC_JOINTS = 5, IDEATEK_SEMANTIC_WEIGHTS = 6, IDEATEK_SEMANTIC_MORPH_POSITION = 7, IDEATEK_SEMANTIC_MORPH_NORMAL = 8, IDEATEK_SEMANTIC_MORPH_TANGENT = 9, IDEATEK_SEMANTIC_SOFT_MAX_DISTANCE = 10, IDEATEK_SEMANTIC_SOFT_BACKSTOP_DISTANCE = 11, IDEATEK_SEMANTIC_SOFT_BACKSTOP_RADIUS = 12 }
{ uint32_t format; uint32_t offset; uint32_t stride; uint16_t target; ideatek_semantic_t semantic; uint8_t set; }
{ uint32_t first; uint32_t count; }
{ float error; uint32_t first; uint32_t count; }
{ ideatek_entity_t texture; float offset[2]; float scale[2]; float rotation; uint8_t texcoord; }
{ IDEATEK_GI_MODE_DYNAMIC = 0, IDEATEK_GI_MODE_STATIC = 1, IDEATEK_GI_MODE_DISABLED = 2 }
{ IDEATEK_ALPHA_MODE_OPAQUE = 0, IDEATEK_ALPHA_MODE_MASK = 1, IDEATEK_ALPHA_MODE_BLEND = 2 }
{ IDEATEK_DIMENSION_2D = 0, IDEATEK_DIMENSION_2D_ARRAY = 1, IDEATEK_DIMENSION_CUBE = 2, IDEATEK_DIMENSION_3D = 3 }
{ IDEATEK_FILTER_NEAREST = 0, IDEATEK_FILTER_LINEAR = 1 }
{ IDEATEK_WRAP_REPEAT = 0, IDEATEK_WRAP_MIRRORED_REPEAT = 1, IDEATEK_WRAP_CLAMP_TO_EDGE = 2 }
{ IDEATEK_LIGHT_KIND_DIRECTIONAL = 0, IDEATEK_LIGHT_KIND_POINT = 1, IDEATEK_LIGHT_KIND_SPOT = 2 }
{ IDEATEK_CAST_SHADOW_OFF = 0, IDEATEK_CAST_SHADOW_ON = 1, IDEATEK_CAST_SHADOW_ONLY = 2 }
{ IDEATEK_FOG_SHAPE_INFINITE = 0, IDEATEK_FOG_SHAPE_BOX = 1, IDEATEK_FOG_SHAPE_ELLIPSOID = 2 }
{ IDEATEK_SKY_KIND_ATMOSPHERE = 0, IDEATEK_SKY_KIND_TEXTURE = 1 }
{ IDEATEK_TONEMAP_CURVE_AGX = 0, IDEATEK_TONEMAP_CURVE_NEUTRAL = 1, IDEATEK_TONEMAP_CURVE_ACES = 2, IDEATEK_TONEMAP_CURVE_LINEAR = 3 }
{ ideatek_entity_t target; uint64_t groups; ideatek_rect_t viewport; int32_t order; bool active; }
{ float fov; float height; float near; float far; ideatek_projection_kind_t kind; }
{ float left; float right; float up; float down; }
{ bridge_text_t bytes; uint32_t vertex_count; uint32_t index_count; uint32_t index_offset; uint32_t stream_count; uint32_t submesh_count; uint32_t lod_count; uint32_t target_count; uint32_t revision; ideatek_topology_t topology; ideatek_index_format_t index_format; }
{ ideatek_entity_t mesh; float lod_bias; }
{ bridge_text_t materials; }
{ uint64_t groups; ideatek_cast_shadow_t cast_shadow; ideatek_gi_mode_t gi_mode; bool visible; }
{ uint64_t groups; }
{ float begin; float end; float begin_margin; float end_margin; }
{ ideatek_texture_slot_t base_color_texture; ideatek_texture_slot_t metallic_roughness_texture; ideatek_texture_slot_t normal_texture; ideatek_texture_slot_t occlusion_texture; ideatek_texture_slot_t emissive_texture; ideatek_color_t base_color; ideatek_rgb_t emissive; float metallic; float roughness; float emissive_strength; float normal_scale; float occlusion_strength; float alpha_cutoff; ideatek_alpha_mode_t alpha_mode; bool double_sided; }
{ bridge_text_t bytes; uint32_t format; uint32_t width; uint32_t height; uint32_t depth_or_layers; uint32_t mip_count; uint32_t top_mip; uint32_t revision; ideatek_dimension_t dimension; }
{ float max_anisotropy; ideatek_filter_t min_filter; ideatek_filter_t mag_filter; ideatek_filter_t mip_filter; ideatek_wrap_t wrap_u; ideatek_wrap_t wrap_v; ideatek_wrap_t wrap_w; }
{ ideatek_entity_t cookie; ideatek_rgb_t color; float intensity; float range; float source_radius; float inner_cone; float outer_cone; ideatek_light_kind_t kind; bool shadow; }
{ ideatek_entity_t material; ideatek_vec3_t half_size; float normal_fade; int32_t sort_order; }
{ ideatek_vec3_t size; ideatek_rgb_t albedo; ideatek_rgb_t emission; float density; float height_falloff; float anisotropy; float edge; ideatek_fog_shape_t shape; }
{ ideatek_entity_t texture; float intensity; int32_t priority; ideatek_sky_kind_t kind; }
{ ideatek_rgb_t rayleigh; ideatek_rgb_t ozone; float bottom_radius; float height; float rayleigh_height; float mie; float mie_absorption; float mie_height; float mie_anisotropy; float ground_albedo; }
{ ideatek_entity_t coverage_texture; float bottom; float thickness; float coverage; float density; }
{ float ev100; }
{ float min_ev; float max_ev; float compensation; float speed_up; float speed_down; }
{ ideatek_tonemap_curve_t curve; }
{ float intensity; float threshold; }
{ ideatek_entity_t lut; float saturation; float contrast; }
{ float focus_distance; float f_stop; float sensor_width; }
{ float shutter; }
{ float intensity; }
{ float intensity; }
{ float intensity; }
{ ideatek_entity_t bokeh; ideatek_rgb_t tint; float intensity; float threshold; }
{ ideatek_texture_slot_t texture; ideatek_texture_slot_t roughness_texture; ideatek_texture_slot_t normal_texture; float factor; float roughness; }
{ ideatek_texture_slot_t texture; ideatek_texture_slot_t color_texture; ideatek_rgb_t color; float factor; float ior; }
{ ideatek_texture_slot_t texture; ideatek_texture_slot_t thickness_texture; ideatek_rgb_t attenuation_color; float factor; float thickness; float attenuation_distance; }
{ ideatek_texture_slot_t texture; float strength; float rotation; }
{ ideatek_texture_slot_t texture; ideatek_texture_slot_t color_texture; ideatek_rgb_t color; float factor; }
{ ideatek_entity_t cube; ideatek_vec3_t half_size; float falloff; float intensity; int32_t priority; }
{ ideatek_entity_t texture; ideatek_vec3_t half_size; float intensity; int32_t priority; }
{ ideatek_entity_t texture; ideatek_rect_t uv_rect; uint8_t texcoord; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_vec3_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_color_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_rgb_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_rect_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_projection_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_topology_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_index_format_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_semantic_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_mesh_stream_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_submesh_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_mesh_lod_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_texture_slot_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_gi_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_alpha_mode_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_dimension_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_filter_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_wrap_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_light_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_cast_shadow_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_fog_shape_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_sky_kind_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ideatek_tonemap_curve_t` | `BRIDGE_ENUM` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Camera` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Projection` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ProjectionAngles` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Mesh` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MeshInstance` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MeshMaterials` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Visibility` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Affects` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:VisibilityRange` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Material` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Texture` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Sampler` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Light` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Decal` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Fog` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Sky` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Atmosphere` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:CloudLayer` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Exposure` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:AutoExposure` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Tonemap` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Bloom` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ColorGrade` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:DepthOfField` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MotionBlur` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Vignette` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ChromaticAberration` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:FilmGrain` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:LensFlare` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MaterialClearcoat` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MaterialSpecular` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MaterialTransmission` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MaterialAnisotropy` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MaterialDiffuseTransmission` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:MaterialUnlit` | `BRIDGE_TAG` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ReflectionProbe` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:IrradianceVolume` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Lightmap` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| An author | Each type of the part | The types of the fields |
| The render module | No type. | Each type of the part, `ideatek:WorldTransform`, and the types of the fields |

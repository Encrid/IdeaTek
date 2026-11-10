# include/engine_subsystems/ideatek_scene.h

`include/engine_subsystems/ideatek_scene.h` gives the types of the models: the nodes of a file as a resource, and the copies of a model under an entity.
`readme.md`, *The contracts*, tells the rules of each contract.
A node holds the place of the space part. The tables hold lights and projections of the rendering part.
So the header includes `ideatek_space.h` and `ideatek_rendering.h`.

## The roles

- A loader or the game is the author. It writes `ideatek:Model` on a resource entity, and names the meshes, the materials and the skin binds of the nodes by entity.
- The game puts `ideatek:ModelInstance` on an entity. That entity is the root of the instance.
- The scene module reads `ideatek:Model` and `ideatek:ModelInstance`. It spawns 1 entity for each node of the model, and writes their components: *The node entities*.
- A game can also read `ideatek:Model` with no scene module, for example to assemble the parts of a robot itself.

## The bytes of a model

`bytes` of `ideatek:Model` holds these tables, one after the other, with no gap:

1. `node_count` records of `ideatek_model_node_t`.
2. `light_count` records of `ideatek_light_t` of the rendering part.
3. `material_count` records of `ideatek_entity_t`: the materials of the nodes.
4. `weight_count` values of `float`: the default morph weights of the nodes.
5. `camera_count` records of `ideatek_projection_t` of the rendering part.
6. The names of the nodes in UTF-8, with no zero between them.

All numbers are little-endian. A reader copies each record with `memcpy`.
The order of the tables keeps each record at a multiple of its alignment, when `bytes` starts at a multiple of 8.

## The names

The path of a node is the names from the root of the model to the node, with `/` between them.
The target id of a node comes from its path, by the rule of `docs/extern/engine_subsystems/ideatek_animation.h.md`, *The names*. So a clip of the same file moves the node.
2 nodes can have the same path. The k-th node with a path is the k-th node in the order of the records.

## The joints

The joints of a model are the nodes whose `joint` is not `IDEATEK_MODEL_NONE`. `joint` is the rank of the node among them, from 0 to `joint_count` minus 1.
A mesh of a skinned node names its joints by these ranks in its streams `IDEATEK_SEMANTIC_JOINTS`. `ideatek:SkinBinds` of a node holds `joint_count` matrices, in the order of the ranks.
So 1 instance is 1 skeleton, and each skinned node of the model moves with it.

## The node entities

When an entity has `ideatek:ModelInstance`, and its model has `ideatek:Model`, the scene module spawns 1 entity for each node.
It attaches these components to the entity of each node:

| Component | Value | When |
| --- | --- | --- |
| `ideatek:ModelNode` | The instance and the index of the node. | Each node. |
| `ideatek:Transform` | `position`, `rotation` and `scale` of the node. | Each node. |
| `ideatek:Parent` | The entity of the parent node, or the root of the instance for a root node. `order` is the index of the node, `keep` is `IDEATEK_KEEP_LOCAL`, and `orphan` is `IDEATEK_ORPHAN_DESTROY`. | Each node. |
| `ideatek:AnimationTarget` | `id` is the target id of the node, and `player` is the root of the instance. | Each node. |
| `ideatek:MeshInstance` | `mesh` of the node, with `lod_bias` 1. | `mesh` is not 0. |
| `ideatek:MeshMaterials` | The materials of the node, in the order of the sub-meshes of its mesh. | `material_count` of the node is not 0. |
| `ideatek:MorphWeights` | The default weights of the node. | `weight_count` of the node is not 0. |
| `ideatek:Skin` | `skeleton` is the root of the instance, and `binds` is `binds` of the node. | `binds` is not 0. |
| `ideatek:Joint` | `skeleton` is the root of the instance, and `index` is `joint` of the node. | `joint` is not `IDEATEK_MODEL_NONE`. |
| `ideatek:Light` | The light of the node. | `light` is not `IDEATEK_MODEL_NONE`. |
| `ideatek:Projection` | The projection of the node. | `camera` is not `IDEATEK_MODEL_NONE`. |

The scene module attaches no `ideatek:Camera`, because only the game knows its target. The game attaches it to the node entity.
The texts of `ideatek:MeshMaterials` and `ideatek:MorphWeights` are bytes of the scene module. They stay good while the node entity has the component.

## The changes

- When `ideatek:Model` of the model changes, the scene module updates each instance. The node entity of a path that stays gets the new components, and the components that its new node has no longer go.
  It keeps its entity, so a child that the game attached stays. A node that left is destroyed, and a new node gets a new entity.
- When `model` of `ideatek:ModelInstance` changes, the scene module updates the instance in the same way with the new model.
- When `ideatek:ModelInstance` leaves its entity, the scene module destroys each node entity of the instance. A child that the game attached to a node follows its own `orphan`.
- When `ideatek:Model` leaves the model, the scene module destroys the node entities of each instance of the model. The instance stays, and spawns again when `ideatek:Model` comes back.

The game keeps a use of the model with the resources part while an instance names it: `docs/extern/kernel/ideatek_resources.h.md`, *The handles*.
A change of a node entity that the game makes stays until the next change of its model. An update writes the components of *The node entities* again.

## ideatek:ideatek_model_node_t

`ideatek_model_node_t` is 1 node of a model.
`ideatek_model_node_t` takes 112 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `position` | `ideatek_position_t` | 0 | The position of the node relative to its parent. |
| `rotation` | `ideatek_quat_t` | 24 | The rotation. |
| `scale` | `ideatek_vec3_t` | 40 | The scale on each axis. |
| `mesh` | `ideatek_entity_t` | 56 | The resource entity of the mesh, or 0. |
| `binds` | `ideatek_entity_t` | 64 | The resource entity with `ideatek:SkinBinds`, or 0. |
| `parent` | `uint32_t` | 72 | The index of the parent node, or `IDEATEK_MODEL_NONE` for a root node. |
| `name_offset` | `uint32_t` | 76 | The first byte of the name in the names. |
| `name_length` | `uint32_t` | 80 | The bytes of the name. |
| `material_first` | `uint32_t` | 84 | The first material of the node in the table of the materials. |
| `material_count` | `uint32_t` | 88 | The materials of the node. |
| `weight_first` | `uint32_t` | 92 | The first weight of the node in the table of the weights. |
| `weight_count` | `uint32_t` | 96 | The weights of the node. |
| `light` | `uint32_t` | 100 | The index of the light in the table of the lights, or `IDEATEK_MODEL_NONE`. |
| `camera` | `uint32_t` | 104 | The index of the projection in the table of the cameras, or `IDEATEK_MODEL_NONE`. |
| `joint` | `uint32_t` | 108 | The rank of the node among the joints, or `IDEATEK_MODEL_NONE`: *The joints*. |

Bytes 52 to 55 hold no field. The 3 fields of the place have the offsets of `ideatek_transform_t` of the space part.
A parent comes before its children in the records. So a reader spawns the nodes in the order of the records.
`IDEATEK_MODEL_NONE` is `0xFFFFFFFF`.

## ideatek:Model

`ideatek:Model` is a tree of nodes on a resource entity.
`ideatek_model_t` takes 36 bytes. Its alignment is 4.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `bytes` | `bridge_text_t` | 0 | The tables and the names: *The bytes of a model*. |
| `node_count` | `uint32_t` | 8 | The number of nodes. |
| `light_count` | `uint32_t` | 12 | The number of lights. |
| `material_count` | `uint32_t` | 16 | The number of materials. |
| `weight_count` | `uint32_t` | 20 | The number of weights. |
| `camera_count` | `uint32_t` | 24 | The number of cameras. |
| `joint_count` | `uint32_t` | 28 | The number of joints. |
| `revision` | `uint32_t` | 32 | Raised at each change. |

The writer owns the bytes. A change of a model gives new bytes and a higher `revision`.
The scene module refuses a model whose index or range is outside its tables, or whose parent comes after its child. It spawns no node of that model, and its log gets 1 line.

## ideatek:ModelInstance

`ideatek:ModelInstance` puts the nodes of a model under its entity.
`ideatek_model_instance_t` takes 8 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `model` | `ideatek_entity_t` | 0 | The resource entity with `ideatek:Model`. |

The root of the instance is its entity. The game places it with `ideatek:Transform`, and plays the clips of the model on it with `ideatek:AnimationPlayer`.

## ideatek:ModelNode

`ideatek:ModelNode` tells the instance and the node of a node entity.
`ideatek_model_node_ref_t` takes 16 bytes. Its alignment is 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `instance` | `ideatek_entity_t` | 0 | The root of the instance. |
| `node` | `uint32_t` | 8 | The index of the node in the model. |

Bytes 12 to 15 hold no field.

A game finds a node by its target id in `ideatek:AnimationTarget`, or by its index in `ideatek:ModelNode`. An attach point of a part, as a socket of a robot, is a node.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_model_node_t` | `ideatek_model_node_t` | `ideatek_model_node_t_desc` | `IDEATEK_MODEL_NODE_FIELDS` |
| `ideatek:Model` | `ideatek_model_t` | `ideatek_model_t_desc` | `IDEATEK_MODEL_FIELDS` |
| `ideatek:ModelInstance` | `ideatek_model_instance_t` | `ideatek_model_instance_t_desc` | `IDEATEK_MODEL_INSTANCE_FIELDS` |
| `ideatek:ModelNode` | `ideatek_model_node_ref_t` | `ideatek_model_node_ref_t_desc` | `IDEATEK_MODEL_NODE_REF_FIELDS` |

`IDEATEK_MODEL_NONE` is the index of no node, no light, no camera and no joint.

```text
{ ideatek_position_t position; ideatek_quat_t rotation; ideatek_vec3_t scale; ideatek_entity_t mesh; ideatek_entity_t binds; uint32_t parent; uint32_t name_offset; uint32_t name_length; uint32_t material_first; uint32_t material_count; uint32_t weight_first; uint32_t weight_count; uint32_t light; uint32_t camera; uint32_t joint; }
{ bridge_text_t bytes; uint32_t node_count; uint32_t light_count; uint32_t material_count; uint32_t weight_count; uint32_t camera_count; uint32_t joint_count; uint32_t revision; }
{ ideatek_entity_t model; }
{ ideatek_entity_t instance; uint32_t node; }
```

## The declarations

| Type of Bridge | Kind | Storage layout | `on_destroy` |
| --- | --- | --- | --- |
| `ideatek:ideatek_model_node_t` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:Model` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ModelInstance` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |
| `ideatek:ModelNode` | `BRIDGE_STRUCT` | `BRIDGE_TABLE` | `BRIDGE_ON_DESTROY_REMOVE` |

A module declares `ideatek:ideatek_entity_t`, `ideatek:ideatek_vec3_t`, `ideatek:ideatek_quat_t` and `ideatek:ideatek_position_t` before `ideatek:ideatek_model_node_t`.

## The grants

| Role | `write` | `read` |
| --- | --- | --- |
| An author | `ideatek:Model` | The types of the fields |
| The game | `ideatek:ModelInstance` | `ideatek:Model`, `ideatek:ModelNode` and the types of the fields |
| The scene module | `ideatek:ModelNode`, and the 10 types of *The node entities* of the space, rendering and animation parts | `ideatek:Model`, `ideatek:ModelInstance` and the types of the fields |

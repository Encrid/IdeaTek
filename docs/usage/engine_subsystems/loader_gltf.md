# BM-IdeaTek-Loader-Gltf

The glTF 2.0 loader of IdeaTek: models, meshes, materials, skin binds and clips from a file of the resources part.

## Install

The release of `BM-IdeaTek-Loader-Gltf` has the tag `loader_gltf-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Loader-Gltf.zip` and `sha256.txt`. `BM-IdeaTek-Loader-Gltf.zip` has a folder for each target.
In `BM-IdeaTek-Loader-Gltf`, each folder holds the module `loader_gltf.dll`, `loader_gltf.so` or `loader_gltf.dylib`.
Each folder of `BM-IdeaTek-Loader-Gltf.zip` also holds this readme, the licence and the notice of cgltf.

cgltf 1.15 is inside the module, so a deployment needs no other library: https://github.com/jkuhlmann/cgltf/releases/tag/v1.15.
A project names the zip of `BM-IdeaTek-Loader-Gltf` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_loader_gltf:loader@0.1.0`.
Its role is a loader module: `docs/extern/kernel/ideatek_resources.h.md`, *The roles*.
Its `init` spawns the entity of its loader:

| Field | Value |
| --- | --- |
| `name` | `gltf` |
| `type` | `ideatek:Model` |
| `extensions` | `gltf` and `glb` |
| `magic` | Empty: the module reads both forms, so it needs no magic. |
| `ranged` | `false` |

The resource of the file gets `ideatek:Model` of its default scene: `docs/extern/engine_subsystems/ideatek_scene.h.md`.
Each other part of the file is a sub-resource with a label: *The labels*.

### The read of a file

1. cgltf reads the JSON or the GLB container from the bytes of the file. The module checks the data with `cgltf_validate`.
2. The module loads each dependency: *The dependencies*.
3. When each dependency has its handle and each buffer has its bytes, the module builds the resources.
   *The meshes*, *The materials*, *The skins*, *The clips* and *The models* tell how.
4. It attaches each component and each `ideatek:ResourceLabel`, then `ideatek:ResourceOutcome` of the file.

A refused file attaches `ideatek:ResourceOutcome` with `IDEATEK_RESOURCE_ERROR_LOADER_FAILED`, and writes 1 line: *The messages*.

### The labels

| Label | Component | Part of the file |
| --- | --- | --- |
| `mesh/<name>` | `ideatek:Mesh` | A mesh, with each of its primitives. |
| `material/<name>` | `ideatek:Material`, and its extensions | A material. |
| `skin/<name>` | `ideatek:SkinBinds` | A skin. |
| `clip/<name>` | `ideatek:Clip` | An animation. |
| `scene/<name>` | `ideatek:Model` | A scene. The default scene is also the model of the file itself. |

A part with no name or an empty name takes its index as its name, for example `mesh/0`.
A name that 2 parts of 1 kind share gets `.` and its index, for example `mesh/Cube.3`.

### The dependencies

Each dependency is a load whose `owner` is the resource of the file, with `reload_owner` at 1. So a change of a dependency reloads the file.
A path of a dependency is relative to the folder of the file, and in the same source.

- A buffer in the GLB container needs no load. A buffer in a `data:` URI is decoded by cgltf.
- Each other buffer loads with the type `ideatek:RawBytes`. The module copies its bytes in the round of `ideatek:RawBytes`.
- Each image loads with the type `ideatek:Texture`, through the KTX2 loader: `docs/usage/engine_subsystems/loader_ktx2.md`.
  The load has the settings of the sampler of the first texture that uses the image: `docs/usage/engine_subsystems/loader_ktx2.md`, *The sampler*.

An image must be a file with the extension `ktx2`, or the type `image/ktx2`. An image in a buffer view or in a `data:` URI is not read.
Then the slot of its texture stays empty, and the log gets 1 line.

A second sampler for 1 image has no effect, because the resources part keeps the settings of the first load. The log gets 1 line.
A buffer that does not load refuses the file. An image that does not load leaves its slot on a resource with no `ideatek:Texture`.
Then `dependencies` of the resource tells the failure.

### The meshes

Each mesh of the file is 1 `ideatek:Mesh`. Each primitive of the mesh is 1 sub-mesh, in the order of the file.
The primitives share their streams: the vertices of a primitive follow the vertices of the primitive before it.

| Attribute of glTF | Stream | Format |
| --- | --- | --- |
| `POSITION` | `IDEATEK_SEMANTIC_POSITION` | `VK_FORMAT_R32G32B32_SFLOAT` |
| `NORMAL` | `IDEATEK_SEMANTIC_NORMAL` | `VK_FORMAT_R32G32B32_SFLOAT` |
| `TANGENT` | `IDEATEK_SEMANTIC_TANGENT` | `VK_FORMAT_R32G32B32A32_SFLOAT` |
| `TEXCOORD_<n>` | `IDEATEK_SEMANTIC_TEXCOORD`, set `<n>` | `VK_FORMAT_R32G32_SFLOAT` |
| `COLOR_<n>` | `IDEATEK_SEMANTIC_COLOR`, set `<n>` | `VK_FORMAT_R32G32B32A32_SFLOAT` |
| `JOINTS_<n>` | `IDEATEK_SEMANTIC_JOINTS`, set `<n>` | `VK_FORMAT_R16G16B16A16_UINT` |
| `WEIGHTS_<n>` | `IDEATEK_SEMANTIC_WEIGHTS`, set `<n>` | `VK_FORMAT_R32G32B32A32_SFLOAT` |
| `POSITION`, `NORMAL` and `TANGENT` of a morph target | `IDEATEK_SEMANTIC_MORPH_POSITION`, `IDEATEK_SEMANTIC_MORPH_NORMAL` and `IDEATEK_SEMANTIC_MORPH_TANGENT`, with its target | `VK_FORMAT_R32G32B32_SFLOAT` |

- The module unpacks each accessor with cgltf, also a sparse or a normalized accessor. So each stream has the format of the table. A `COLOR_<n>` of 3 values gets an alpha of 1.
- A joint of `JOINTS_<n>` becomes its rank among the joints of the model: `docs/extern/engine_subsystems/ideatek_scene.h.md`, *The joints*.
- A stream that a primitive lacks gets a default value: 0 for each stream, except 1 for each part of a color.
  `WEIGHTS_0` gets 1, 0, 0, 0.
- The indices are `IDEATEK_INDEX_FORMAT_U16` when the mesh has at most 65535 vertices, else `IDEATEK_INDEX_FORMAT_U32`. A primitive with no indices gets the indices 0 to its vertex count minus 1.
- A fan of triangles becomes a list of triangles, and a loop of lines becomes a list of lines. Each other mode keeps its topology.
- The mesh has the topology of its first primitive. A primitive with another topology is skipped, and the log gets 1 line.
- The mesh has 1 level: `lod_count` is 1.

An attribute that is not in the table, for example `_FEATURE_ID_0`, is skipped. A primitive with `KHR_draco_mesh_compression` or `EXT_meshopt_compression` is not read: the log gets 1 line.

### The materials

Each material of the file is 1 `ideatek:Material`, with the fields of `pbrMetallicRoughness` and of the core of glTF 2.0.
Each slot of a texture names the resource of its image, its `texCoord`, and the transform of `KHR_texture_transform`.

| Extension of glTF | Type on the material entity |
| --- | --- |
| `KHR_materials_emissive_strength` | `emissive_strength` of `ideatek:Material` |
| `KHR_materials_clearcoat` | `ideatek:MaterialClearcoat` |
| `KHR_materials_specular` | `ideatek:MaterialSpecular` |
| `KHR_materials_ior` | `ior` of `ideatek:MaterialSpecular`, with the defaults for its other fields |
| `KHR_materials_transmission` | `ideatek:MaterialTransmission` |
| `KHR_materials_volume` | `thickness`, `thickness_texture`, `attenuation_color` and `attenuation_distance` of `ideatek:MaterialTransmission` |
| `KHR_materials_anisotropy` | `ideatek:MaterialAnisotropy` |
| `KHR_materials_diffuse_transmission` | `ideatek:MaterialDiffuseTransmission` |
| `KHR_materials_unlit` | `ideatek:MaterialUnlit` |

An extension of a material that the table does not hold, for example `KHR_materials_sheen`, has no type. The log gets 1 line for each such material.
A primitive with no material draws with the default material of glTF: the module adds the label `material/default` for it.

### The skins

Each skin of the file is 1 `ideatek:SkinBinds`, with `joint_count` matrices of the model in the order of the ranks.
The matrix of a joint is the inverse bind matrix of the skin, or the identity when the skin gives none. A joint that the skin does not name gets the identity.

### The clips

Each animation of the file is 1 `ideatek:Clip`: `docs/extern/engine_subsystems/ideatek_animation.h.md`, *The bytes of a clip*.

- A channel of glTF is 1 channel of the clip. Its target is the target id of the path of its node.
- The times and the values keep the interpolation of glTF: `STEP`, `LINEAR` or `CUBICSPLINE`.
- `duration` is the last time of each channel. The clip has no event, and `additive` is `false`.

A channel with `KHR_animation_pointer` is skipped, and the log gets 1 line.

### The models

Each scene of the file is 1 `ideatek:Model`: `docs/extern/engine_subsystems/ideatek_scene.h.md`, *The bytes of a model*.

- The nodes of the scene are in the order of a walk from the roots, so a parent comes before its children.
- A node with no name or an empty name takes its index as its name, also in the paths of the target ids.
- A node with a matrix gets the position, the rotation and the scale of that matrix.
- `mesh`, `binds` and the materials of a node name the sub-resources of the same file. The default weights of a node are the weights of the node, or else of its mesh.
- A light of `KHR_lights_punctual` is 1 `ideatek_light_t`. A camera of glTF is 1 `ideatek_projection_t`: `yfov`, or 2 times `ymag` for an orthographic camera, then `znear` and `zfar`.
- The joints of the model are the joints of its skins, in the order of the skins and of their joints. A node that 2 skins name has 1 rank.

The axes of glTF are the axes of the space part: +Y is up, and the front of a model faces +Z. A length is in meters.

### The reloads

A reload of the file builds each resource again from the new bytes. A label that stays keeps its entity, and gets its new component with a higher `revision`.
A label that the new file does not hold gets `IDEATEK_RESOURCE_ERROR_NO_LABEL`, as the resources part tells.

### The bytes

At the first load, the module takes `parse_bytes` and `output_bytes` bytes of its persistent arena:

- cgltf takes its memory from the `parse_bytes`. The module frees them after the file is built.
- The `output_bytes` hold the bytes of the meshes, the models, the skin binds and the clips, and the copies of the buffers.
  Each block comes back when its component leaves, or after a reload is applied, as in the KTX2 loader.

### The settings

The component of its settings is `ideatek_loader_gltf:Settings`.

| Setting | Type | Default | Range | Value |
| --- | --- | --- | --- | --- |
| `parse_bytes` | `uint32_t` | 16777216 | 65536 to 1073741824 | The bytes for the memory of cgltf while 1 file is read. |
| `output_bytes` | `uint32_t` | 67108864 | 65536 to 1073741824 | The bytes for the bytes of all the resources and the copies of the buffers. |
| `max_files` | `uint32_t` | 256 | 1 to 65536 | The most files at a time. |
| `max_parts` | `uint32_t` | 4096 | 1 to 1048576 | The most sub-resources and dependencies of all the files at a time. |

A file above `max_files`, or with more parts than are free, is refused with its line.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `false` | Each call of the module can run on any thread. |
| `reloadable` | `false` | The cases do not reload the module. |
| `spawns` | 4097 | The entity of the loader, and 1 entity for each sub-resource and each dependency, at `max_parts`. |
| `changes` | 262144 | The attaches of the parts of a file, of its labels and of its dependencies. |
| `persistent` | 92274688 | `parse_bytes` and `output_bytes` at their defaults, plus 8388608 for the tables at `max_files` and `max_parts`. |
| `scratch` | 1048576 | The texts of the log, the paths of the dependencies, and the values of the attaches. |
| `log_ring` | 2048 | 64 lines of the log, 32 bytes for each. |
| `spans` | 65536 | The spans of its 6 observers, and the columns of the types that it declares. |

- The deployment has the resources module and the KTX2 loader.
- The module has the grants of a loader module: `docs/extern/kernel/ideatek_resources.h.md`, *The grants*.
  It also writes `ideatek:Model`, `ideatek:Mesh`, `ideatek:Material`, the 6 extensions of a material, `ideatek:SkinBinds` and `ideatek:Clip`, and reads the types of their fields.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `<path> is not a glTF file: <reason>` | cgltf did not read the file: *The reasons*. |
| `<path> is refused: the table of the files is full` | `max_files` files are loaded. |
| `<path> is refused, because the loader has wrong settings` | A setting is outside its range. |
| `<setting>: <value> is outside <low> to <high>` | A value of *The settings* is outside its range. |
| `<path>: no room for the parse` | The arrays of the build do not fit in `parse_bytes`. |
| `<path>: no room for its dependencies` | The dependencies do not fit in `parse_bytes`, in `max_parts` or in `output_bytes`. |
| `<path>: no room for its parts` | The sub-resources do not fit in `max_parts`. |
| `<path>: the output needs <count> bytes, and 1 free part holds at most <room>` | The output of the file does not fit in 1 free part of `output_bytes`. |
| `<path>: the file was not built` | A line before it gives the fault of the build. |
| `<path>: a buffer did not load` | A buffer file failed: *The dependencies*. |
| `<path>: no room for the bytes of a buffer` | The copy of a buffer file does not fit in `max_files` copies or in `output_bytes`. |
| `<path>: a buffer has no good base64 data` | A `data:` URI has no base64 data, or bad base64 data. |
| `<path>: the buffer <index> is shorter than its byteLength` | A buffer has fewer bytes than its `byteLength`. |
| `<path>: the image <index> is not a KTX 2.0 file of its own` | An image is not read: *The dependencies*. |
| `<path>: the image <index> has a second sampler, which has no effect` | 2 textures of 1 image have 2 samplers. |
| `<path>: a primitive of the mesh <index> is compressed, and is skipped` | A primitive uses `KHR_draco_mesh_compression` or `EXT_meshopt_compression`. |
| `<path>: a primitive of the mesh <index> has no POSITION, and is skipped` | A primitive has no positions. |
| `<path>: a primitive of the mesh <index> has another topology, and is skipped` | A primitive has a topology that the mesh does not have. |
| `<path>: the mesh <index> has more than 256 streams, and the rest is skipped` | The streams of a mesh are more than 256. |
| `<path>: a material has an extension with no type` | *The materials*. |
| `<path>: a channel of the clip <index> has no node, and is skipped` | *The clips*. |

### The reasons

| Reason | Fault |
| --- | --- |
| `the data is too short` | The file ends before its JSON or its container ends. |
| `the format is unknown` | The container is not GLB of version 2. |
| `the JSON is not valid` | The JSON of the file is not valid. |
| `the glTF is not valid` | An index, a count or a mode of the file is not valid, as `cgltf_validate` checks. |
| `the glTF is of version 1` | The file is glTF 1.0. |
| `no room for the parse` | The data of the file does not fit in `parse_bytes`. |
| `the options are wrong`, `a file is not found`, `a read failed` | Other results of cgltf, which a parse from bytes does not give. |

### The test

The cases write small glTF files as text, with their buffers in `data:` URIs or in `.bin` files written by the test game.
The test game shows each component of each label.

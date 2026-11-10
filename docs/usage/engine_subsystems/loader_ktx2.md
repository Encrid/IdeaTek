# BM-IdeaTek-Loader-KTX2

A loader of the resources part that reads KTX 2.0 files into `ideatek:Texture`, with no conversion of the texels.

## Install

The release of `BM-IdeaTek-Loader-KTX2` has the tag `loader_ktx2-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BM-IdeaTek-Loader-KTX2.zip` and `sha256.txt`. `BM-IdeaTek-Loader-KTX2.zip` has a folder for each target.
In `BM-IdeaTek-Loader-KTX2`, each folder holds the module `loader_ktx2.dll`, `loader_ktx2.so` or `loader_ktx2.dylib`.
Each folder of `BM-IdeaTek-Loader-KTX2.zip` also holds this readme and the licence.

A deployment puts no library beside the module, which needs only Bridge.
A project names the zip of `BM-IdeaTek-Loader-KTX2` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

The identity of the module is `ideatek_loader_ktx2:loader@0.1.0`.
Its role is a loader module: `docs/extern/kernel/ideatek_resources.h.md`, *The roles*.
At `init`, it spawns 1 entity with `ideatek:ResourceLoader`:

| Field | Value |
| --- | --- |
| `name` | `ktx2` |
| `type` | `ideatek:Texture` |
| `extensions` | `ktx2` |
| `magic` | The 12 bytes of the identifier of KTX 2.0: `AB 4B 54 58 20 32 30 BB 0D 0A 1A 0A`. |
| `ranged` | `false` |

The module reads the whole file in 1 `ideatek:ResourceBytes`. Thus a file above `load_bytes` of the resources module gets `IDEATEK_RESOURCE_ERROR_TOO_BIG`.
It gives no label: 1 file is 1 texture.

### The read of a file

The format is the specification of KTX 2.0: https://registry.khronos.org/KTX/specs/2.0/ktxspec.v2.html.

1. The module reads the header, the index and the level index.
2. It refuses each file that *The refusals* names.
3. It reads the size of a block in the basic block of the data format descriptor.
   That block gives `texelBlockDimension0` to `texelBlockDimension2` and `bytesPlane0`, so the module needs no table of the formats of Vulkan.
4. It computes the bytes of each level from the blocks of its size, the layers and the faces.
   The bytes must be `byteLength` of the level.
5. It copies the levels into its own bytes, from level 0 to the smallest level, with no padding.
   `docs/extern/engine_subsystems/ideatek_rendering.h.md`, *The bytes of a texture*, gives that order.
   In a level, the order of KTX 2.0 stays: each layer, then each face, then each slice of a volume.
6. It attaches `ideatek:Texture` and `ideatek:Sampler` to the resource, then `ideatek:ResourceOutcome` with `IDEATEK_RESOURCE_ERROR_NONE`.

The fields of `ideatek:Texture` come from the header:

| Field | Value |
| --- | --- |
| `format` | `vkFormat`. |
| `width`, `height` | `pixelWidth`, `pixelHeight`. |
| `depth_or_layers` | `pixelDepth` for a volume, `layerCount` for an array, else 1. |
| `mip_count` | `levelCount`, or 1 when `levelCount` is 0. |
| `top_mip` | 0: the module gives each level. |
| `revision` | 1 at the first load, and 1 more at each reload. |
| `dimension` | `IDEATEK_DIMENSION_CUBE` when `faceCount` is 6, `IDEATEK_DIMENSION_3D` when `pixelDepth` is not 0, `IDEATEK_DIMENSION_2D_ARRAY` when `layerCount` is not 0, else `IDEATEK_DIMENSION_2D`. |

A `levelCount` of 0 asks a reader to make the other levels. The module makes none: the texture has 1 level.
The module does not read the key and value data, so `KTXorientation` and `KTXswizzle` have no effect. A file from a tool gives the texels in the order of Vulkan.

### The refusals

The module refuses a file, and attaches `ideatek:ResourceOutcome` with `IDEATEK_RESOURCE_ERROR_LOADER_FAILED`, for each reason of this table.
The first reason of the table that the file has gives the line `<path> is not a KTX 2.0 file: <reason>`.

| Reason | Fault of the file |
| --- | --- |
| `the file is shorter than its header` | The file has less than 80 bytes. |
| `the identifier is not the one of KTX 2.0` | The first 12 bytes are not the identifier. |
| `vkFormat is 0` | A format of Basis Universal needs `vkFormat` 0. |
| `supercompressionScheme is not 0` | The levels are supercompressed. |
| `pixelWidth or pixelHeight is 0` | The rendering part has no 1D texture. |
| `faceCount is not 1 or 6` | The faces are not those of an image or of a cube. |
| `a cube is not square or has a pixelDepth` | A cube has 6 square faces and no depth. |
| `a cube or a volume has a layerCount` | The rendering part has no array of cubes and no array of volumes. |
| `levelCount is above the levels of the largest side` | The file names more levels than its largest side has. |
| `the data format descriptor is outside the file` | The range of the descriptor passes the end of the file. |
| `the data format descriptor has no basic block` | The descriptor has less than 28 bytes, or its first block is not the basic block of Khronos. |
| `bytesPlane0 is 0` | The block has no bytes. |
| `the level index is outside the file` | The entries of the levels pass the end of the file. |
| `a level is outside the file` | The range of a level passes the end of the file. |
| `uncompressedByteLength is not byteLength` | The 2 lengths of a level are different. |
| `a level has not the bytes of its size` | The bytes of a level are not the blocks of its size. |

A refused file or a refused reload keeps the old `ideatek:Texture`, when the resource has one.

### The sampler

The settings of a load, or the table `[settings]` of the meta file, give the sampler: `docs/extern/kernel/ideatek_resources.h.md`, *The meta files*.
Each key is a line `key = value`. A key that is not given takes the default of `ideatek:Sampler`.

| Key | Values | Field of `ideatek:Sampler` |
| --- | --- | --- |
| `min_filter` | `"nearest"` or `"linear"` | `min_filter` |
| `mag_filter` | `"nearest"` or `"linear"` | `mag_filter` |
| `mip_filter` | `"nearest"` or `"linear"` | `mip_filter` |
| `wrap_u`, `wrap_v`, `wrap_w` | `"repeat"`, `"mirrored_repeat"` or `"clamp_to_edge"` | `wrap_u`, `wrap_v`, `wrap_w` |
| `max_anisotropy` | A number from 1 to 16 | `max_anisotropy` |

The palette textures of a look with no filter take `min_filter`, `mag_filter` and `mip_filter` at `"nearest"`.
An unknown key, or a value outside its range, writes 1 line of the log. The key then takes its default, and the load goes on.

### The bytes of the textures

At the first load, the module takes `texture_bytes` bytes of its persistent arena. The texels of each texture are in them, and `bytes` of `ideatek:Texture` names them.
The texels stay good while `ideatek:Texture` stays on the resource. The module frees them when it hears `ideatek:Texture` leave: `docs/extern/kernel/ideatek_resources.h.md`, *The handles*.
A reload takes new bytes for the new texels, then frees the old bytes. So a reload needs the room of both textures for 1 round.

### The settings

The component of its settings is `ideatek_loader_ktx2:Settings`.

| Setting | Type | Default | Range | Value |
| --- | --- | --- | --- | --- |
| `texture_bytes` | `uint32_t` | 33554432 | 4096 to 1073741824 | The bytes for the texels of all the textures. |
| `max_textures` | `uint32_t` | 1024 | 1 to 65536 | The most textures at a time. |

A texture above `max_textures` is refused with its line.

### The deployment

| Key | Default of the manifest | Rule |
| --- | --- | --- |
| `pinned` | `false` | No call of the module needs a given thread. |
| `reloadable` | `false` | No case tests a reload of the module. |
| `spawns` | 1 | The entity of the loader. |
| `changes` | 65536 | The attaches of the textures, the samplers and the outcomes of a round. |
| `persistent` | 33685504 | `texture_bytes` at its default, plus 131072 for the table of the textures at `max_textures`. A larger bound needs a larger `persistent`. |
| `scratch` | 65536 | The texts of the log and the values of the attaches. |
| `log_ring` | 512 | The room of 16 lines of the log, at 32 bytes each. |
| `spans` | 16384 | The spans of its queries, and the columns of its types. |

- The deployment has the resources module: `docs/usage/kernel/resources_files.md`.
- The module takes the grants of a loader module: `docs/extern/kernel/ideatek_resources.h.md`, *The grants*.
  It also writes `ideatek:Texture` and `ideatek:Sampler`, and reads the types of their fields.

### The messages

The module writes these lines:

| Line | Cause |
| --- | --- |
| `<path> is not a KTX 2.0 file: <reason>` | A reason of *The refusals*. |
| `<path> needs <count> bytes, and the most free bytes in 1 part are <room>` | The texels do not fit in 1 free part of `texture_bytes`. |
| `<path> is refused: <max_textures> textures are loaded` | The table of the textures is full. |
| `<path> is refused: the settings of the module are wrong` | A setting is outside its range, so the module refuses each load. |
| `<setting>: <value> is outside <low> to <high>` | A setting of *The settings* is outside its range. |
| `<path>: the key <key> of the settings is not known` | A key of *The sampler* is not known. It has no effect. |
| `<path>: the value of <key> is not allowed` | A value of *The sampler* is outside its range. The key takes its default. |

A line of a fault of the settings has at most 127 bytes. The module writes the lines of the first 8 faults of the settings of 1 load.

### The test

The test game writes small KTX 2.0 files into the content folder with its verb `ktx2`, because the file of a case holds only text.
The verb takes the fields of the header and the size of a block.
It also takes the faults of a file: a shorter file, another identifier, another descriptor, or another range of level 0.
The test game shows `ideatek:Texture` and `ideatek:Sampler` when they come. Each refusal of *The refusals* has a case, with a file that has the fault.

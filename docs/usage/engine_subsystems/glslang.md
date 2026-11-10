# BL-IdeaTek-Glslang

glslang 16.6.0 as a static library that compiles GLSL for Vulkan into SPIR-V while a game runs.

## Install

The release of `BL-IdeaTek-Glslang` has the tag `glslang-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BL-IdeaTek-Glslang.zip` and `sha256.txt`. `BL-IdeaTek-Glslang.zip` has a folder for each target.
For each target, the folder holds the static archive `glslang.a`, the headers of the C interface of glslang, and the notice:

| Folder | Content |
| --- | --- |
| `glslang.a` | The 43 sources of glslang 16.6.0: the front end of GLSL, the preprocessor, the generator of SPIR-V, the C interface and the default limits. |
| `include/glslang/Include` | `glslang_c_interface.h`, `glslang_c_shader_types.h` and `visibility.h`. |
| `include/glslang/Public` | `resource_limits_c.h`. |
| `include/glslang` | `build_info.h`, the version of glslang. |

A project names the zip of `BL-IdeaTek-Glslang` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `glslang/Include/glslang_c_interface.h` and `glslang/Public/resource_limits_c.h`, and links `glslang.a` with the flag `-lc++`, which brings the C++ runtime of zig.
The C interface of glslang is the API: https://github.com/KhronosGroup/glslang/blob/16.6.0/glslang/Include/glslang_c_interface.h.

1. `glslang_initialize_process` starts glslang, once for each process. `glslang_finalize_process` stops it.
2. `glslang_shader_create` takes 1 stage of GLSL with the client `GLSLANG_CLIENT_VULKAN` and the target `GLSLANG_TARGET_SPV`.
3. `glslang_shader_set_preamble` adds text before the source. The rendering module puts each setting of a shader there as a `#define`.
4. `glslang_shader_preprocess` and `glslang_shader_parse` read the source. On a fault, `glslang_shader_get_info_log` gives the text of the fault.
5. `glslang_program_link` and `glslang_program_SPIRV_generate` make the SPIR-V. `glslang_program_SPIRV_get_ptr` and `glslang_program_SPIRV_get_size` give its words.

`glslang_default_resource` gives the default limits of glslang. A module that knows the limits of its GPU can give its own.

`BL-IdeaTek-Glslang` names no other part in `uses`.

### The build

zig builds glslang from its release archive, with no CMake, no Python and no package of the system:

- The front end of GLSL, the generator of SPIR-V and the C interfaces. The build defines `ENABLE_SPIRV`, and `GLSLANG_OSINCLUDE_WIN32` on Windows or `GLSLANG_OSINCLUDE_UNIX` on Linux and macOS.
- No front end of HLSL: glslang deprecates it, and the shaders of IdeaTek are GLSL.
- No optimizer of SPIR-V: SPIRV-Tools is a second library of about 400 sources. The driver of the GPU optimizes the SPIR-V that it gets.
- No program of glslang: the standalone `glslang` of the command line is not in the archive.

`build_info.h` comes from `build_info.h.tmpl` of the release with the version 16.6.0, and is kept in `third_party/glslang/generated/glslang`.

### The licence

The notice `third_party/glslang/license` is `LICENSE.txt` of glslang 16.6.0. The sources of the archive are under the BSD licence with 3 clauses, the BSD licence with 2 clauses, and the MIT licence.
`hex_float.h` and `bitutils.h`, which `SpvBuilder.cpp` includes, are under the Apache licence 2.0.
`glslang_tab.cpp` is under the GPL 3 with the special exception of Bison, which lets a project of any licence use it.

### The test

The program `glslang_smoke` of the tests compiles a fragment shader of GLSL in 1 of 3 modes:

| Mode | Shader | Output |
| --- | --- | --- |
| `right` | A shader that reads the define `PALETTE_GAIN` of its preamble. | The count of words, the magic number `07230203` and the version `00010600` of SPIR-V 1.6. |
| `fault` | A shader that reads an unknown name. | The line of the fault, and the status 1. |
| `bare` | The shader of `right` without its preamble. | The line of the fault for `PALETTE_GAIN`, and the status 1. |

Another mode gives the 3 modes, and the status 2.

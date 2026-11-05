# IdeaTek

IdeaTek holds contracts for game engines on Bridge, and the parts that implement them. Each concern has a contract.

## Install

Python 3.11 or a later version.

The release of a part is a zip with a folder for each target. A deployment puts the files of a module and the shared library of `BL-IdeaTek-SDL3` in the same folder.

## Usage

| Command | Result |
| --- | --- |
| `python project.py fetch` | Downloads what the build needs. |
| `python project.py build` | Builds the project. |
| `python project.py pack` | Packs the build for a release. |
| `python project.py test` | Runs the tests. |
| `python project.py test --break` | Runs the tests with their claims made false. Each test must fail. |
| `python project.py test --branches` | Runs the tests on a build with probes. Each branch of the code must run. |
| `python project.py test --leaks` | Runs the tests on a build with counts. Each test must give back what the code takes. |
| `python project.py check` | Checks the project. |
| `python project.py clean` | Removes what the build made. Keeps what `fetch` downloaded. |
| `python project.py distclean` | Removes the folder `build`, also what `fetch` downloaded. |

### The contracts

Each part has a header in `include` and a document in `docs/extern`, which gives its types.

- Each module in C that writes or reads a type of a part includes the header of the part.
- Each macro of the text gives the braces of its type and the text between them.
- A module in another language does not include a header. It states the descriptor of each type with the bytes of the document.
- Each module that uses a type declares it in its `init`. A module in C gives the C type and the descriptor of the header.
- A module declares a type after the types of its fields.
- Bridge compares the descriptors byte for byte. It also refuses a second module that states another kind, storage layout or `on_destroy`.
- Bridge does not compare `on_unload`, so each module states its own.
- No module has the namespace `ideatek`. Thus each module needs a grant for each type that it declares, in `read` or `write` of the deployment.
- Two modules that conflict in one phase need a pair of the deployment.

The documents of `bridge.h` and `bridge_abi.h` of Bridge tell the pairs and what each grant allows.

### The implementations

Each implementation is a part with its own release. A module part fills the role of a module that a contract document names.

| Part | Package | Identity | Fills the role of | Beside it |
| --- | --- | --- | --- | --- |
| `sdl3` | `BL-IdeaTek-SDL3` | A library: no identity | | |
| `window_sdl3` | `BM-IdeaTek-Window-SDL3` | `ideatek_window_sdl3:window@0.1.0` | The window module, with the clipboard | The SDL3 library |
| `input_sdl3` | `BM-IdeaTek-Input-SDL3` | `ideatek_input_sdl3:input@0.1.0` | The input module | The SDL3 library |
| `gpu_vulkan_module` | `BM-IdeaTek-Gpu-Vulkan` | `ideatek_gpu_vulkan:gpu@0.1.0` | The GPU module | The Vulkan loader. On macOS, the bundled loader with KosmicKrisp and MoltenVK |

- The behavior and the settings of each role are in the document of its contract, under the name of the role.
- The namespace of each identity is the key of its part, so no module has the namespace `ideatek`.
- `sdl3` holds the shared library of SDL3 3.4.20. A deployment puts 1 copy of it beside its modules, so each module in the process uses the same SDL3.
- For each target, the zip of `sdl3` holds the library, its headers in `include/SDL3`, the notices, and `leaks.toml` for `[leaks] with` of a module.
- The library is `SDL3.dll`, `SDL3.so` with that soname, or `SDL3.dylib` with the install name `@rpath/SDL3.dylib`. A module links it with no import library.
- The library keeps GL, Vulkan and the GPU API. It loads the GL or Vulkan library of the system only for a window or a call that asks for it.
- On macOS, a window that names no graphics API and no external context asks for GL.
- `SDL_Metal_CreateView` gives a view with a CAMetalLayer on macOS, and no view on the other systems.
- The GPU module needs the base of `docs/extern/platform_abstraction/ideatek_gpu_vulkan.h.md`, *The features*.
- On macOS, KosmicKrisp needs Apple silicon and macOS 26. MoltenVK serves an Intel Mac and an older macOS.
- A GPU that lacks a part of that base gets `IDEATEK_GPU_STATUS_NONE`, and the log gets 1 line for each missing part.
- A deployment grants each module the `write` and the `read` of the types of its role.
- The pairs that a role needs are in the document of its contract.

## Contributing

| Document | Content |
| --- | --- |
| `docs/contributing/needs.md` | The needs that each decision meets. |
| `docs/contributing/decisions.md` | The steps of a decision. |
| `docs/contributing/layers.md` | The layers of an engine, and the layers of the project. |
| `docs/contributing/coding_style.md` | How to write the code. |
| `docs/contributing/documentation_style.md` | How to write the documents. |
| `docs/contributing/tests.md` | How to write the tests. |
| `docs/contributing/commits.md` | How to write a commit. |
| `docs/contributing/pull_request.md` | How to write a pull request. |
| `docs/contributing/agreement.md` | The agreement `cla.md`, which each contributor signs. |

## License

The GNU Affero General Public License, version 3 only. The file `license` holds the text.
Each source of third-party code has its notice in `third_party/<source>/license`.
SDL3 is under the zlib licence: `third_party/sdl3/license`. Its Linux build holds code of SDL_linux_deps: `third_party/sdl3_linux/license`.

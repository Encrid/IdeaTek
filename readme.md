# IdeaTek

IdeaTek holds contracts for game engines on Bridge, and the parts that implement them. Each concern has a contract.

## Install

Python 3.11 or a later version.

The release of a part is a zip with a folder for each target. The readme of each part tells what a deployment puts beside it.

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
- The document of a contract gives the range of each setting of a role. A setting outside its range is a fault of the deployment.
- Then the module writes `<setting>: <value> is outside <range>`, and its system fails in each round.
- Bridge then stops the module or the world, as `on_error` and `max_system_failures` of the deployment tell.

The documents of `bridge.h` and `bridge_abi.h` of Bridge tell the pairs and what each grant allows.

### The parts

Each part has a readme, which its release holds.

| Part | Package | Readme |
| --- | --- | --- |
| `animation` | `BC-IdeaTek-Animation` | `docs/usage/engine_subsystems/animation.md` |
| `changes` | `BC-IdeaTek-Changes` | `docs/usage/engine_subsystems/changes.md` |
| `debug_draw` | `BC-IdeaTek-DebugDraw` | `docs/usage/engine_subsystems/debug_draw.md` |
| `effects` | `BC-IdeaTek-Effects` | `docs/usage/engine_subsystems/effects.md` |
| `input_mapping` | `BC-IdeaTek-InputMapping` | `docs/usage/engine_subsystems/input_mapping.md` |
| `localization` | `BC-IdeaTek-Localization` | `docs/usage/engine_subsystems/localization.md` |
| `navigation` | `BC-IdeaTek-Navigation` | `docs/usage/engine_subsystems/navigation.md` |
| `networking` | `BC-IdeaTek-Networking` | `docs/usage/engine_subsystems/networking.md` |
| `persistence` | `BC-IdeaTek-Persistence` | `docs/usage/engine_subsystems/persistence.md` |
| `physics` | `BC-IdeaTek-Physics` | `docs/usage/engine_subsystems/physics.md` |
| `rendering` | `BC-IdeaTek-Rendering` | `docs/usage/engine_subsystems/rendering.md` |
| `services` | `BC-IdeaTek-Services` | `docs/usage/engine_subsystems/services.md` |
| `sound` | `BC-IdeaTek-Sound` | `docs/usage/engine_subsystems/sound.md` |
| `space` | `BC-IdeaTek-Space` | `docs/usage/engine_subsystems/space.md` |
| `terrain` | `BC-IdeaTek-Terrain` | `docs/usage/engine_subsystems/terrain.md` |
| `ui` | `BC-IdeaTek-Ui` | `docs/usage/engine_subsystems/ui.md` |
| `frame` | `BC-IdeaTek-Frame` | `docs/usage/kernel/frame.md` |
| `resources` | `BC-IdeaTek-Resources` | `docs/usage/kernel/resources.md` |
| `states` | `BC-IdeaTek-States` | `docs/usage/kernel/states.md` |
| `audio` | `BC-IdeaTek-Audio` | `docs/usage/platform_abstraction/audio.md` |
| `files` | `BC-IdeaTek-Files` | `docs/usage/platform_abstraction/files.md` |
| `gpu` | `BC-IdeaTek-Gpu` | `docs/usage/platform_abstraction/gpu.md` |
| `input` | `BC-IdeaTek-Input` | `docs/usage/platform_abstraction/input.md` |
| `sockets` | `BC-IdeaTek-Sockets` | `docs/usage/platform_abstraction/sockets.md` |
| `window` | `BC-IdeaTek-Window` | `docs/usage/platform_abstraction/window.md` |
| `world` | `BC-IdeaTek-World` | `docs/usage/platform_abstraction/world.md` |
| `sdl3` | `BL-IdeaTek-SDL3` | `docs/usage/platform_abstraction/sdl3.md` |
| `sdl3_start` | `BL-IdeaTek-SDL3-Start` | `docs/usage/platform_abstraction/sdl3_start.md` |
| `vulkan` | `BL-IdeaTek-Vulkan` | `docs/usage/platform_abstraction/vulkan.md` |
| `gns` | `BL-IdeaTek-GameNetworkingSockets` | `docs/usage/platform_abstraction/gns.md` |
| `glslang` | `BL-IdeaTek-Glslang` | `docs/usage/engine_subsystems/glslang.md` |
| `window_sdl3` | `BM-IdeaTek-Window-SDL3` | `docs/usage/platform_abstraction/window_sdl3.md` |
| `input_sdl3` | `BM-IdeaTek-Input-SDL3` | `docs/usage/platform_abstraction/input_sdl3.md` |
| `gpu_vulkan_module` | `BM-IdeaTek-Gpu-Vulkan` | `docs/usage/platform_abstraction/gpu_vulkan_module.md` |
| `frame_sdl3` | `BM-IdeaTek-Frame-SDL3` | `docs/usage/kernel/frame_sdl3.md` |
| `files_mosl` | `BM-IdeaTek-Files-Mosl` | `docs/usage/platform_abstraction/files_mosl.md` |
| `audio_sdl3` | `BM-IdeaTek-Audio-SDL3` | `docs/usage/platform_abstraction/audio_sdl3.md` |
| `sockets_gns` | `BM-IdeaTek-Sockets-GNS` | `docs/usage/platform_abstraction/sockets_gns.md` |
| `states_bridge` | `BM-IdeaTek-States-Bridge` | `docs/usage/kernel/states_bridge.md` |
| `resources_files` | `BM-IdeaTek-Resources-Files` | `docs/usage/kernel/resources_files.md` |
| `loader_ktx2` | `BM-IdeaTek-Loader-KTX2` | `docs/usage/engine_subsystems/loader_ktx2.md` |
| `space_bridge` | `BM-IdeaTek-Space-Bridge` | `docs/usage/engine_subsystems/space_bridge.md` |

### The implementations

Each implementation is a part with its own release. A module part fills the role of a module that a contract document names.

| Part | Package | Identity | Fills the role of | Beside it |
| --- | --- | --- | --- | --- |
| `sdl3` | `BL-IdeaTek-SDL3` | A library: no identity | | |
| `sdl3_start` | `BL-IdeaTek-SDL3-Start` | A library: no identity | | |
| `vulkan` | `BL-IdeaTek-Vulkan` | A library: no identity | | |
| `gns` | `BL-IdeaTek-GameNetworkingSockets` | A library: no identity | | |
| `glslang` | `BL-IdeaTek-Glslang` | A library: no identity | | |
| `window_sdl3` | `BM-IdeaTek-Window-SDL3` | `ideatek_window_sdl3:window@0.1.0` | The window module, with the clipboard | The SDL3 library |
| `input_sdl3` | `BM-IdeaTek-Input-SDL3` | `ideatek_input_sdl3:input@0.1.0` | The input module | The SDL3 library |
| `gpu_vulkan_module` | `BM-IdeaTek-Gpu-Vulkan` | `ideatek_gpu_vulkan:gpu@0.1.0` | The GPU module | The Vulkan loader. On macOS, the bundled loader with KosmicKrisp and MoltenVK |
| `frame_sdl3` | `BM-IdeaTek-Frame-SDL3` | `ideatek_frame_sdl3:clock@0.1.0` | The clock module | The SDL3 library |
| `files_mosl` | `BM-IdeaTek-Files-Mosl` | `ideatek_files_mosl:files@0.1.0` | The files module | Nothing: mosl is inside it |
| `audio_sdl3` | `BM-IdeaTek-Audio-SDL3` | `ideatek_audio_sdl3:audio@0.1.0` | The audio module | The SDL3 library of IdeaTek |
| `sockets_gns` | `BM-IdeaTek-Sockets-GNS` | `ideatek_sockets_gns:sockets@0.1.0` | The sockets module | Nothing: GameNetworkingSockets is inside it |
| `states_bridge` | `BM-IdeaTek-States-Bridge` | `ideatek_states_bridge:states@0.1.0` | The states module | Nothing: it stands on Bridge alone |
| `resources_files` | `BM-IdeaTek-Resources-Files` | `ideatek_resources_files:resources@0.1.0` | The resources module | The files module of the deployment |
| `loader_ktx2` | `BM-IdeaTek-Loader-KTX2` | `ideatek_loader_ktx2:loader@0.1.0` | A loader module of the resources part | The resources module of the deployment |
| `space_bridge` | `BM-IdeaTek-Space-Bridge` | `ideatek_space_bridge:space@0.1.0` | The space module | Nothing: it stands on Bridge alone |

- The behavior and the settings of each role are in the document of its contract, under the name of the role.
- The namespace of each identity is the key of its part, so no module has the namespace `ideatek`.
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

volk is under the MIT licence: `third_party/volk/license`. The headers of Vulkan are under Apache-2.0 or MIT: `third_party/vulkan_headers/license`.
mosl is under the GNU Affero General Public License, version 3 only: `third_party/mosl/license`.

GameNetworkingSockets is under the BSD 3-Clause licence: `third_party/gns/license`. protobuf is under the BSD 3-Clause licence: `third_party/protobuf/license`. libsodium is under the ISC licence: `third_party/libsodium/license`.
glslang is under the BSD 3-Clause, BSD 2-Clause, MIT and Apache-2.0 licences, and the GPL-3.0 with the exception of Bison: `third_party/glslang/license`.

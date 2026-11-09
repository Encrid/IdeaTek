# BL-IdeaTek-GameNetworkingSockets

GameNetworkingSockets 1.6.1 as static libraries for the sockets module of IdeaTek, with protobuf and libsodium.

## Install

The release of `BL-IdeaTek-GameNetworkingSockets` has the tag `gns-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BL-IdeaTek-GameNetworkingSockets.zip` and `sha256.txt`. The zip has a folder for each target.
For each target, the folder holds 4 static archives, the headers of GameNetworkingSockets in `include/steam`, and the 3 notices:

| Archive | Content |
| --- | --- |
| `gns.a` | The C++ sources of GameNetworkingSockets 1.6.1, with the 3 generated protobuf messages. |
| `gns_c.a` | The 3 C sources of GameNetworkingSockets: the IPv6 text and the SHA-1 of wpa. |
| `protobuf.a` | The runtime of protobuf 3.21.12. |
| `sodium.a` | libsodium 1.0.20, the crypto of the connections. |

A project names the zip in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module links the 4 archives with `zig c++`, which brings the C++ runtime of zig. On Windows, it also links `ws2_32`, `crypt32`, `winmm` and `iphlpapi`.
The module defines `STEAMNETWORKINGSOCKETS_STATIC_LINK` before it includes a header of `include/steam`.
The sockets module of IdeaTek is the user of the part: `docs/extern/platform_abstraction/ideatek_sockets.h.md`.

### The build

zig builds the 3 sources from their release archives, with no CMake and no package of the system:

- GameNetworkingSockets 1.6.1: the 36 sources of its client library, with the ICE client for P2P, without Google WebRTC and without the parts that need Steam.
- protobuf 3.21.12: the full runtime, because the messages of GameNetworkingSockets ask for `optimize_for = SPEED`. It is the last release of protobuf before its dependency on abseil.
- libsodium 1.0.20: the backend of the crypto. On x86_64 Linux and macOS, the build holds its assembly sources.

The 3 `.proto` files of GameNetworkingSockets are generated once with `protoc` 21.12, and kept in `third_party/gns/generated`. The runtime and the generated code must come from 1 release of protobuf.
The patch `third_party/libsodium/xgetbv.patch` lets libsodium read the AVX state of the processor on every system with the inline assembly of GCC and clang.
Without it, the Windows build of libsodium finds no AVX, its AES-GCM is not available, and GameNetworkingSockets refuses each connection.

### The processor

GameNetworkingSockets with libsodium needs the AES instructions of the processor: AES-NI with AVX on x86_64, the crypto extension on aarch64.
A processor without them refuses each connection with the line of GameNetworkingSockets `No hardware AES support on this CPU.`.

### The test

The program `gns_smoke` of the tests connects 2 peers of 1 process on the local host, and carries 1 reliable message.

# BC-IdeaTek-Sockets

The contract of sockets: identity, connections over IP and P2P, lanes, sessions and messages.

## Install

The release of `BC-IdeaTek-Sockets` has the tag `sockets-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Sockets.zip` and `sha256.txt`. `BC-IdeaTek-Sockets.zip` has a folder for each target.
In `BC-IdeaTek-Sockets`, each folder holds `ideatek_sockets.h` in `include`.
Each folder of `BC-IdeaTek-Sockets.zip` also holds this readme and the licence.

`BC-IdeaTek-Sockets` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Sockets` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_sockets.h`. `docs/extern/platform_abstraction/ideatek_sockets.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Sockets` are `BC-IdeaTek-World`. Their readmes give their headers.

# BC-IdeaTek-Networking

The contract of game networking: sessions, peers, replication, relevance, commands and the net clock.

## Install

The release of `BC-IdeaTek-Networking` has the tag `networking-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Networking.zip` and `sha256.txt`. `BC-IdeaTek-Networking.zip` has a folder for each target.
In `BC-IdeaTek-Networking`, each folder holds `ideatek_networking.h` in `include`.
Each folder of `BC-IdeaTek-Networking.zip` also holds this readme and the licence.

`BC-IdeaTek-Networking` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Networking` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_networking.h`. `docs/extern/engine_subsystems/ideatek_networking.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Networking` are `BC-IdeaTek-World`, `BC-IdeaTek-Sockets`, `BC-IdeaTek-Frame` and `BC-IdeaTek-Changes`. Their readmes give their headers.

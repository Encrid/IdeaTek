# BC-IdeaTek-Persistence

The contract of saves on Bridge: requests, the answers of modules, slots, versions and user settings.

## Install

The release of `BC-IdeaTek-Persistence` has the tag `persistence-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Persistence.zip` and `sha256.txt`. `BC-IdeaTek-Persistence.zip` has a folder for each target.
In `BC-IdeaTek-Persistence`, each folder holds `ideatek_persistence.h` in `include`.
Each folder of `BC-IdeaTek-Persistence.zip` also holds this readme and the licence.

`BC-IdeaTek-Persistence` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Persistence` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_persistence.h`. `docs/extern/engine_subsystems/ideatek_persistence.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Persistence` are `BC-IdeaTek-World`, `BC-IdeaTek-Files` and `BC-IdeaTek-Changes`. Their readmes give their headers.

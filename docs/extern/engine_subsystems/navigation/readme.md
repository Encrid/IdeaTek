# BC-IdeaTek-Navigation

The contract of navigation on Bridge: nav meshes and their links, path queries, agents and obstacles.

## Install

The release of `BC-IdeaTek-Navigation` has the tag `navigation-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Navigation.zip` and `sha256.txt`. `BC-IdeaTek-Navigation.zip` has a folder for each target.
In `BC-IdeaTek-Navigation`, each folder holds `ideatek_navigation.h` in `include`.
Each folder of `BC-IdeaTek-Navigation.zip` also holds this readme and the licence.

`BC-IdeaTek-Navigation` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Navigation` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_navigation.h`. `docs/extern/engine_subsystems/ideatek_navigation.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Navigation` are `BC-IdeaTek-World`, `BC-IdeaTek-Space` and `BC-IdeaTek-Physics`. Their readmes give their headers.

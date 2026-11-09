# BC-IdeaTek-Space

The contract of the transform of an entity: its place, its rotation, its scale and its parent.

## Install

The release of `BC-IdeaTek-Space` has the tag `space-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Space.zip` and `sha256.txt`. `BC-IdeaTek-Space.zip` has a folder for each target.
In `BC-IdeaTek-Space`, each folder holds `ideatek_space.h` in `include`.
Each folder of `BC-IdeaTek-Space.zip` also holds this readme and the licence.

`BC-IdeaTek-Space` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
It is `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Space` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_space.h`. `docs/extern/engine_subsystems/ideatek_space.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Space` are `BC-IdeaTek-World`. Their readmes give their headers.

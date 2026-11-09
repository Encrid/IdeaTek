# BC-IdeaTek-Terrain

The contract of terrain: voxels, heightmaps, materials, edits, viewers and scatter.

## Install

The release of `BC-IdeaTek-Terrain` has the tag `terrain-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Terrain.zip` and `sha256.txt`. `BC-IdeaTek-Terrain.zip` has a folder for each target.
In `BC-IdeaTek-Terrain`, each folder holds `ideatek_terrain.h` in `include`.
Each folder of `BC-IdeaTek-Terrain.zip` also holds this readme and the licence.

`BC-IdeaTek-Terrain` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Terrain` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_terrain.h`. `docs/extern/engine_subsystems/ideatek_terrain.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Terrain` are `BC-IdeaTek-World`, `BC-IdeaTek-Space`, `BC-IdeaTek-Rendering` and `BC-IdeaTek-Physics`. Their readmes give their headers.

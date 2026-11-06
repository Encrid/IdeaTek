# BC-IdeaTek-Rendering

The contract of rendering on Bridge: cameras, meshes and their materials, lights, decals, fog and the sky.

## Install

The release of `BC-IdeaTek-Rendering` has the tag `rendering-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Rendering.zip` and `sha256.txt`. `BC-IdeaTek-Rendering.zip` has a folder for each target.
In `BC-IdeaTek-Rendering`, each folder holds `ideatek_rendering.h` in `include`.
Each folder of `BC-IdeaTek-Rendering.zip` also holds this readme and the licence.

`BC-IdeaTek-Rendering` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Rendering` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_rendering.h`. `docs/extern/engine_subsystems/ideatek_rendering.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Rendering` are `BC-IdeaTek-World` and `BC-IdeaTek-Space`. Their readmes give their headers.

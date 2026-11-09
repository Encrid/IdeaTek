# BC-IdeaTek-Physics

The contract of physics: bodies, shapes, joints, vehicles, destruction, soft bodies, fluids and fields.

## Install

The release of `BC-IdeaTek-Physics` has the tag `physics-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Physics.zip` and `sha256.txt`. `BC-IdeaTek-Physics.zip` has a folder for each target.
In `BC-IdeaTek-Physics`, each folder holds `ideatek_physics.h` in `include`.
Each folder of `BC-IdeaTek-Physics.zip` also holds this readme and the licence.

`BC-IdeaTek-Physics` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Physics` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_physics.h`. `docs/extern/engine_subsystems/ideatek_physics.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Physics` are `BC-IdeaTek-World`, `BC-IdeaTek-Space`, `BC-IdeaTek-Frame` and `BC-IdeaTek-Rendering`. Their readmes give their headers.

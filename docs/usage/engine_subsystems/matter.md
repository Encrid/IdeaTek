# BC-IdeaTek-Matter

The contract of the matter of bodies: families, materials, and the state of each chunk under the laws.

## Install

The release of `BC-IdeaTek-Matter` has the tag `matter-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Matter.zip` and `sha256.txt`. `BC-IdeaTek-Matter.zip` has a folder for each target.
In `BC-IdeaTek-Matter`, each folder holds `ideatek_matter.h` in `include`.
Each folder of `BC-IdeaTek-Matter.zip` also holds this readme and the licence.

`BC-IdeaTek-Matter` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
It is `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Matter` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_matter.h`. `docs/extern/engine_subsystems/ideatek_matter.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Matter` are `BC-IdeaTek-World`, `BC-IdeaTek-Space`, `BC-IdeaTek-Frame`, `BC-IdeaTek-Rendering` and `BC-IdeaTek-Physics`. Their readmes give their headers.

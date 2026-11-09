# BC-IdeaTek-Effects

The contract of visual effects: curves, particle emitters, their looks, collisions and sub-emitters.

## Install

The release of `BC-IdeaTek-Effects` has the tag `effects-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Effects.zip` and `sha256.txt`. `BC-IdeaTek-Effects.zip` has a folder for each target.
In `BC-IdeaTek-Effects`, each folder holds `ideatek_effects.h` in `include`.
Each folder of `BC-IdeaTek-Effects.zip` also holds this readme and the licence.

`BC-IdeaTek-Effects` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Effects` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_effects.h`. `docs/extern/engine_subsystems/ideatek_effects.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Effects` are `BC-IdeaTek-World`, `BC-IdeaTek-Space`, `BC-IdeaTek-Audio`, `BC-IdeaTek-Rendering`, `BC-IdeaTek-Physics` and `BC-IdeaTek-Sound`. Their readmes give their headers.

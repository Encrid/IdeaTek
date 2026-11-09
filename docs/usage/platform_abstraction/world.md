# BC-IdeaTek-World

The contract of the entity id: the type that each other contract uses to name an entity.

## Install

The release of `BC-IdeaTek-World` has the tag `world-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-World.zip` and `sha256.txt`. `BC-IdeaTek-World.zip` has a folder for each target.
In `BC-IdeaTek-World`, each folder holds `ideatek_world.h` in `include`.
Each folder of `BC-IdeaTek-World.zip` also holds this readme and the licence.

`BC-IdeaTek-World` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
It is `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-World` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_world.h`. `docs/extern/platform_abstraction/ideatek_world.h.md` gives its types and its rules.

`BC-IdeaTek-World` names no other part in `uses`.

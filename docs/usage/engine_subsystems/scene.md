# BC-IdeaTek-Scene

The contract of a model file in the world: its tree of nodes, and the entities that each copy spawns.

## Install

The release of `BC-IdeaTek-Scene` has the tag `scene-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Scene.zip` and `sha256.txt`. `BC-IdeaTek-Scene.zip` has a folder for each target.
In `BC-IdeaTek-Scene`, each folder holds `ideatek_scene.h` in `include`.
Each folder of `BC-IdeaTek-Scene.zip` also holds this readme and the licence.

`BC-IdeaTek-Scene` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
It is `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Scene` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_scene.h`. `docs/extern/engine_subsystems/ideatek_scene.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Scene` are `BC-IdeaTek-World`, `BC-IdeaTek-Space` and `BC-IdeaTek-Rendering`. Their readmes give their headers.
The node entities also get types of `BC-IdeaTek-Animation`, so the scene module needs that readme too.

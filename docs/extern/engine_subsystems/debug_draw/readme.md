# BC-IdeaTek-DebugDraw

The contract of debug drawings: lines, shapes and labels in the world and on the screen.

## Install

The release of `BC-IdeaTek-DebugDraw` has the tag `debug_draw-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-DebugDraw.zip` and `sha256.txt`. `BC-IdeaTek-DebugDraw.zip` has a folder for each target.
In `BC-IdeaTek-DebugDraw`, each folder holds `ideatek_debug_draw.h` in `include`.
Each folder of `BC-IdeaTek-DebugDraw.zip` also holds this readme and the licence.

`BC-IdeaTek-DebugDraw` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-DebugDraw` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_debug_draw.h`. `docs/extern/engine_subsystems/ideatek_debug_draw.h.md` gives its types and its rules.

`BC-IdeaTek-DebugDraw` names no other part in `uses`.

# BC-IdeaTek-Sound

The contract of sound: players, spatial sound, buses, effects, zones, voices, streams and music timing.

## Install

The release of `BC-IdeaTek-Sound` has the tag `sound-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Sound.zip` and `sha256.txt`. `BC-IdeaTek-Sound.zip` has a folder for each target.
In `BC-IdeaTek-Sound`, each folder holds `ideatek_sound.h` in `include`.
Each folder of `BC-IdeaTek-Sound.zip` also holds this readme and the licence.

`BC-IdeaTek-Sound` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Sound` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_sound.h`. `docs/extern/engine_subsystems/ideatek_sound.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Sound` are `BC-IdeaTek-World`, `BC-IdeaTek-Space`, `BC-IdeaTek-Audio` and `BC-IdeaTek-Physics`. Their readmes give their headers.

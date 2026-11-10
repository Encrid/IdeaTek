# BC-IdeaTek-Animation

The contract of animation on Bridge: skeletons, clips, players, blend spaces, state machines and root motion.

## Install

The release of `BC-IdeaTek-Animation` has the tag `animation-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Animation.zip` and `sha256.txt`. `BC-IdeaTek-Animation.zip` has a folder for each target.
In `BC-IdeaTek-Animation`, each folder holds `ideatek_animation.h` in `include`.
Each folder of `BC-IdeaTek-Animation.zip` also holds this readme and the licence.

`BC-IdeaTek-Animation` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Animation` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_animation.h`. `docs/extern/engine_subsystems/ideatek_animation.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Animation` are `BC-IdeaTek-World`, `BC-IdeaTek-Space`, `BC-IdeaTek-Frame`, `BC-IdeaTek-States` and `BC-IdeaTek-Physics`. Their readmes give their headers.

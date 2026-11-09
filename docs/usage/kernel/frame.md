# BC-IdeaTek-Frame

The contract of the time of a round: the frame, the fixed steps and the requests of time.

## Install

The release of `BC-IdeaTek-Frame` has the tag `frame-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Frame.zip` and `sha256.txt`. `BC-IdeaTek-Frame.zip` has a folder for each target.
In `BC-IdeaTek-Frame`, each folder holds `ideatek_frame.h` in `include`.
Each folder of `BC-IdeaTek-Frame.zip` also holds this readme and the licence.

`BC-IdeaTek-Frame` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
It is `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Frame` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_frame.h`. `docs/extern/kernel/ideatek_frame.h.md` gives its types and its rules.

`BC-IdeaTek-Frame` names no other part in `uses`.

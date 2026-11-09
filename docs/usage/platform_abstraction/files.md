# BC-IdeaTek-Files

The contract of files: the requests of the game, their results and the watch of a folder.

## Install

The release of `BC-IdeaTek-Files` has the tag `files-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Files.zip` and `sha256.txt`. `BC-IdeaTek-Files.zip` has a folder for each target.
In `BC-IdeaTek-Files`, each folder holds `ideatek_files.h` in `include`.
Each folder of `BC-IdeaTek-Files.zip` also holds this readme and the licence.

`BC-IdeaTek-Files` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Files` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_files.h`. `docs/extern/platform_abstraction/ideatek_files.h.md` gives its types and its rules.

`BC-IdeaTek-Files` names no other part in `uses`.

# BC-IdeaTek-Changes

The contract of the changes of rows in a stream: ids, durability, the store and the recordings.

## Install

The release of `BC-IdeaTek-Changes` has the tag `changes-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Changes.zip` and `sha256.txt`. `BC-IdeaTek-Changes.zip` has a folder for each target.
In `BC-IdeaTek-Changes`, each folder holds `ideatek_changes.h` in `include`.
Each folder of `BC-IdeaTek-Changes.zip` also holds this readme and the licence.

`BC-IdeaTek-Changes` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Changes` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_changes.h`. `docs/extern/engine_subsystems/ideatek_changes.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Changes` are `BC-IdeaTek-World`. Their readmes give their headers.

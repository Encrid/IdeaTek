# BC-IdeaTek-States

The contract of the states of the application: state machines, their requests and their changes.

## Install

The release of `BC-IdeaTek-States` has the tag `states-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-States.zip` and `sha256.txt`. `BC-IdeaTek-States.zip` has a folder for each target.
In `BC-IdeaTek-States`, each folder holds `ideatek_states.h` in `include`.
Each folder of `BC-IdeaTek-States.zip` also holds this readme and the licence.

`BC-IdeaTek-States` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
It is `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-States` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_states.h`. `docs/extern/kernel/ideatek_states.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-States` are `BC-IdeaTek-World`. Their readmes give their headers.

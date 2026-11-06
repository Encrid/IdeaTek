# BC-IdeaTek-Resources

The contract of resources: loads, handles, loaders, sources and the state of each resource.

## Install

The release of `BC-IdeaTek-Resources` has the tag `resources-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Resources.zip` and `sha256.txt`. `BC-IdeaTek-Resources.zip` has a folder for each target.
In `BC-IdeaTek-Resources`, each folder holds `ideatek_resources.h` in `include`.
Each folder of `BC-IdeaTek-Resources.zip` also holds this readme and the licence.

`BC-IdeaTek-Resources` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Resources` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_resources.h`. `docs/extern/kernel/ideatek_resources.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Resources` are `BC-IdeaTek-World` and `BC-IdeaTek-Files`. Their readmes give their headers.

# BC-IdeaTek-InputMapping

The contract of the map of the inputs to actions: bindings, modifiers, triggers, contexts and rebinding.

## Install

The release of `BC-IdeaTek-InputMapping` has the tag `input_mapping-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-InputMapping.zip` and `sha256.txt`. `BC-IdeaTek-InputMapping.zip` has a folder for each target.
In `BC-IdeaTek-InputMapping`, each folder holds `ideatek_input_mapping.h` in `include`.
Each folder of `BC-IdeaTek-InputMapping.zip` also holds this readme and the licence.

`BC-IdeaTek-InputMapping` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-InputMapping` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_input_mapping.h`. `docs/extern/engine_subsystems/ideatek_input_mapping.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-InputMapping` are `BC-IdeaTek-World`, `BC-IdeaTek-Input` and `BC-IdeaTek-Frame`. Their readmes give their headers.

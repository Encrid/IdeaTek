# BC-IdeaTek-Ui

The contract of the user interface: nodes, layout, text, focus, widgets, screens and world markers.

## Install

The release of `BC-IdeaTek-Ui` has the tag `ui-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Ui.zip` and `sha256.txt`. `BC-IdeaTek-Ui.zip` has a folder for each target.
In `BC-IdeaTek-Ui`, each folder holds `ideatek_ui.h` in `include`.
Each folder of `BC-IdeaTek-Ui.zip` also holds this readme and the licence.

`BC-IdeaTek-Ui` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Ui` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_ui.h`. `docs/extern/engine_subsystems/ideatek_ui.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Ui` are `BC-IdeaTek-World`, `BC-IdeaTek-Space`, `BC-IdeaTek-Window`, `BC-IdeaTek-Input`, `BC-IdeaTek-Rendering`, `BC-IdeaTek-InputMapping`, `BC-IdeaTek-Animation` and `BC-IdeaTek-Localization`. Their readmes give their headers.

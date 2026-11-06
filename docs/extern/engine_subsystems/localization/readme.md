# BC-IdeaTek-Localization

The contract of localization: the locale, string tables, the keys of texts and localized texts.

## Install

The release of `BC-IdeaTek-Localization` has the tag `localization-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Localization.zip` and `sha256.txt`. `BC-IdeaTek-Localization.zip` has a folder for each target.
In `BC-IdeaTek-Localization`, each folder holds `ideatek_localization.h` in `include`.
Each folder of `BC-IdeaTek-Localization.zip` also holds this readme and the licence.

`BC-IdeaTek-Localization` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Localization` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_localization.h`. `docs/extern/engine_subsystems/ideatek_localization.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Localization` are `BC-IdeaTek-World` and `BC-IdeaTek-Resources`. Their readmes give their headers.

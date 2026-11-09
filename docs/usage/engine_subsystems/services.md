# BC-IdeaTek-Services

The contract of the services of a platform: users, tickets, lobbies, voice and downloadable content.

## Install

The release of `BC-IdeaTek-Services` has the tag `services-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Services.zip` and `sha256.txt`. `BC-IdeaTek-Services.zip` has a folder for each target.
In `BC-IdeaTek-Services`, each folder holds `ideatek_services.h` in `include`.
Each folder of `BC-IdeaTek-Services.zip` also holds this readme and the licence.

`BC-IdeaTek-Services` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Services` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_services.h`. `docs/extern/engine_subsystems/ideatek_services.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Services` are `BC-IdeaTek-World`, `BC-IdeaTek-Audio`, `BC-IdeaTek-Files` and `BC-IdeaTek-Sockets`. Their readmes give their headers.

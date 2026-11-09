# BC-IdeaTek-Audio

The contract of audio devices: the devices, 1 stream to play and 1 stream to record.

## Install

The release of `BC-IdeaTek-Audio` has the tag `audio-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BC-IdeaTek-Audio.zip` and `sha256.txt`. `BC-IdeaTek-Audio.zip` has a folder for each target.
In `BC-IdeaTek-Audio`, each folder holds `ideatek_audio.h` in `include`.
Each folder of `BC-IdeaTek-Audio.zip` also holds this readme and the licence.

`BC-IdeaTek-Audio` needs the headers of Bridge v0.5.0: https://github.com/Encrid/Bridge/releases/tag/v0.5.0.
They are `bridge_abi.h` and `bridge_schema.h`.
A project names the zip of `BC-IdeaTek-Audio` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes `ideatek_audio.h`. `docs/extern/platform_abstraction/ideatek_audio.h.md` gives its types and its rules.

The parts in the `uses` of `BC-IdeaTek-Audio` are `BC-IdeaTek-World`. Their readmes give their headers.

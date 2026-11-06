# BL-IdeaTek-SDL3

The shared library of SDL3 3.4.20 for the modules of IdeaTek that use SDL3, with its headers and its leak file.

## Install

The release of `BL-IdeaTek-SDL3` has the tag `sdl3-v<version>`: https://github.com/Encrid/IdeaTek/releases.
Its 2 files are `BL-IdeaTek-SDL3.zip` and `sha256.txt`. `BL-IdeaTek-SDL3.zip` has a folder for each target.
For each target, the zip of `sdl3` holds the library, its headers in `include/SDL3`, the notices, and `leaks.toml` for `[leaks] with` of a module.
The library is `SDL3.dll`, `SDL3.so` with that soname, or `SDL3.dylib` with the install name `@rpath/SDL3.dylib`. A module links it with no import library.

`sdl3` holds the shared library of SDL3 3.4.20. A deployment puts 1 copy of it beside its modules, so each module in the process uses the same SDL3.
A project names the zip of `BL-IdeaTek-SDL3` in a `[[source]]`: `.toolchain/docs/workflows.py.md`, *The release*.

## Usage

A module in C includes the headers of SDL3 from `include/SDL3`, and links the library of its target.
`docs/intern/platform_abstraction/sdl3/leaks.toml.md` tells the pairs and the kinds of `leaks.toml`.

- The library keeps GL, Vulkan and the GPU API. It loads the GL or Vulkan library of the system only for a window or a call that asks for it.
- On macOS, a window that names no graphics API and no external context asks for GL.
- `SDL_Metal_CreateView` gives a view with a CAMetalLayer on macOS, and no view on the other systems.

`BL-IdeaTek-SDL3` names no other part in `uses`.

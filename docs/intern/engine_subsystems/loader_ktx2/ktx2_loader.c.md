# src/engine_subsystems/loader_ktx2/ktx2_loader.c

This file spawns the entity of the loader at `init`.

## The texts

The 4 texts of `ideatek:ResourceLoader` are in the persistent arena, because a `bridge_text_t` names bytes of that arena.
`keep_text` takes the bytes with `alloc`, and copies the text into them. The loader has the name `ktx2`, the type `ideatek:Texture`, the extension `ktx2` and the 12 bytes of the identifier as its magic.

## Functions

| Function | Result |
| --- | --- |
| `keep_text` | A text in the persistent arena. |
| `spawn_loader` | Spawns the entity of the loader, and attaches its `ideatek:ResourceLoader`. |

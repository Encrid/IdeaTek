# src/engine_subsystems/loader_gltf/gltf_loader.c

This file spawns the entity of the glTF loader at `init`, and keeps the 2 type texts of the dependency loads.

The texts of a `bridge_text_t` must be in the persistent arena, so `keep_text` copies each text there with `alloc`.

## Functions

| Function | Result |
| --- | --- |
| `keep_text` | A text in the persistent arena. |
| `spawn_loader` | Keeps the type texts, spawns the loader and attaches `ideatek:ResourceLoader`. |

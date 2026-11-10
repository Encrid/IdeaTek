# src/engine_subsystems/loader_gltf/gltf_extensions.c

This file attaches or detaches each extension type of a material.

A type whose extension the material lacks is detached, so a reload removes it. An extension with no type writes 1 line.

## Functions

| Function | Result |
| --- | --- |
| `put` | Attaches a value, or detaches the type. |
| `give_coat` | Gives the clearcoat and the specular. |
| `give_volume` | Gives the transmission with the volume. |
| `give_flags` | Gives the unlit tag, and the line of an extension with no type. |
| `give_extensions` | Gives each extension of a material. |

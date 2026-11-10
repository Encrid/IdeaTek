# src/engine_subsystems/loader_ktx2/ktx2_hear.c

This file holds the functions of the 3 observers.

## The observers

- `bytes` takes each entity of its list whose `ideatek:ResourceBytes` names the loader of the module, and loads it. The bytes of another loader stay for that loader.
- `swap` hears a new `ideatek:Texture` on the resource. The barrier applied the new texels, so `swap` frees the block of the texels before the reload.
- `gone` hears `ideatek:Texture` leave, and frees the record and both blocks.

A texture that the module did not make has no record, and `swap` and `gone` skip it.
An item of the list of `bytes` always carries `ideatek:ResourceBytes` at the barrier. So the test of its value is a guard, and `.toolchain/project.toml` excepts its false outcome.

## Functions

| Function | Result |
| --- | --- |
| `get_triggers` | The items of the list of an observer, and their count. |
| `bytes` | The observer of the bytes. |
| `swap` | The observer of a new texture. |
| `gone` | The observer of a texture that leaves. |

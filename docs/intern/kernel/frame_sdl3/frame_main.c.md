# src/kernel/frame_sdl3/frame_main.c

This file loads the module.

## The load

`init` declares the types and the calls: `docs/intern/kernel/frame_sdl3/frame_declare.c.md`.
Then it attaches the 3 singletons to the entity 0. `ideatek:Frame` and `ideatek:Tick` start with zeros. `ideatek:TimeRequest` starts with its defaults: a scale of 1, a rate of 64, no limit and no pause.

It reads no setting, because Bridge writes the settings after `init`. The first round reads them: `docs/intern/kernel/frame_sdl3/frame_settings.c.md`.
`reload` and `shutdown` give 0. The module holds no object of the system, and its manifest says that it is not reloadable.

## Functions

| Function | Result |
| --- | --- |
| `init` | Declares the types and the calls, and attaches the 3 singletons. |
| `reload` | Gives 0. |
| `shutdown` | Gives 0. |

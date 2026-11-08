# src/platform_abstraction/files_mosl/files_paths.h

This header gives the path rule, the join and the split of the files module. They call no Bridge and no system, so a test program checks them with paths of its own.

`FILES_PATH` is 4096: a path with its storage has fewer bytes. The header holds it, so that the test program needs no other header of the module.
The functions are in `docs/intern/platform_abstraction/files_mosl/files_paths.c.md`.

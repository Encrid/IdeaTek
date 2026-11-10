# src/engine_subsystems/loader_gltf/gltf_wait.c

This file waits for the dependencies of a file, builds it, or refuses it.

A file is ready when each dependency has its resource, and each buffer has its copy. The bytes of the file stay good while it waits, because the resources module detaches them only after the outcome.
A refused file that has no output forgets its record and drops its dependencies. A refused reload keeps its parts.

## Functions

| Function | Result |
| --- | --- |
| `is_ready` | `true` when each dependency of the file has what the build needs. |
| `refuse_file` | Writes the line, attaches a failed outcome, and forgets a file with no output. |
| `wait_for` | Marks the file as waiting, and tries to build it. |
| `try_build` | Parses the file again and builds it when it is ready. |

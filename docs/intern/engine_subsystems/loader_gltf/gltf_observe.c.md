# src/engine_subsystems/loader_gltf/gltf_observe.c

This file adds the 6 observers of the glTF loader.

| Observer | Events | Term |
| --- | --- | --- |
| `bytes` | enter, attach | `ideatek:ResourceBytes` |
| `handles` | enter | `ideatek:ResourceHandle` |
| `raw` | enter, attach | `ideatek:RawBytes` |
| `failed` | enter | `ideatek:ResourceFailed` |
| `swap` | attach | `ideatek:Model` |
| `gone` | leave | `ideatek:Model` |

The first 4 can build a file, so each can attach each output type, the labels, the outcome, the loads and the reload tag. They run in this order with `before`, because Bridge needs an order for 2 writers of 1 type at a barrier.

## Functions

| Function | Result |
| --- | --- |
| `add_givers` | Adds the 4 observers that can build a file. |
| `add_observers` | Adds the 6 observers. |
| `get_triggers` | The triggers of the list of an observer, with their number. |

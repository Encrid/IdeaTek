# src/kernel/resources_files/resources_reload.c

This file reloads the resources: on a change of a watched file, on `ideatek:ResourceReload`, on a change of a variable, and for `reload_owner`.

## The time

`tick_time` adds `real_delta` of `ideatek:Frame` to `now` in each round. With no `ideatek:Frame` in the deployment, `now` does not move, and the delay of a change is 0. The first change writes the line of the clock: `docs/usage/kernel/resources_files.md`, *The messages*.

## The changes

The observer `changes` hears `ideatek:FileChange` on a watch entity of the module. Each line of `paths` is a changed path of the content storage.
A path that ends with `.meta` loses that end. The observer marks each resource of the source of the watch whose path is the changed path, or is under a changed folder. Its `reload_at` becomes `now` plus `reload_delay_ms`.
A second change before `reload_at` moves it: the events of 1 save join into 1 reload, and the writer can end the file.

## The reloads

`start_reloads` runs in each round:

- A resource with `ideatek:ResourceReload` reloads at once: the module detaches the tag.
- A resource whose `reload_at` came reloads.
- When a variable changed, each resource whose path holds the name of the variable reloads.

`begin_reload` keeps the entity and the uses, writes `IDEATEK_LOAD_STATE_LOADING`, and puts the resource back at `STEP_NEW` with `reloading` set. The end of the load then attaches `ideatek:ResourceModified`, and reloads each owner that asked `reload_owner`. A resource of a module, with an empty path, has no reload.
A reload while a read runs waits for the read: the resource is marked, and starts again when its load ends.

## Functions

| Function | Result |
| --- | --- |
| `tick_time` | Moves `now` with the clock. |
| `changes` | The observer of `ideatek:FileChange`. |
| `reloads` | The observer of `ideatek:ResourceReload`. |
| `begin_reload` | Starts the reload of a resource. |
| `start_reloads` | Starts the reloads that are due. |

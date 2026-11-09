# src/kernel/resources_files/resources_fold.c

This file folds the states of the dependencies of a resource into `dependencies` and `recursive`. It is pure: the program `resources_fold` of the tests calls it.

## The fold

A set of states is `IDEATEK_LOAD_STATE_FAILED` when 1 state failed. Else it is `IDEATEK_LOAD_STATE_LOADED` when each state is loaded, also when the set is empty. Else it is `IDEATEK_LOAD_STATE_LOADING`.
`fold_add` counts 1 state, and keeps the first error that is not `IDEATEK_RESOURCE_ERROR_NONE`. The counts are arithmetic on the state, with no branch.
`fold_state` gives the state of the set from its counts.

For each resource, the module folds `dependencies` from the `state` of the resources of its dependencies. It folds `recursive` from its own `state` and from the `recursive` of its dependencies.
A dependency is a use whose `owner` is that resource. The fold runs up to `RES_PASSES` passes over the resources, until a pass changes nothing, because a dependency can have dependencies.

## Functions

| Function | Result |
| --- | --- |
| `fold_start` | An empty fold. |
| `fold_add` | Adds 1 state and 1 error to a fold. |
| `fold_state` | The state of a fold. |

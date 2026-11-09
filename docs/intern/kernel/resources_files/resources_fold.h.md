# src/kernel/resources_files/resources_fold.h

This header gives the prototypes of the fold of the states of a resource and of its dependencies.

## The types

`res_fold_t` is the fold of a set of states: the count of the states, how many are loaded, how many failed, and the first error. `fold_add` adds 1 state and 1 error. `fold_state` gives the state of the set.

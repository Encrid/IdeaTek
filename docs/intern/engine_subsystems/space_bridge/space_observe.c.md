# src/engine_subsystems/space_bridge/space_observe.c

This file registers the 5 observers and the system of the module.

## The observers

| Observer | Terms | Events | Before it |
| --- | --- | --- | --- |
| `link` | `ideatek:Parent` | Enter, attach and leave | None |
| `place` | `ideatek:Transform` | Enter and attach | `link` |
| `relink` | `ideatek:Sibling` | Enter, attach and leave | `place` |
| `previous` | `ideatek:Interpolated` | Enter and leave | `relink` |
| `gone` | `ideatek:Transform` | Leave | `previous` |

`place` and `relink` share 1 function: an entity whose place or whose list changed is placed again.
Each observer states the types that it changes, so Bridge accepts its attaches and its detaches.

## The system

The query of `step` names `ideatek:Interpolated`, `ideatek:WorldTransform` and `ideatek:PreviousWorldTransform`.

## Functions

| Function | Result |
| --- | --- |
| `add_observer` | Registers 1 observer, and gives the slot of its list. |
| `observe_all` | Registers the 5 observers in order. |
| `add_step` | Registers the system `step` and its query. |

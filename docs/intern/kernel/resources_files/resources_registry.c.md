# src/kernel/resources_files/resources_registry.c

This file holds the observer of the loaders, and finds the sources and the variables by name. The observers of the source modules and of the variables are in `docs/intern/kernel/resources_files/resources_modules.c.md`.

## The loaders

A loader enters with `ideatek:ResourceLoader`: the observer takes a record with its entity and `ranged`. The texts stay in the component, which lives with the entity of the loader module.
A loader above `max_loaders` gets the line `no room for the loader <name>`. A loader that leaves frees its record.

## The names

`find_source` gives the record of a name, among the named sources and the source modules. `find_variable` gives the record of a variable by the name in its component.
`slot_with` finds the record that holds an entity, in any table whose records start with the entity.

## Functions

| Function | Result |
| --- | --- |
| `find_source` | The record of a source by its name, or `RES_NONE`. |
| `find_variable` | The record of a variable by its name, or `RES_NONE`. |
| `slot_with` | The index of the record with an entity, in a table of records, or `RES_NONE`. |
| `add_loader` | Takes the record of a loader, or writes the line. |
| `loaders` | The observer of the loaders. |

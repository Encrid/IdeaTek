# src/kernel/resources_files/resources_registry.c

This file holds the observers of the loaders, the sources and the variables, and finds them by name.

## The loaders

A loader enters with `ideatek:ResourceLoader`: the observer takes a record with its entity and `ranged`. The texts stay in the component, which lives with the entity of the loader module.
A loader above `max_loaders` gets the line `no room for the loader <name>`. A loader that leaves frees its record.

## The sources

A source module enters with `ideatek:ResourceSource`. Its name must follow the rule of the contract: `source_name_is_allowed`. A refused name gets the line `the source name <name> is refused`.
A name that a source holds already gets the line `2 sources have the name <name>`, and the first source stays. A source above `max_sources` gets the line `no room for the source <name>`.
`find_source` gives the record of a name, among the named sources and the source modules.

## The variables

A variable enters, changes or leaves with `ideatek:ResourceVariable`. The observer takes or frees its record, and marks the variables changed: the round reloads each resource whose path holds a changed name.
A second variable with 1 name gets the line `2 variables have the name <name>`, and the first stays. A variable above `max_variables` gets the line `no room for the variable <name>`.
`find_variable` gives the record of a name.

## Functions

| Function | Result |
| --- | --- |
| `loaders` | The observer of the loaders. |
| `sources` | The observer of the source modules. |
| `variables` | The observer of the variables. |
| `find_source` | The record of a source by its name, or `RES_NONE`. |
| `find_variable` | The record of a variable by its name, or `RES_NONE`. |

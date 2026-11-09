# src/kernel/resources_files/resources_modules.c

This file hears the source modules and the variables that come and go.

## The source modules

A source module enters with `ideatek:ResourceSource`. `source_fault` checks its name: a name outside the rule of the contract, a name that a source holds already, or no free record. Each fault has its line in `SOURCE_LINES`, and the source is not taken.
`add_source` fills a free record with the entity and the name. A source module that leaves frees its record.

## The variables

A variable enters, changes or leaves with `ideatek:ResourceVariable`. The observer marks the variables changed, so the round reloads the resources whose path holds a name.
`add_variable` refuses a second variable with 1 name, or a variable above `max_variables`, with the lines of `VARIABLE_LINES`. Else it keeps the entity.

## Functions

| Function | Result |
| --- | --- |
| `source_fault` | 0 for a good source, else the index of its fault. |
| `add_source` | Takes the record of a source module, or writes its fault. |
| `sources` | The observer of the source modules. |
| `add_variable` | Takes the record of a variable, or writes its fault. |
| `variables` | The observer of the variables. |

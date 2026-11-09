# src/kernel/resources_files/resources_loads.c

This file holds the observer `loads` and the records of the uses. The observer `adds` is in `docs/intern/kernel/resources_files/resources_adds.c.md`.

## The uses

A use entity enters with `ideatek:ResourceLoad` or `ideatek:ResourceAdd`. The observer takes a record: the entity, its `owner`, its `priority` and `reload_owner`, and no resource yet.
A use above `max_uses` gets no record: the log gets `no room for the use <entity>`, and the module does nothing more for it.
The system `round` gives each new use its resource: `docs/intern/kernel/resources_files/resources_uses.c.md`.

A use entity leaves when a module destroys it, or detaches its component. The observer drops its record, takes 1 from the count of its resource, and marks the module dirty.
A count of 0 frees the resource in the round: `docs/intern/kernel/resources_files/resources_free.c.md`.

## Functions

| Function | Result |
| --- | --- |
| `fill_use` | Fills the record of a use from its component. |
| `take_use` | Takes a record for a use, or writes the line. |
| `drop_use` | Frees the record of a use, and counts it off its resource. |
| `record_uses` | Walks the list of an observer: takes the uses that enter, drops the uses that leave. |
| `loads` | The observer of `ideatek:ResourceLoad`. |

# src/kernel/resources_files/resources_free.c

This file frees the resources that no use keeps, and the uses whose owner went.

## The owners

In each round, `check_owners` looks at each use with an `owner`: `bridge_native_find_value` of `ideatek:ideatek_entity_t` is not the test, because an owner can carry anything. The test is the entity index: a generation that differs tells a dead entity.
The module destroys the use entity of a dead owner. The observer `loads` or `adds` hears the leave at the barrier, and the count follows in the next round.

## A count of 0

`free_dead` walks the resources in a dirty round. A resource with a count of 0, and no parent, goes:

1. Its read that runs is canceled: the module destroys the request entity. The files module or the source module then stops the read.
2. Each sub-resource of it goes the same way, and its record is dropped.
3. Each use whose `owner` is the resource is destroyed. Its own resource loses 1 count at the barrier. It goes in the next round when nothing else keeps it.
4. The entity of the resource is destroyed, and its record is dropped.

The loader module hears the free through `BRIDGE_ON_EXIT` of its component, as the contract says.

## Functions

| Function | Result |
| --- | --- |
| `is_alive` | `true` when an id names an entity of the world. |
| `check_owners` | Destroys the use entities of the dead owners. |
| `cancel_read` | Destroys the request entity of a read, and frees the read. |
| `free_resource` | Runs the 4 steps for 1 resource. |
| `free_dead` | Frees each resource with a count of 0. |

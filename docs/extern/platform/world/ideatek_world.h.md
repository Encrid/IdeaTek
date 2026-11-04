# include/platform/world/ideatek_world.h

`include/platform/world/ideatek_world.h` gives the type that names an entity of the world: its C type and its descriptor.
`readme.md`, *The contracts*, tells the rules of each contract.

## ideatek:ideatek_entity_t

`ideatek_entity_t` names an entity. It is a struct of 8 bytes, with an alignment of 8.

| Field | C type | Offset | Value |
| --- | --- | --- | --- |
| `id` | `uint64_t` | 0 | The id of the entity that Bridge gives, or 0 for no entity. |

Bridge never gives the id 0 to an entity.
An id of a destroyed entity names nothing: the document of `bridge_abi.h` of Bridge tells the generation of an id.

## The C types

| Type of Bridge | C type | Descriptor | Macro of the text |
| --- | --- | --- | --- |
| `ideatek:ideatek_entity_t` | `ideatek_entity_t` | `ideatek_entity_t_desc` | `IDEATEK_ENTITY_FIELDS` |

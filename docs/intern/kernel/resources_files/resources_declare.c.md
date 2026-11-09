# src/kernel/resources_files/resources_declare.c

This file gives Bridge the types that the resources module reads and writes.

## The declarations

The module declares, in this order:

1. `ideatek:ideatek_entity_t`, because the structs name it.
2. The 3 enums and the 3 structs of the files part. The module asks the reads and the watches with `ideatek:FileRequest`, and reads the 2 other structs.
3. `ideatek:Frame` of the clock contract, a singleton, for the time of the reloads.
4. The 21 types of the resources contract, each type after the types of its fields. The order is the one of `docs/extern/kernel/ideatek_resources.h.md`, *The declarations*.
5. `ideatek_resources_files:Settings`, the singleton of the settings, as `res_settings_t`.

The 3 functions each declare 1 group, because 1 function of 28 declarations would pass 20 lines.

## Functions

| Function | Result |
| --- | --- |
| `declare_files` | Declares the entity type, the types of the files part and `ideatek:Frame`. |
| `declare_contract` | Declares the 21 types of the resources contract. |
| `declare_types` | Calls the 2 others, then declares the settings. |

# src/engine_subsystems/loader_ktx2/ktx2_declare.c

This file declares the types of the module, in the order of the slots of `docs/intern/engine_subsystems/loader_ktx2/ktx2_inside.h.md`.

## The order

1. The types of the fields: `ideatek:ideatek_entity_t`, `ideatek:ideatek_load_state_t`, `ideatek:ideatek_resource_error_t`, `ideatek:ideatek_dimension_t`, `ideatek:ideatek_filter_t` and `ideatek:ideatek_wrap_t`.
2. The types of the resources part: `ideatek:Resource`, `ideatek:ResourceLoader`, `ideatek:ResourceBytes` and `ideatek:ResourceOutcome`.
3. The types of the rendering part: `ideatek:Texture` and `ideatek:Sampler`.
4. The singleton of the settings, `ideatek_loader_ktx2:Settings`.

Each type of a contract takes the storage of its contract: a table, and `BRIDGE_ON_DESTROY_REMOVE`.

## Functions

| Function | Result |
| --- | --- |
| `declare_fields` | Declares the types of the fields. |
| `declare_types` | Declares each type of the module. |

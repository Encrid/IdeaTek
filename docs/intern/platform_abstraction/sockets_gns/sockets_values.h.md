# src/platform_abstraction/sockets_gns/sockets_values.h

This header declares the pure functions of the module: the words, the states, the flags and the records.

## The content

The header declares the functions of `docs/intern/platform_abstraction/sockets_gns/sockets_values.c.md` and of `docs/intern/platform_abstraction/sockets_gns/sockets_records.c.md`.
It declares `sockets_record_t`, the fields of 1 record of a batch: `connection` or `session`, `number`, `channel`, `length`, `lane` and `flags`. `bytes` is a pointer into the batch.
It includes `ideatek_sockets.h` and nothing of GameNetworkingSockets, so the program `sockets_values` of the tests builds it alone.
The constants `GNS_STATE_*` and `GNS_FLAG_*` that the functions read are in `gns_flat.h`, and the 2 sources include both headers.

The heads of the records of the contract are constants here: `RECORD_OUT_HEAD` 15, `RECORD_IN_HEAD` 23, `SESSION_RECORD_HEAD` 17, `SIGNAL_OUT_HEAD` 140 and `SIGNAL_IN_HEAD` 4. The contract gives them, from `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The messages*, to its sections of the sessions and of the signals.

# src/platform_abstraction/sockets_gns/sockets_values.h

This header declares the pure parts of the module: the words, the states, the flags, the records and the rings.

## The content

The header declares the functions of `docs/intern/platform_abstraction/sockets_gns/sockets_values.c.md`, `docs/intern/platform_abstraction/sockets_gns/sockets_flags.c.md`, `docs/intern/platform_abstraction/sockets_gns/sockets_records.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_ring.c.md`.
It declares `sockets_record_t`, the fields of 1 record of a batch: `connection` or `session`, `number`, `channel`, `length`, `lane` and `flags`. `bytes` is a pointer into the batch.
It declares `sockets_ring_t`, a ring of records in a span of bytes: its place, its size, its head and its tail.

It includes `ideatek_sockets.h` and nothing of GameNetworkingSockets or of Bridge. So the programs `sockets_values` and `sockets_ring` of the tests build the pure sources alone.
The constants `GNS_STATE_*`, `GNS_FLAG_*` and `GNS_SEND_*` that the functions read are in `gns_flat.h`, and the sources that need them include both headers.

The heads of the records of the contract are constants here: `RECORD_OUT_HEAD` 15, `RECORD_IN_HEAD` 23, `SESSION_RECORD_HEAD` 17, `SIGNAL_OUT_HEAD` 140 and `SIGNAL_IN_HEAD` 4. The contract gives them, from `docs/extern/platform_abstraction/ideatek_sockets.h.md`, *The messages*, to its sections of the sessions and of the signals.
The counts of the tables of words are constants here too: `ALLOW_COUNT` 3, `UNENCRYPTED_COUNT` 4, `ICE_COUNT` 3 and `LOG_COUNT` 9.

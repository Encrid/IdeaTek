# src/platform_abstraction/sockets_gns/sockets_flags.c

This file turns the flags of GameNetworkingSockets into the flags of the contract, and back.

## The flags

`flags_of` turns the flags of a connection of GameNetworkingSockets into `ideatek_connection_flag_t`. The 5 bits have the same places in both, so this is a mask of the 5 bits: unauthenticated, unencrypted, loopback, fast and relayed.

`send_of` turns `ideatek_send_t` into the send flags of GameNetworkingSockets, bit by bit: reliable, no Nagle, no delay and auto restart. The unreliable send is the absence of the reliable bit in both.
`send_back` turns the flags of a message that came into `ideatek_send_t`. Only the reliable bit has a meaning for a message that came.

The file includes `gns_flat.h` for the values of GameNetworkingSockets, and the program `sockets_values` of the tests builds it with no library.

## The branches

| Case | Result |
| --- | --- |
| A bit of GameNetworkingSockets outside the 5 flags. | `flags_of` drops it. |
| A send flag outside the 4 of the contract. | `send_of` drops it. |

## Functions

| Function | Result |
| --- | --- |
| `flags_of` | Gives the flags of the contract for the flags of a connection of GameNetworkingSockets. |
| `send_of` | Gives the send flags of GameNetworkingSockets for the send flags of the contract. |
| `send_back` | Gives the send flags of the contract for a message that came. |

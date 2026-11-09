# src/platform_abstraction/sockets_gns/sockets_values.c

This file turns the words of the settings into the numbers of GameNetworkingSockets, and its states into the statuses of the contract.

## The words

`word_number` finds a text in a table of words, and gives its index, or the count of the words when none matches.

The tables are constants of the file. `ALLOW_WORDS` holds `refuse`, `warn` and `allow`. `UNENCRYPTED_WORDS` holds `never`, `allowed`, `preferred` and `required`. `ICE_WORDS` holds `auto`, `native` and `webrtc`. `LOG_WORDS` holds the 9 levels of the debug output.

The index of each word is the value of GameNetworkingSockets for it. `IP_AllowWithoutAuth` is 0 to 2, `Unencrypted` 0 to 3, `P2P_Transport_ICE_Implementation` 0 to 2, and the levels 0 to 8.

`ice_mask` reads `ice_transports`, the words `relay`, `private` and `public` separated by commas, and gives the mask of GameNetworkingSockets: 1, 2 and 4. An empty text gives 0. A wrong word gives -1.

## The states

`status_of` turns a state of GameNetworkingSockets into `ideatek_connection_status_t`:

| State of GameNetworkingSockets | Status |
| --- | --- |
| Connecting | `IDEATEK_CONNECTION_STATUS_CONNECTING` |
| Finding route | `IDEATEK_CONNECTION_STATUS_FINDING_ROUTE` |
| Connected | `IDEATEK_CONNECTION_STATUS_CONNECTED` |
| Closed by peer | `IDEATEK_CONNECTION_STATUS_CLOSED_BY_PEER` |
| Problem detected locally, none, and the 2 ends | `IDEATEK_CONNECTION_STATUS_PROBLEM` |

The flags are in `docs/intern/platform_abstraction/sockets_gns/sockets_flags.c.md`.

## The branches

| Case | Result |
| --- | --- |
| A text that matches no word. | The count of the words. |
| `ice_transports` with a wrong word, or a comma with no word. | -1. |
| A state of GameNetworkingSockets outside its 4 live states. | `IDEATEK_CONNECTION_STATUS_PROBLEM`. |

## Functions

| Function | Result |
| --- | --- |
| `word_number` | Gives the index of a text in a table of words, or the count. |
| `ice_mask` | Gives the mask of the routes of `ice_transports`, or -1. |
| `status_of` | Gives the status of the contract for a state of GameNetworkingSockets. |

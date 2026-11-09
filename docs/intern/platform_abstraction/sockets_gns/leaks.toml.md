# src/platform_abstraction/sockets_gns/leaks.toml

This file names the resources of GameNetworkingSockets and of the OS that `test --leaks` counts for the sockets module.
`.toolchain/config/leaks.toml` names it in `[leaks] with`. The deliverable of the part carries it under the name `leaks.toml`.

## The listen sockets

The kind `gns_listen_socket` counts the listen sockets. `SteamAPI_ISteamNetworkingSockets_CreateListenSocketIP` and `SteamAPI_ISteamNetworkingSockets_CreateListenSocketP2P` take one at their result, a handle that is not 0.
`SteamAPI_ISteamNetworkingSockets_CloseListenSocket` gives one back at its second argument, the handle.
The handle is a number, so the listen sockets cannot be a pair: a pair takes a pointer.
The module closes each listen socket at the end of a run, and at an identity change: `docs/intern/platform_abstraction/sockets_gns/sockets_main.c.md` and `docs/intern/platform_abstraction/sockets_gns/sockets_close.c.md`.

## The addresses

The kind `os_addresses` counts the lists of addresses of the OS. `getaddrinfo` takes one at the pointer that its fourth argument points to, when the OS gives a list.
`freeaddrinfo` gives one back at its first argument.

## What the count leaves

`GameNetworkingSockets_Init` gives a boolean, and `GameNetworkingSockets_Kill` takes nothing, so the count cannot pair them. `GameNetworkingSockets_Kill` closes each connection that stays.
A message of `SteamAPI_ISteamNetworkingSockets_ReceiveMessagesOnConnection` comes in an array, which the count cannot read: `docs/intern/platform_abstraction/sockets_gns/sockets_receive.c.md` releases each message in place.
A message that the module sends is no resource of the module: `SteamAPI_ISteamNetworkingSockets_SendMessages` takes the message of `SteamAPI_ISteamNetworkingUtils_AllocateMessage`, and frees it.

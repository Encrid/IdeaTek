/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

BRIDGE_NATIVE(round)
{
    if (!sockets_module.started)
        start_sockets(api);
    if (sockets_module.broken)
        return (1);
    turn_half(api);
    drop_closed();
    say_lines(api);
    SteamNetworkingSockets_Poll(0);
    SteamAPI_ISteamNetworkingSockets_RunCallbacks(sockets_module.sockets);
    receive_connections(api);
    receive_sessions(api);
    attach_signals(api);
    attach_lanes(api);
    publish_names(api);
    attach_states(api);
    return (0);
}

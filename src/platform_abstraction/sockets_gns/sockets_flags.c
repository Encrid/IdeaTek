/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "gns_flat.h"
#include "sockets_values.h"

ideatek_connection_flag_t flags_of(int32_t flags)
{
    return ((ideatek_connection_flag_t)(flags & (GNS_FLAG_UNAUTHENTICATED
        | GNS_FLAG_UNENCRYPTED | GNS_FLAG_LOOPBACK | GNS_FLAG_FAST
        | GNS_FLAG_RELAYED)));
}

int send_of(ideatek_send_t flags)
{
    int out = 0;

    out |= ((flags & IDEATEK_SEND_RELIABLE) != 0) * GNS_SEND_RELIABLE;
    out |= ((flags & IDEATEK_SEND_NO_NAGLE) != 0) * GNS_SEND_NO_NAGLE;
    out |= ((flags & IDEATEK_SEND_NO_DELAY) != 0) * GNS_SEND_NO_DELAY;
    out |= ((flags & IDEATEK_SEND_AUTO_RESTART) != 0) * GNS_SEND_AUTO_RESTART;
    return (out);
}

ideatek_send_t send_back(int flags)
{
    return ((ideatek_send_t)(((flags & GNS_SEND_RELIABLE) != 0)
        * IDEATEK_SEND_RELIABLE));
}

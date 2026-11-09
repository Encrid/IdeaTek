/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

bool put_signal(uint64_t entity, const gns_identity_t *peer,
    const void *bytes, uint32_t len)
{
    const bridge_api_t *api = sockets_module.api;
    uint8_t record[SIGNAL_OUT_HEAD + SOCKETS_SIGNAL];
    bool ok;

    if (len > SOCKETS_SIGNAL)
        return (false);
    memset(record, 0, SIGNAL_OUT_HEAD);
    memcpy(record, &entity, 8);
    SteamAPI_SteamNetworkingIdentity_ToString(peer, (char *)record + 8, 128);
    memcpy(record + 136, &len, 4);
    memcpy(record + SIGNAL_OUT_HEAD, bytes, len);
    ok = ring_put(api->shared_base, &sockets_module.signals, record,
        SIGNAL_OUT_HEAD + len);
    return (ok);
}

static bool send_signal([[maybe_unused]] void *context,
    [[maybe_unused]] uint32_t connection, const gns_info_t *info,
    const void *bytes, int len)
{
    int64_t user = info->user_data;
    bool known = (user >= 0) & (user < sockets_module.connection_max);
    uint64_t entity = known ? sockets_module.connections[known * user].entity
        : 0;

    return (put_signal(entity, &info->identity_remote, bytes,
            (uint32_t)len));
}

static void release_signaling([[maybe_unused]] void *context)
{
}

gns_signaling_t *make_signaling(void)
{
    return (SteamAPI_ISteamNetworkingSockets_CreateCustomSignaling(nullptr,
            send_signal, release_signaling));
}

gns_signaling_t *create_signaling([[maybe_unused]] gns_sockets_t *sockets,
    [[maybe_unused]] const gns_identity_t *peer,
    [[maybe_unused]] int local_port, [[maybe_unused]] int remote_port)
{
    return (make_signaling());
}

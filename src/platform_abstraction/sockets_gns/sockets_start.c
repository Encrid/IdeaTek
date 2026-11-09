/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

static void take_texts(const bridge_api_t *api)
{
    uint8_t *base = api->shared_base;

    for (uint32_t i = 0; i < SOCKETS_TEXTS; i++)
        sockets_module.texts[i] = (char *)(base + api->alloc(api,
                SOCKETS_TEXT));
    sockets_module.debug_level = (char *)(base + api->alloc(api,
            SOCKETS_TEXT));
}

static void take_tables(const bridge_api_t *api, const sockets_settings_t *s)
{
    uint8_t *base = api->shared_base;

    sockets_module.connection_max = s->max_connections;
    sockets_module.connections = (void *)(base + api->alloc(api,
            s->max_connections * sizeof(sockets_connection_t)));
    memset(sockets_module.connections, 0,
        s->max_connections * sizeof(sockets_connection_t));
    sockets_module.listen_max = s->max_listen_sockets;
    sockets_module.listens = (void *)(base + api->alloc(api,
            s->max_listen_sockets * sizeof(sockets_listen_t)));
    memset(sockets_module.listens, 0,
        s->max_listen_sockets * sizeof(sockets_listen_t));
    sockets_module.name_max = s->max_addresses;
    sockets_module.names = (void *)(base + api->alloc(api,
            s->max_addresses * sizeof(sockets_name_t)));
    memset(sockets_module.names, 0, s->max_addresses * sizeof(sockets_name_t));
    sockets_module.channels = s->session_channels;
}

static void take_arena(const bridge_api_t *api, const sockets_settings_t *s)
{
    uint32_t gone = s->max_connections + s->max_addresses;

    sockets_module.half.at = api->alloc(api, s->message_bytes);
    sockets_module.half.size = s->message_bytes / 2;
    sockets_module.certificate_at = api->alloc(api, SOCKETS_CERTIFICATE);
    sockets_module.signals.at = api->alloc(api, SOCKETS_SIGNAL_RING);
    sockets_module.signals.size = SOCKETS_SIGNAL_RING;
    sockets_module.lines.at = api->alloc(api, SOCKETS_LINE_RING);
    sockets_module.lines.size = SOCKETS_LINE_RING;
    sockets_module.gone = (void *)(api->shared_base + api->alloc(api,
            gone * sizeof(uint64_t)));
    sockets_module.later = (void *)(api->shared_base + api->alloc(api,
            gone * sizeof(uint64_t)));
    take_tables(api, s);
}

static bool start_network(const bridge_api_t *api)
{
    char reason[GNS_ERR_MSG];
    gns_identity_t identity = {};
    int level = (int)word_number(sockets_module.debug_level, LOG_WORDS,
        LOG_COUNT);

    SteamAPI_SteamNetworkingIdentity_ParseString(&identity, sizeof(identity),
        "ip:::1");
    if (!start_gns(&identity, reason)) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no sockets: %s", reason);
        return (false);
    }
    SteamAPI_ISteamNetworkingUtils_SetDebugOutputFunction(
        sockets_module.utils, level, on_output);
    apply_settings(api);
    sockets_module.self = api->spawn(api);
    attach_identity(api);
    return (true);
}

bool start_sockets(const bridge_api_t *api)
{
    const sockets_settings_t *s = bridge_native_get_single(api, SETTINGS);
    bool ok = check_numbers(api, s);

    sockets_module.started = true;
    take_texts(api);
    ok &= copy_texts(api, s);
    ok = ok && check_words(api);
    if (ok) {
        take_arena(api, s);
        ok = start_network(api);
    }
    sockets_module.broken = !ok;
    return (ok);
}

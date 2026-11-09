/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

bool start_gns(const gns_identity_t *identity, char *reason)
{
    reason[0] = '\0';
    SteamNetworkingSockets_SetManualPollMode(true);
    if (!GameNetworkingSockets_Init(identity, reason))
        return (false);
    sockets_module.identity = *identity;
    sockets_module.sockets = SteamAPI_SteamNetworkingSockets_v009();
    sockets_module.utils = SteamAPI_SteamNetworkingUtils_v003();
    sockets_module.messages = SteamAPI_SteamNetworkingMessages_v002();
    return (true);
}

static void restart(const bridge_api_t *api, const gns_identity_t *identity,
    bridge_text_t certificate)
{
    char reason[GNS_ERR_MSG];

    stop_all(api);
    GameNetworkingSockets_Kill();
    sockets_module.sockets = nullptr;
    if (!start_gns(identity, reason)) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no sockets: %s", reason);
        sockets_module.broken = true;
        return;
    }
    apply_settings(api);
    set_certificate(api, certificate);
    attach_identity(api);
}

static void apply_identity(const bridge_api_t *api, uint64_t entity)
{
    const ideatek_identity_request_t *r = (const void *)(api->shared_base
        + bridge_native_find_value(api, entity, IDENTITY_REQUEST));
    gns_identity_t identity;
    char text[sizeof(r->identity.text) + 1] = "";

    memcpy(text, r->identity.text, sizeof(r->identity.text));
    if (SteamAPI_SteamNetworkingIdentity_ParseString(&identity,
            sizeof(identity), text))
        restart(api, &identity, r->certificate);
    else
        bridge_native_say(api, BRIDGE_LOG_WARN, "the identity %s is refused",
            text);
}

BRIDGE_NATIVE(identity)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_IDENTITY]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        if (t[i].id == sockets_module.self)
            apply_identity(api, t[i].id);
    return (0);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

void set_certificate(const bridge_api_t *api, bridge_text_t text)
{
    static const char *const WORDS[] = { "refused: ", "taken" };
    static const uint32_t KINDS[] = { BRIDGE_LOG_WARN, BRIDGE_LOG_INFO };
    char reason[GNS_ERR_MSG] = "";
    bool ok;

    sockets_module.auth_status = IDEATEK_AUTH_STATUS_NONE;
    if (!text.len)
        return;
    ok = SteamAPI_ISteamNetworkingSockets_SetCertificate(
        sockets_module.sockets, api->shared_base + text.at, (int)text.len,
        reason);
    bridge_native_say(api, KINDS[ok], "the certificate is %s%s", WORDS[ok],
        reason);
    sockets_module.auth_status = ok * IDEATEK_AUTH_STATUS_CURRENT
        + !ok * IDEATEK_AUTH_STATUS_FAILED;
}

void attach_identity(const bridge_api_t *api)
{
    ideatek_identity_state_t state = {};
    char reason[GNS_ERR_MSG];
    int len = (int)SOCKETS_CERTIFICATE;
    bool ok = SteamAPI_ISteamNetworkingSockets_GetCertificateRequest(
        sockets_module.sockets, &len, api->shared_base
        + sockets_module.certificate_at, reason);

    state.certificate_request.at = sockets_module.certificate_at;
    state.certificate_request.len = ok * (uint32_t)len;
    SteamAPI_SteamNetworkingIdentity_ToString(&sockets_module.identity,
        state.identity.text, sizeof(state.identity.text));
    state.status = (ideatek_auth_status_t)sockets_module.auth_status;
    BRIDGE_NATIVE_ATTACH(api, sockets_module.self, IDENTITY_STATE, state);
}

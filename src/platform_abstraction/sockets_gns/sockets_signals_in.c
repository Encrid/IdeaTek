/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

static gns_signaling_t *on_connect_request([[maybe_unused]] void *context,
    [[maybe_unused]] uint32_t connection,
    [[maybe_unused]] const gns_identity_t *peer,
    [[maybe_unused]] int virtual_port)
{
    return (make_signaling());
}

static void give_signals(const bridge_api_t *api, uint64_t entity)
{
    const ideatek_batch_t *b = batch_of(api, entity, SIGNALS_IN);
    const uint8_t *at = api->shared_base + b->data.at;
    uint32_t left = b->data.len;
    uint32_t used = 1;
    sockets_record_t r;

    while (left && used) {
        used = read_signal(at, left, &r);
        if (used && !SteamAPI_ISteamNetworkingSockets_ReceivedP2PCustomSignal2(
                sockets_module.sockets, r.bytes, (int)r.length, nullptr,
                on_connect_request, nullptr))
            bridge_native_say(api, BRIDGE_LOG_WARN,
                "a signal of %u bytes is refused", r.length);
        at += used;
        left -= used;
    }
    if (left)
        say_malformed(api, entity, b->data.len - left);
}

BRIDGE_NATIVE(signals_in)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_SIGNALS_IN]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    for (uint32_t i = 0; i < list->count; i++)
        give_signals(api, t[i].id);
    SteamNetworkingSockets_Poll(0);
    return (0);
}

void attach_signals(const bridge_api_t *api)
{
    uint32_t start = sockets_module.half.used;
    const uint8_t *bytes;
    uint32_t len;
    sockets_ring_t saved;

    saved = sockets_module.signals;
    len = ring_take(api->shared_base, &sockets_module.signals, &bytes);
    while ((len != 0) & (room_left() >= len)) {
        put_bytes(api, bytes, len);
        saved = sockets_module.signals;
        len = ring_take(api->shared_base, &sockets_module.signals,
            &bytes);
    }
    sockets_module.signals.tail = (len != 0) * saved.tail
        + (len == 0) * sockets_module.signals.tail;
    attach_batch(api, SIGNALS_OUT, start);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

void fill_info(ideatek_connection_state_t *s, const gns_info_t *info)
{
    bool ended = (info->state == GNS_STATE_CLOSED_BY_PEER)
        | (info->state == GNS_STATE_PROBLEM);

    s->status = status_of(info->state);
    s->flags = flags_of(info->flags);
    SteamAPI_SteamNetworkingIdentity_ToString(&info->identity_remote,
        s->remote_identity.text, sizeof(s->remote_identity.text));
    memcpy(&s->remote, &info->addr_remote, sizeof(s->remote));
    s->end_reason = ended * info->end_reason;
    memcpy(s->end_text, info->end_debug, ended * sizeof(s->end_text));
    s->end_text[sizeof(s->end_text) - 1] = '\0';
}

void fill_status(ideatek_connection_state_t *s,
    const gns_status_t *st)
{
    s->queue_time = st->queue_time;
    s->ping = st->ping;
    s->quality_local = st->quality_local;
    s->quality_remote = st->quality_remote;
    s->in_bytes = st->in_bytes;
    s->out_bytes = st->out_bytes;
    s->in_packets = st->in_packets;
    s->out_packets = st->out_packets;
    s->send_rate = st->send_rate;
    s->max_jitter = st->max_jitter;
    s->pending_reliable = st->pending_reliable;
    s->pending_unreliable = st->pending_unreliable;
    s->sent_unacked_reliable = st->sent_unacked_reliable;
}

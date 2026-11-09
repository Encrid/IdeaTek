/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

bridge_text_t write_lanes(const bridge_api_t *api, sockets_connection_t *c)
{
    gns_status_t status = {};
    gns_lane_t lanes[255];
    uint8_t record[20];
    bridge_text_t text = { .at = sockets_module.half.at
        + sockets_module.half.side * sockets_module.half.size
        + sockets_module.half.used };

    memset(lanes, 0, c->lanes * sizeof(*lanes));
    SteamAPI_ISteamNetworkingSockets_GetConnectionRealTimeStatus(
        sockets_module.sockets, c->handle, &status, (int)c->lanes, lanes);
    for (uint32_t i = 0; i < c->lanes; i++) {
        memcpy(record, &lanes[i].queue_time, 8);
        memcpy(record + 8, &lanes[i].pending_unreliable, 4);
        memcpy(record + 12, &lanes[i].pending_reliable, 4);
        memcpy(record + 16, &lanes[i].sent_unacked_reliable, 4);
        put_bytes(api, record, sizeof(record));
        text.len += sizeof(record);
    }
    return (text);
}

void attach_lanes(const bridge_api_t *api)
{
    sockets_connection_t *c = sockets_module.connections;
    ideatek_batch_t batch;
    bool fits;

    for (uint32_t i = 0; i < sockets_module.connection_max; i++) {
        fits = (c[i].entity != 0) & !c[i].closed & !c[i].forgotten
            & (c[i].lanes > 1) & (room_left() >= c[i].lanes * 20);
        if (!fits)
            continue;
        batch.data = write_lanes(api, &c[i]);
        BRIDGE_NATIVE_ATTACH(api, c[i].entity, LANE_STATES, batch);
    }
}

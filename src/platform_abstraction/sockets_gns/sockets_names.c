/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <string.h>

#include "sockets_inside.h"

static uint32_t free_name(void)
{
    sockets_name_t *n = sockets_module.names;
    uint32_t at = sockets_module.name_max;

    for (uint32_t i = sockets_module.name_max; i > 0; i--)
        at = n[i - 1].state == SOCKETS_FREE ? i - 1 : at;
    return (at);
}

static bool copy_name(const bridge_api_t *api, sockets_name_t *n,
    uint64_t entity)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    const ideatek_address_request_t *r = (const void *)(api->shared_base
        + bridge_native_find_value(api, entity, ADDRESS_REQUEST));
    bridge_text_t text = r->name;
    bool inside = (text.at >= h->arena_lo) & (text.at <= h->arena_hi)
        & (text.len <= h->arena_hi - text.at);
    bool fits = (text.len >= 1) & (text.len < SOCKETS_NAME);
    char field[64];

    if (!inside || !fits) {
        snprintf(field, sizeof(field), "name of %u bytes", text.len);
        say_request(api, entity, field, inside ? "1 to 255" : "the arena");
        return (false);
    }
    memcpy(n->name, api->shared_base + text.at, text.len);
    n->name[text.len] = '\0';
    return (true);
}

static void queue_name(const bridge_api_t *api, uint64_t entity)
{
    uint32_t at = free_name();
    sockets_name_t *n = &sockets_module.names[at % sockets_module.name_max];
    ideatek_address_result_t failed = {
        .status = IDEATEK_ADDRESS_STATUS_FAILED };

    if (at == sockets_module.name_max) {
        say_bound(api, "names", sockets_module.name_max, entity);
        BRIDGE_NATIVE_ATTACH(api, entity, ADDRESS_RESULT, failed);
        drop_later(entity);
        return;
    }
    memset(n, 0, sizeof(*n));
    n->entity = entity;
    n->status = IDEATEK_ADDRESS_STATUS_FAILED;
    n->state = copy_name(api, n, entity) ? SOCKETS_WAITING : SOCKETS_DONE;
}

BRIDGE_NATIVE(address)
{
    const bridge_list_t *list = bridge_native_get_list(api,
        sockets_module.lists[LIST_ADDRESS]);
    const bridge_trigger_t *t = (const void *)(api->private_base + list->at);

    os_mutex_lock(sockets_module.lock);
    for (uint32_t i = 0; i < list->count; i++)
        queue_name(api, t[i].id);
    os_mutex_unlock(sockets_module.lock);
    os_event_set(sockets_module.wake);
    return (0);
}

void publish_names(const bridge_api_t *api)
{
    sockets_name_t *n = sockets_module.names;
    ideatek_address_result_t result;
    bool ready;

    os_mutex_lock(sockets_module.lock);
    for (uint32_t i = 0; i < sockets_module.name_max; i++) {
        ready = (n[i].state == SOCKETS_DONE)
            & (room_left() >= n[i].count * sizeof(ideatek_address_t));
        if (!ready)
            continue;
        result.status = (ideatek_address_status_t)n[i].status;
        result.addresses = put_bytes(api, n[i].addresses,
            n[i].count * sizeof(ideatek_address_t));
        BRIDGE_NATIVE_ATTACH(api, n[i].entity, ADDRESS_RESULT, result);
        sockets_module.gone[sockets_module.gone_count++] = n[i].entity;
        n[i].state = SOCKETS_FREE;
    }
    os_mutex_unlock(sockets_module.lock);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_inside.h"

uint32_t find_entity(uint64_t entity)
{
    sockets_connection_t *c = sockets_module.connections;
    uint32_t at = sockets_module.connection_max;
    bool hit;

    for (uint32_t i = 0; i < sockets_module.connection_max; i++) {
        hit = (entity != 0) & (c[i].entity == entity);
        at = hit * i + !hit * at;
    }
    return (at);
}

uint32_t find_peer(const gns_identity_t *peer)
{
    sockets_connection_t *c = sockets_module.connections;
    uint32_t at = sockets_module.connection_max;
    bool hit;

    for (uint32_t i = 0; i < sockets_module.connection_max; i++) {
        hit = (c[i].entity != 0) & c[i].session
            & !memcmp(&c[i].peer, peer, sizeof(*peer));
        at = hit * i + !hit * at;
    }
    return (at);
}

sockets_connection_t *take_slot(uint64_t entity)
{
    sockets_connection_t *c = sockets_module.connections;
    uint32_t at = sockets_module.connection_max;

    for (uint32_t i = sockets_module.connection_max; i > 0; i--)
        at = c[i - 1].entity ? at : i - 1;
    if (at == sockets_module.connection_max)
        return (nullptr);
    memset(&c[at], 0, sizeof(c[at]));
    c[at].entity = entity;
    c[at].lanes = 1;
    return (&c[at]);
}

void free_slot(sockets_connection_t *c)
{
    memset(c, 0, sizeof(*c));
}

void drop_closed(void)
{
    sockets_connection_t *c = sockets_module.connections;

    for (uint32_t i = 0; i < sockets_module.connection_max; i++)
        if (c[i].entity && c[i].closed)
            free_slot(&c[i]);
}

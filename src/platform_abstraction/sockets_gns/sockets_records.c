/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "sockets_values.h"

uint32_t read_message(const uint8_t *at, uint32_t left, sockets_record_t *r)
{
    if (left < RECORD_OUT_HEAD)
        return (0);
    memcpy(&r->connection, at, 8);
    memcpy(&r->length, at + 8, 4);
    memcpy(&r->lane, at + 12, 2);
    r->flags = at[14];
    r->bytes = at + RECORD_OUT_HEAD;
    if (left - RECORD_OUT_HEAD < r->length)
        return (0);
    return (RECORD_OUT_HEAD + r->length);
}

uint32_t read_session_message(const uint8_t *at, uint32_t left,
    sockets_record_t *r)
{
    if (left < SESSION_RECORD_HEAD)
        return (0);
    memcpy(&r->connection, at, 8);
    memcpy(&r->channel, at + 8, 4);
    memcpy(&r->length, at + 12, 4);
    r->flags = at[16];
    r->bytes = at + SESSION_RECORD_HEAD;
    if (left - SESSION_RECORD_HEAD < r->length)
        return (0);
    return (SESSION_RECORD_HEAD + r->length);
}

uint32_t read_signal(const uint8_t *at, uint32_t left, sockets_record_t *r)
{
    if (left < SIGNAL_IN_HEAD)
        return (0);
    memcpy(&r->length, at, 4);
    r->bytes = at + SIGNAL_IN_HEAD;
    if (left - SIGNAL_IN_HEAD < r->length)
        return (0);
    return (SIGNAL_IN_HEAD + r->length);
}

uint32_t write_message(uint8_t *at, uint32_t room, const sockets_record_t *r)
{
    if (room < RECORD_IN_HEAD || room - RECORD_IN_HEAD < r->length)
        return (0);
    memcpy(at, &r->connection, 8);
    memcpy(at + 8, &r->number, 8);
    memcpy(at + 16, &r->length, 4);
    memcpy(at + 20, &r->lane, 2);
    at[22] = r->flags;
    memcpy(at + RECORD_IN_HEAD, r->bytes, r->length);
    return (RECORD_IN_HEAD + r->length);
}

uint32_t write_session_message(uint8_t *at, uint32_t room,
    const sockets_record_t *r)
{
    if (room < SESSION_RECORD_HEAD || room - SESSION_RECORD_HEAD < r->length)
        return (0);
    memcpy(at, &r->connection, 8);
    memcpy(at + 8, &r->channel, 4);
    memcpy(at + 12, &r->length, 4);
    at[16] = r->flags;
    memcpy(at + SESSION_RECORD_HEAD, r->bytes, r->length);
    return (SESSION_RECORD_HEAD + r->length);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_sockets.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_address(void)
{
    SIZE(ideatek_address_t);
    AT(ideatek_address_t, bytes);
    AT(ideatek_address_t, port);
}

static void show_send(void)
{
    SIZE(ideatek_send_t);
}

static void show_listen_status(void)
{
    SIZE(ideatek_listen_status_t);
}

static void show_action(void)
{
    SIZE(ideatek_connection_action_t);
}

static void show_connection_status(void)
{
    SIZE(ideatek_connection_status_t);
}

static void show_address_status(void)
{
    SIZE(ideatek_address_status_t);
}

static void show_listen_request(void)
{
    SIZE(ideatek_listen_request_t);
    AT(ideatek_listen_request_t, port);
}

static void show_listen_state(void)
{
    SIZE(ideatek_listen_state_t);
    AT(ideatek_listen_state_t, port);
    AT(ideatek_listen_state_t, status);
}

static void show_connection_request(void)
{
    SIZE(ideatek_connection_request_t);
    AT(ideatek_connection_request_t, send_rate);
    AT(ideatek_connection_request_t, reason);
    AT(ideatek_connection_request_t, address);
    AT(ideatek_connection_request_t, action);
    AT(ideatek_connection_request_t, linger);
}

static void show_inbound(void)
{
    SIZE(ideatek_inbound_t);
    AT(ideatek_inbound_t, listen);
}

static void show_connection_state(void)
{
    SIZE(ideatek_connection_state_t);
    AT(ideatek_connection_state_t, queue_time);
    AT(ideatek_connection_state_t, end_reason);
    AT(ideatek_connection_state_t, ping);
    AT(ideatek_connection_state_t, quality_local);
    AT(ideatek_connection_state_t, quality_remote);
    AT(ideatek_connection_state_t, in_bytes);
    AT(ideatek_connection_state_t, out_bytes);
    AT(ideatek_connection_state_t, pending_reliable);
    AT(ideatek_connection_state_t, pending_unreliable);
    AT(ideatek_connection_state_t, remote);
    AT(ideatek_connection_state_t, status);
}

static void show_messages(void)
{
    SIZE(ideatek_messages_t);
    AT(ideatek_messages_t, data);
}

static void show_address_request(void)
{
    SIZE(ideatek_address_request_t);
    AT(ideatek_address_request_t, name);
}

static void show_address_result(void)
{
    SIZE(ideatek_address_result_t);
    AT(ideatek_address_result_t, addresses);
    AT(ideatek_address_result_t, status);
}

static void show_descriptors(void)
{
    static const char *const DESCRIPTORS[] = {
        ideatek_address_t_desc, ideatek_send_t_desc,
        ideatek_listen_status_t_desc, ideatek_connection_action_t_desc,
        ideatek_connection_status_t_desc, ideatek_address_status_t_desc,
        ideatek_listen_request_t_desc, ideatek_listen_state_t_desc,
        ideatek_connection_request_t_desc, ideatek_inbound_t_desc,
        ideatek_connection_state_t_desc, ideatek_messages_t_desc,
        ideatek_address_request_t_desc, ideatek_address_result_t_desc
    };

    for (size_t i = 0; i < sizeof(DESCRIPTORS) / sizeof(DESCRIPTORS[0]); i++)
        printf("%s\n", DESCRIPTORS[i]);
}

static void show_texts(void)
{
    static const char *const TEXTS[] = {
        TEXT_OF(IDEATEK_ADDRESS_FIELDS),
        TEXT_OF(IDEATEK_SEND_VALUES),
        TEXT_OF(IDEATEK_LISTEN_STATUS_VALUES),
        TEXT_OF(IDEATEK_CONNECTION_ACTION_VALUES),
        TEXT_OF(IDEATEK_CONNECTION_STATUS_VALUES),
        TEXT_OF(IDEATEK_ADDRESS_STATUS_VALUES),
        TEXT_OF(IDEATEK_LISTEN_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_LISTEN_STATE_FIELDS),
        TEXT_OF(IDEATEK_CONNECTION_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_INBOUND_FIELDS),
        TEXT_OF(IDEATEK_CONNECTION_STATE_FIELDS),
        TEXT_OF(IDEATEK_MESSAGES_FIELDS),
        TEXT_OF(IDEATEK_ADDRESS_REQUEST_FIELDS),
        TEXT_OF(IDEATEK_ADDRESS_RESULT_FIELDS)
    };

    for (size_t i = 0; i < sizeof(TEXTS) / sizeof(TEXTS[0]); i++)
        printf("%s\n", TEXTS[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "address", show_address }, { "send", show_send },
        { "listen_status", show_listen_status }, { "action", show_action },
        { "connection_status", show_connection_status },
        { "address_status", show_address_status },
        { "listen_request", show_listen_request },
        { "listen_state", show_listen_state },
        { "connection_request", show_connection_request },
        { "inbound", show_inbound },
        { "connection_state", show_connection_state },
        { "messages", show_messages },
        { "address_request", show_address_request },
        { "address_result", show_address_result },
        { "descriptors", show_descriptors }, { "texts", show_texts }
    };

    for (size_t i = 0; argc > 1 && i < sizeof(MODES) / sizeof(MODES[0]); i++)
        if (!strcmp(argv[1], MODES[i].name)) {
            MODES[i].show();
            return (0);
        }
    printf("usage: layout <mode>\n");
    return (2);
}

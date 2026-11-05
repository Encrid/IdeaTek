/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ideatek_networking.h>

#define SIZE(type) printf("size %zu\nalignment %zu\n", sizeof(type), \
    alignof(type))
#define AT(type, field) printf("%s %zu\n", #field, offsetof(type, field))
#define TEXT_OF(...) BRIDGE_SCHEMA_TEXT(__VA_ARGS__)

typedef struct {
    const char *name;
    void (*show)(void);
} show_mode_t;

static void show_net_role(void)
{
    SIZE(ideatek_net_role_t);
}

static void show_net_session(void)
{
    SIZE(ideatek_net_session_t);
    AT(ideatek_net_session_t, connection);
    AT(ideatek_net_session_t, listen);
    AT(ideatek_net_session_t, protocol);
    AT(ideatek_net_session_t, role);
}

static void show_peer_state(void)
{
    SIZE(ideatek_peer_state_t);
}

static void show_net_peer(void)
{
    SIZE(ideatek_net_peer_t);
    AT(ideatek_net_peer_t, session);
    AT(ideatek_net_peer_t, connection);
    AT(ideatek_net_peer_t, id);
    AT(ideatek_net_peer_t, state);
}

static void show_net_join(void)
{
    SIZE(ideatek_net_join_t);
    AT(ideatek_net_join_t, data);
}

static void show_net_join_event(void)
{
    SIZE(ideatek_net_join_event_t);
    AT(ideatek_net_join_event_t, peer);
    AT(ideatek_net_join_event_t, data);
    AT(ideatek_net_join_event_t, order);
}

static void show_admit(void)
{
    SIZE(ideatek_admit_t);
}

static void show_net_admit(void)
{
    SIZE(ideatek_net_admit_t);
    AT(ideatek_net_admit_t, reason);
    AT(ideatek_net_admit_t, action);
}

static void show_net_authority(void)
{
    SIZE(ideatek_net_authority_t);
}

static void show_replicated(void)
{
    SIZE(ideatek_replicated_t);
    AT(ideatek_replicated_t, prefab);
    AT(ideatek_replicated_t, group);
    AT(ideatek_replicated_t, owner);
    AT(ideatek_replicated_t, host);
    AT(ideatek_replicated_t, authority);
}

static void show_net_viewer(void)
{
    SIZE(ideatek_net_viewer_t);
    AT(ideatek_net_viewer_t, peer);
}

static void show_relevance_mode(void)
{
    SIZE(ideatek_relevance_mode_t);
}

static void show_net_relevance(void)
{
    SIZE(ideatek_net_relevance_t);
    AT(ideatek_net_relevance_t, groups);
    AT(ideatek_net_relevance_t, distance);
    AT(ideatek_net_relevance_t, priority);
    AT(ideatek_net_relevance_t, steps);
    AT(ideatek_net_relevance_t, mode);
}

static void show_net_command(void)
{
    SIZE(ideatek_net_command_t);
    AT(ideatek_net_command_t, data);
    AT(ideatek_net_command_t, tick);
    AT(ideatek_net_command_t, view_tick);
}

static void show_net_command_in(void)
{
    SIZE(ideatek_net_command_in_t);
    AT(ideatek_net_command_in_t, data);
    AT(ideatek_net_command_in_t, peer);
    AT(ideatek_net_command_in_t, tick);
    AT(ideatek_net_command_in_t, view_tick);
}

static void show_net_session_state(void)
{
    SIZE(ideatek_net_session_state_t);
    AT(ideatek_net_session_state_t, tick);
    AT(ideatek_net_session_state_t, command_tick);
    AT(ideatek_net_session_state_t, playback);
    AT(ideatek_net_session_state_t, ack);
    AT(ideatek_net_session_state_t, confirmed);
    AT(ideatek_net_session_state_t, host);
}

static void show_net_peer_groups(void)
{
    SIZE(ideatek_net_peer_groups_t);
    AT(ideatek_net_peer_groups_t, groups);
}

static void show_net_predicted_spawn(void)
{
    SIZE(ideatek_net_predicted_spawn_t);
    AT(ideatek_net_predicted_spawn_t, key);
}

static void show_net_rollback(void)
{
    SIZE(ideatek_net_rollback_t);
    AT(ideatek_net_rollback_t, group);
    AT(ideatek_net_rollback_t, tick);
    AT(ideatek_net_rollback_t, order);
}

static void show_net_authority_changed(void)
{
    SIZE(ideatek_net_authority_changed_t);
    AT(ideatek_net_authority_changed_t, entity);
    AT(ideatek_net_authority_changed_t, old_host);
    AT(ideatek_net_authority_changed_t, new_host);
    AT(ideatek_net_authority_changed_t, order);
    AT(ideatek_net_authority_changed_t, old_authority);
    AT(ideatek_net_authority_changed_t, new_authority);
}

static void show_net_host_changed(void)
{
    SIZE(ideatek_net_host_changed_t);
    AT(ideatek_net_host_changed_t, old_host);
    AT(ideatek_net_host_changed_t, new_host);
    AT(ideatek_net_host_changed_t, order);
}

static void show_descriptors(void)
{
    const char *descriptors[] = {
        ideatek_net_role_t_desc, ideatek_net_session_t_desc,
        ideatek_peer_state_t_desc, ideatek_net_peer_t_desc,
        ideatek_net_join_t_desc, ideatek_net_join_event_t_desc,
        ideatek_admit_t_desc, ideatek_net_admit_t_desc,
        ideatek_net_authority_t_desc, ideatek_replicated_t_desc,
        ideatek_net_viewer_t_desc, ideatek_relevance_mode_t_desc,
        ideatek_net_relevance_t_desc, ideatek_net_command_t_desc,
        ideatek_net_command_in_t_desc, ideatek_net_session_state_t_desc,
        ideatek_net_peer_groups_t_desc, ideatek_net_predicted_spawn_t_desc,
        ideatek_net_rollback_t_desc, ideatek_net_authority_changed_t_desc,
        ideatek_net_host_changed_t_desc };

    for (size_t i = 0; i < sizeof(descriptors) / sizeof(descriptors[0]); i++)
        printf("%s\n", descriptors[i]);
}

static void show_texts(void)
{
    const char *texts[] = {
        TEXT_OF(IDEATEK_NET_ROLE_VALUES), TEXT_OF(IDEATEK_NET_SESSION_FIELDS),
        TEXT_OF(IDEATEK_PEER_STATE_VALUES), TEXT_OF(IDEATEK_NET_PEER_FIELDS),
        TEXT_OF(IDEATEK_NET_JOIN_FIELDS),
        TEXT_OF(IDEATEK_NET_JOIN_EVENT_FIELDS), TEXT_OF(IDEATEK_ADMIT_VALUES),
        TEXT_OF(IDEATEK_NET_ADMIT_FIELDS),
        TEXT_OF(IDEATEK_NET_AUTHORITY_VALUES),
        TEXT_OF(IDEATEK_REPLICATED_FIELDS), TEXT_OF(IDEATEK_NET_VIEWER_FIELDS),
        TEXT_OF(IDEATEK_RELEVANCE_MODE_VALUES),
        TEXT_OF(IDEATEK_NET_RELEVANCE_FIELDS),
        TEXT_OF(IDEATEK_NET_COMMAND_FIELDS),
        TEXT_OF(IDEATEK_NET_COMMAND_IN_FIELDS),
        TEXT_OF(IDEATEK_NET_SESSION_STATE_FIELDS),
        TEXT_OF(IDEATEK_NET_PEER_GROUPS_FIELDS),
        TEXT_OF(IDEATEK_NET_PREDICTED_SPAWN_FIELDS),
        TEXT_OF(IDEATEK_NET_ROLLBACK_FIELDS),
        TEXT_OF(IDEATEK_NET_AUTHORITY_CHANGED_FIELDS),
        TEXT_OF(IDEATEK_NET_HOST_CHANGED_FIELDS) };

    for (size_t i = 0; i < sizeof(texts) / sizeof(texts[0]); i++)
        printf("%s\n", texts[i]);
}

int main(int argc, char **argv)
{
    static const show_mode_t MODES[] = {
        { "net_role", show_net_role }, { "net_session", show_net_session },
        { "peer_state", show_peer_state }, { "net_peer", show_net_peer },
        { "net_join", show_net_join },
        { "net_join_event", show_net_join_event }, { "admit", show_admit },
        { "net_admit", show_net_admit },
        { "net_authority", show_net_authority },
        { "replicated", show_replicated }, { "net_viewer", show_net_viewer },
        { "relevance_mode", show_relevance_mode },
        { "net_relevance", show_net_relevance },
        { "net_command", show_net_command },
        { "net_command_in", show_net_command_in },
        { "net_session_state", show_net_session_state },
        { "net_peer_groups", show_net_peer_groups },
        { "net_predicted_spawn", show_net_predicted_spawn },
        { "net_rollback", show_net_rollback },
        { "net_authority_changed", show_net_authority_changed },
        { "net_host_changed", show_net_host_changed },
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

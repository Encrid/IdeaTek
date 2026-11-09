/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "sockets_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

sockets_module_t sockets_module;

static void close_listens(void)
{
    sockets_listen_t *l = sockets_module.listens;

    for (uint32_t i = 0; i < sockets_module.listen_max; i++)
        if (l[i].entity)
            SteamAPI_ISteamNetworkingSockets_CloseListenSocket(
                sockets_module.sockets, l[i].handle);
}

static void free_thread(void)
{
    os_event_free(sockets_module.wake);
    os_mutex_free(sockets_module.lock);
    sockets_module.wake = nullptr;
    sockets_module.lock = nullptr;
}

BRIDGE_NATIVE(init)
{
    sockets_module.api = api;
    declare_types(api);
    add_runs(api);
    sockets_module.lock = os_mutex_create();
    sockets_module.wake = os_event_create();
    if (sockets_module.lock && sockets_module.wake)
        sockets_module.thread = os_thread_start(names_loop, nullptr,
            SOCKETS_STACK);
    if (!sockets_module.thread) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no thread");
        free_thread();
        return (1);
    }
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    sockets_module.api = api;
    os_mutex_lock(sockets_module.lock);
    sockets_module.stop = true;
    os_mutex_unlock(sockets_module.lock);
    os_event_set(sockets_module.wake);
    os_thread_join(sockets_module.thread);
    if (sockets_module.sockets) {
        close_listens();
        GameNetworkingSockets_Kill();
    }
    sockets_module.sockets = nullptr;
    free_thread();
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}

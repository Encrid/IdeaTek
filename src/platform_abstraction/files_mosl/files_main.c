/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>

#include "files_inside.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

files_module_t files_module;

static void free_thread(void)
{
    os_event_free(files_module.wake);
    os_mutex_free(files_module.lock);
    files_module.wake = nullptr;
    files_module.lock = nullptr;
}

BRIDGE_NATIVE(init)
{
    declare_types(api);
    add_runs(api);
    files_module.lock = os_mutex_create();
    files_module.wake = os_event_create();
    if (files_module.lock && files_module.wake)
        files_module.thread = os_thread_start(work_loop, nullptr,
            FILES_STACK);
    if (!files_module.thread) {
        bridge_native_say(api, BRIDGE_LOG_ERR, "no thread");
        free_thread();
        return (1);
    }
    return (0);
}

static void free_jobs(void)
{
    for (uint32_t i = 0; i < FILES_QUEUE; i++) {
        free(files_module.jobs[i].data);
        files_module.jobs[i].data = nullptr;
    }
}

BRIDGE_NATIVE(shutdown)
{
    os_mutex_lock(files_module.lock);
    files_module.stop = true;
    os_mutex_unlock(files_module.lock);
    os_event_set(files_module.wake);
    os_thread_join(files_module.thread);
    while (files_module.watch_count)
        stop_watch(files_module.watches[0].entity);
    free_jobs();
    free_thread();
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (0);
}

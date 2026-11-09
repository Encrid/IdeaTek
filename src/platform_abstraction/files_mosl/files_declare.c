/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "files_inside.h"

void declare_types(const bridge_api_t *api)
{
    FILES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_storage_t",
        ideatek_storage_t);
    FILES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_file_action_t",
        ideatek_file_action_t);
    FILES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_file_status_t",
        ideatek_file_status_t);
    FILES_TYPE(api, BRIDGE_STRUCT, "ideatek:FileRequest",
        ideatek_file_request_t);
    FILES_TYPE(api, BRIDGE_STRUCT, "ideatek:FileResult",
        ideatek_file_result_t);
    FILES_TYPE(api, BRIDGE_STRUCT, "ideatek:FileChange",
        ideatek_file_change_t);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek_files_mosl:Settings",
        files_settings_t, BRIDGE_STRIP, BRIDGE_SINGLETON,
        BRIDGE_ON_DESTROY_REMOVE);
}

void add_runs(const bridge_api_t *api)
{
    const uint32_t changes[] = { RESULT, CHANGE };
    bridge_term_t request[1] = {{ REQUEST, BRIDGE_READ, BRIDGE_AND, 0 }};

    BRIDGE_NATIVE_ADD_SYSTEM(api, "results", "first", nullptr, 0, changes, 2,
        nullptr, 0);
    files_module.request_list = BRIDGE_NATIVE_OBSERVER(api, "request",
        BRIDGE_ON_ENTER | BRIDGE_ON_LEAVE, request, 1, nullptr, 0, nullptr,
        0);
}

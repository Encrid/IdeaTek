/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_inside.h"

static void declare_files(const bridge_api_t *api)
{
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ideatek_entity_t",
        ideatek_entity_t);
    RESOURCES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_storage_t",
        ideatek_storage_t);
    RESOURCES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_file_action_t",
        ideatek_file_action_t);
    RESOURCES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_file_status_t",
        ideatek_file_status_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:FileRequest",
        ideatek_file_request_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:FileResult",
        ideatek_file_result_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:FileChange",
        ideatek_file_change_t);
    RESOURCES_SINGLE(api, "ideatek:Frame", ideatek_frame_t);
}

static void declare_loads(const bridge_api_t *api)
{
    RESOURCES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_load_state_t",
        ideatek_load_state_t);
    RESOURCES_TYPE(api, BRIDGE_ENUM, "ideatek:ideatek_resource_error_t",
        ideatek_resource_error_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceLoad",
        ideatek_resource_load_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceAdd",
        ideatek_resource_add_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceHandle",
        ideatek_resource_handle_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:Resource",
        ideatek_resource_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceLoader",
        ideatek_resource_loader_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceBytes",
        ideatek_resource_bytes_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceRead",
        ideatek_resource_read_t);
}

static void declare_rest(const bridge_api_t *api)
{
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceOutcome",
        ideatek_resource_outcome_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceLabel",
        ideatek_resource_label_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:RawBytes",
        ideatek_raw_bytes_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceSource",
        ideatek_resource_source_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceSourceRead",
        ideatek_resource_source_read_t);
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceSourceResult",
        ideatek_resource_source_result_t);
    RESOURCES_TAG(api, "ideatek:ResourceReload");
    RESOURCES_TAG(api, "ideatek:ResourceModified");
    RESOURCES_TAG(api, "ideatek:ResourceReady");
    RESOURCES_TAG(api, "ideatek:ResourceFailed");
    RESOURCES_TYPE(api, BRIDGE_STRUCT, "ideatek:ResourceVariable",
        ideatek_resource_variable_t);
}

void declare_types(const bridge_api_t *api)
{
    declare_files(api);
    declare_loads(api);
    declare_rest(api);
    RESOURCES_SINGLE(api, "ideatek_resources_files:Settings", res_settings_t);
}

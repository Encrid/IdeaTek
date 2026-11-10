/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "space_inside.h"

static void declare_fields(const bridge_api_t *api)
{
    SPACE_TYPE(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    SPACE_TYPE(api, "ideatek:ideatek_vec3_t", ideatek_vec3_t);
    SPACE_TYPE(api, "ideatek:ideatek_quat_t", ideatek_quat_t);
    SPACE_TYPE(api, "ideatek:ideatek_position_t", ideatek_position_t);
    SPACE_ENUM(api, "ideatek:ideatek_keep_t", ideatek_keep_t);
    SPACE_ENUM(api, "ideatek:ideatek_orphan_t", ideatek_orphan_t);
    SPACE_TYPE(api, "ideatek:ideatek_world_transform_t",
        ideatek_world_transform_t);
}

void declare_types(const bridge_api_t *api)
{
    declare_fields(api);
    SPACE_TYPE(api, "ideatek:Transform", ideatek_transform_t);
    SPACE_TYPE(api, "ideatek:Parent", ideatek_parent_t);
    SPACE_TYPE(api, "ideatek:WorldTransform", ideatek_world_transform_t);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek:Children",
        ideatek_children_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_KEEP);
    BRIDGE_NATIVE_DECLARE(api, BRIDGE_STRUCT, "ideatek:Sibling",
        ideatek_sibling_t, BRIDGE_STRIP, BRIDGE_TABLE,
        BRIDGE_ON_DESTROY_KEEP);
    SPACE_TYPE(api, "ideatek:PreviousWorldTransform",
        ideatek_previous_world_transform_t);
    SPACE_TAG(api, "ideatek:Interpolated");
    SPACE_TAG(api, "ideatek:Teleported");
    BRIDGE_NATIVE_COMPONENT(api, "ideatek:Tick", ideatek_tick_t, BRIDGE_STRIP,
        BRIDGE_SINGLETON, BRIDGE_ON_DESTROY_REMOVE);
}

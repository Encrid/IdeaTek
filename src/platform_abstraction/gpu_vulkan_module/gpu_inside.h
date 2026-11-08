/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef GPU_INSIDE_H
    #define GPU_INSIDE_H

    #include "bridge_native.h"
    #include "ideatek_gpu.h"
    #include "ideatek_gpu_vulkan.h"

    #define GPU_SETTINGS_FIELDS {         uint32_t frames_in_flight;         uint32_t image_count;         bool validation;     }

    BRIDGE_SCHEMA_STRUCT_TYPE(gpu_settings_t, GPU_SETTINGS_FIELDS);
    BRIDGE_SCHEMA_STRUCT_DESC(gpu_settings_t, GPU_SETTINGS_FIELDS);

#endif

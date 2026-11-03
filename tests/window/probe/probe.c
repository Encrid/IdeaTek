/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "bridge_native.h"
#include "ideatek_window.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define TAG(api, name) BRIDGE_NATIVE_DECLARE_TAG(api, name, BRIDGE_STRIP, \
    BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_place_t", ideatek_place_t);
    ENUM(api, "ideatek:ideatek_window_mode_t", ideatek_window_mode_t);
    ENUM(api, "ideatek:ideatek_surface_t", ideatek_surface_t);
    ENUM(api, "ideatek:ideatek_flash_t", ideatek_flash_t);
    ENUM(api, "ideatek:ideatek_orientation_t", ideatek_orientation_t);
    ENUM(api, "ideatek:ideatek_progress_t", ideatek_progress_t);
    ENUM(api, "ideatek:ideatek_region_t", ideatek_region_t);
    ENUM(api, "ideatek:ideatek_drop_kind_t", ideatek_drop_kind_t);
}

[[maybe_unused]] static uint32_t declare_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:WindowRequest", ideatek_window_request_t));
}

[[maybe_unused]] static uint32_t declare_main(const bridge_api_t *api)
{
    return (TAG(api, "ideatek:MainWindow"));
}

[[maybe_unused]] static uint32_t declare_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:WindowState", ideatek_window_state_t));
}

[[maybe_unused]] static uint32_t declare_display(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Display", ideatek_display_t));
}

[[maybe_unused]] static uint32_t declare_video_mode(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:VideoMode", ideatek_video_mode_t));
}

[[maybe_unused]] static uint32_t declare_primary(const bridge_api_t *api)
{
    return (TAG(api, "ideatek:PrimaryDisplay"));
}

[[maybe_unused]] static uint32_t declare_region(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:WindowRegion", ideatek_window_region_t));
}

[[maybe_unused]] static uint32_t declare_drop(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Drop", ideatek_drop_t));
}

[[maybe_unused]] static void declare_module(const bridge_api_t *api)
{
    declare_request(api);
    declare_main(api);
    declare_state(api);
    declare_display(api);
    declare_video_mode(api);
    declare_primary(api);
    declare_region(api);
    declare_drop(api);
}

[[maybe_unused]] static void open_window(const bridge_api_t *api)
{
    ideatek_window_request_t request = {
        .title = "IdeaTek", .width = 1280, .height = 720,
        .place = IDEATEK_PLACE_CENTER, .resizable = true, .visible = true
    };
    uint64_t id = api->spawn(api);
    ideatek_window_region_t region = { .window = { id }, .width = 1280,
        .height = 32, .role = IDEATEK_REGION_DRAG };

    declare_fields(api);
    BRIDGE_NATIVE_ATTACH(api, id, declare_request(api), request);
    api->attach(api, id, declare_main(api), BRIDGE_ZEROED);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_region(api), region);
}

[[maybe_unused]] static void report(const bridge_api_t *api)
{
    ideatek_window_state_t state = { .width = 1280, .height = 720,
        .display_scale = 1.0F, .sdr_white_level = 1.0F,
        .hdr_headroom = 1.0F };
    ideatek_display_t display = { .name = "probe", .width = 1920,
        .height = 1080, .content_scale = 1.0F };
    ideatek_video_mode_t mode = { .width = 1920, .height = 1080,
        .pixel_density = 1.0F, .refresh_numerator = 60,
        .refresh_denominator = 1 };
    uint64_t id = api->spawn(api);
    static const char PATH[] = "probe.txt";
    uint32_t at = api->alloc(api, sizeof(PATH) - 1);
    ideatek_drop_t drop = { .window = { id },
        .data = { .at = at, .len = sizeof(PATH) - 1 },
        .kind = IDEATEK_DROP_FILE };

    memcpy(api->shared_base + at, PATH, sizeof(PATH) - 1);
    declare_fields(api);
    declare_request(api);
    declare_main(api);
    declare_region(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_drop(api), drop);
    BRIDGE_NATIVE_ATTACH(api, id, declare_display(api), display);
    BRIDGE_NATIVE_ATTACH(api, id, declare_video_mode(api), mode);
    BRIDGE_NATIVE_ATTACH(api, id, declare_state(api), state);
    api->attach(api, id, declare_primary(api), BRIDGE_ZEROED);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_request(api);
    declare_fields(api);
#elif defined(ROLE_GAME)
    open_window(api);
#elif defined(ROLE_WINDOW)
    report(api);
#else
    declare_fields(api);
    declare_module(api);
#endif
    return (0);
}

BRIDGE_NATIVE(shutdown)
{
    return (0);
}

BRIDGE_NATIVE(reload)
{
    return (1);
}

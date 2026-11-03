/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "bridge_native.h"
#include "ideatek_clipboard.h"
#include "ideatek_window.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_fields(const bridge_api_t *api)
{
    STRUCT(api, "ideatek:ideatek_entity_t", ideatek_entity_t);
    ENUM(api, "ideatek:ideatek_window_mode_t", ideatek_window_mode_t);
    ENUM(api, "ideatek:ideatek_display_target_t", ideatek_display_target_t);
    ENUM(api, "ideatek:ideatek_surface_t", ideatek_surface_t);
    ENUM(api, "ideatek:ideatek_drop_kind_t", ideatek_drop_kind_t);
}

[[maybe_unused]] static uint32_t declare_request(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:WindowRequest", ideatek_window_request_t));
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

[[maybe_unused]] static uint32_t declare_drop(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Drop", ideatek_drop_t));
}

[[maybe_unused]] static uint32_t declare_clipboard(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:ClipboardRequest",
        ideatek_clipboard_request_t));
}

[[maybe_unused]] static uint32_t declare_paste(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:Paste", ideatek_paste_t));
}

[[maybe_unused]] static void declare_module(const bridge_api_t *api)
{
    declare_request(api);
    declare_state(api);
    declare_display(api);
    declare_video_mode(api);
    declare_drop(api);
    declare_clipboard(api);
    declare_paste(api);
}

[[maybe_unused]] static bridge_text_t put(const bridge_api_t *api,
    const char *bytes, uint32_t len)
{
    uint32_t at = api->alloc(api, len);

    memcpy(api->shared_base + at, bytes, len);
    return ((bridge_text_t){ .at = at, .len = len });
}

[[maybe_unused]] static void open_window(const bridge_api_t *api)
{
    ideatek_window_request_t request = {
        .title = "IdeaTek", .width = 1280, .height = 720,
        .mode = IDEATEK_WINDOW_MODE_WINDOWED,
        .target = IDEATEK_DISPLAY_TARGET_DEFAULT
    };
    ideatek_clipboard_request_t clipboard = { .text = put(api, "probe", 5) };

    declare_fields(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_request(api), request);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_clipboard(api),
        clipboard);
}

[[maybe_unused]] static void report(const bridge_api_t *api)
{
    ideatek_window_state_t state = { .width = 1280, .height = 720,
        .display_scale = 1.0F, .sdr_white_level = 1.0F,
        .hdr_headroom = 1.0F };
    ideatek_display_t display = { .name = "probe", .width = 1920,
        .height = 1080, .content_scale = 1.0F, .is_primary = true };
    ideatek_video_mode_t mode = { .width = 1920, .height = 1080,
        .pixel_density = 1.0F, .refresh_numerator = 60,
        .refresh_denominator = 1 };
    uint64_t id = api->spawn(api);
    ideatek_drop_t drop = { .window = { id },
        .data = put(api, "probe.txt", 9), .kind = IDEATEK_DROP_FILE };
    ideatek_paste_t paste = { .text = put(api, "probe", 5) };

    declare_fields(api);
    declare_request(api);
    declare_clipboard(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_drop(api), drop);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_paste(api), paste);
    BRIDGE_NATIVE_ATTACH(api, id, declare_display(api), display);
    BRIDGE_NATIVE_ATTACH(api, id, declare_video_mode(api), mode);
    BRIDGE_NATIVE_ATTACH(api, id, declare_state(api), state);
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

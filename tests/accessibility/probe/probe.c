/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "bridge_native.h"
#include "ideatek_accessibility.h"

[[gnu::section(MANIFEST_SECTION), gnu::used, gnu::retain]]
static const char WHO[] = MANIFEST;

#define ENUM(api, name, T) BRIDGE_NATIVE_DECLARE(api, BRIDGE_ENUM, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)
#define STRUCT(api, name, T) BRIDGE_NATIVE_COMPONENT(api, name, T, \
    BRIDGE_STRIP, BRIDGE_TABLE, BRIDGE_ON_DESTROY_REMOVE)

[[maybe_unused]] static void declare_theme(const bridge_api_t *api)
{
    ENUM(api, "ideatek:ideatek_theme_t", ideatek_theme_t);
}

[[maybe_unused]] static uint32_t declare_state(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:AccessibilityState",
        ideatek_accessibility_state_t));
}

[[maybe_unused]] static uint32_t declare_speech(const bridge_api_t *api)
{
    return (STRUCT(api, "ideatek:SpeechRequest", ideatek_speech_request_t));
}

[[maybe_unused]] static void ask(const bridge_api_t *api)
{
    uint32_t at = api->alloc(api, 5);
    ideatek_speech_request_t speech = {
        .text = { .at = at, .len = 5 }, .interrupt = true };

    memcpy(api->shared_base + at, "probe", 5);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_speech(api), speech);
}

[[maybe_unused]] static void report(const bridge_api_t *api)
{
    ideatek_accessibility_state_t state = { .theme = IDEATEK_THEME_DARK,
        .increase_contrast = -1, .reduce_animation = 0,
        .reduce_transparency = 1, .screen_reader = -1, .speech = true };

    declare_theme(api);
    declare_speech(api);
    BRIDGE_NATIVE_ATTACH(api, api->spawn(api), declare_state(api), state);
}

BRIDGE_NATIVE(init)
{
#if defined(ROLE_LATE)
    declare_state(api);
    declare_theme(api);
#elif defined(ROLE_GAME)
    ask(api);
#elif defined(ROLE_ACCESSIBILITY)
    report(api);
#elif defined(ROLE_READER)
    declare_theme(api);
    declare_state(api);
#else
    declare_theme(api);
    declare_state(api);
    declare_speech(api);
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

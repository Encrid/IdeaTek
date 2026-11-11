/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdlib.h>
#include <string.h>

#include "files_inside.h"

#if defined(_WIN32)
static const char *const DATA_VARIABLE = "APPDATA";
static const char *const DATA_BELOW = "";
#elif defined(__APPLE__)
static const char *const DATA_VARIABLE = "HOME";
static const char *const DATA_BELOW = "Library/Application Support";
#else
static const char *const DATA_VARIABLE = "XDG_DATA_HOME";
static const char *const DATA_BELOW = "";
#endif

static const char RANGE[] = "2 to 1073741824, an even number";

static bool copy_setting(const bridge_api_t *api, const char *name,
    bridge_text_t text, char *out)
{
    const bridge_header_t *h = bridge_native_get_header(api);
    bool inside = (text.at >= h->arena_lo) & (text.at <= h->arena_hi)
        & (text.len <= h->arena_hi - text.at) & (text.len < FILES_PATH);
    uint32_t len = inside * text.len;
    bool ok;

    memcpy(out, api->shared_base + text.at, len);
    out[len] = '\0';
    ok = inside & path_is_allowed(out);
    if (ok)
        return (true);
    bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %.*s is outside the path rule",
        name, (int)len, out);
    return (false);
}

static void find_content(const bridge_api_t *api, const char *folder)
{
    char *program = os_get_program_path();

    files_module.content[0] = '\0';
    if (program)
        path_parent(program);
    files_module.has_content = program
        && path_join(files_module.content, FILES_PATH, program, folder)
        && os_path_entry(files_module.content) == 3;
    if (!files_module.has_content)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no content folder: %s",
            files_module.content);
    free(program);
}

static void find_user(const bridge_api_t *api, const char *folder)
{
    char base[FILES_PATH];
    char *value = os_env_get(DATA_VARIABLE);
    bool found = value && path_join(base, sizeof(base), value, DATA_BELOW);

    free(value);
#if !defined(_WIN32) && !defined(__APPLE__)
    value = found ? nullptr : os_env_get("HOME");
    found |= value && path_join(base, sizeof(base), value, ".local/share");
    free(value);
#endif
    files_module.user[0] = '\0';
    files_module.has_user = found
        && path_join(files_module.user, FILES_PATH, base, folder)
        && os_folder_make(files_module.user) == 0;
    if (!files_module.has_user)
        bridge_native_say(api, BRIDGE_LOG_WARN, "no user folder: %s",
            found ? files_module.user : "no variable of the system");
}

static bool check_bytes(const bridge_api_t *api, uint32_t bytes)
{
    bool ok = (bytes >= 2) & (bytes <= 1073741824) & (bytes % 2 == 0);

    if (!ok)
        bridge_native_say(api, BRIDGE_LOG_ERR, "%s: %u is outside %s",
            "read_bytes", bytes, RANGE);
    return (ok);
}

bool start_files(const bridge_api_t *api)
{
    const files_settings_t *s = bridge_native_get_single(api, SETTINGS);
    char user_folder[FILES_PATH];
    char content_folder[FILES_PATH];
    uint8_t *base = api->shared_base;
    bool ok = check_bytes(api, s->read_bytes);

    ok &= take_rounds(api, s);
    files_module.started = true;
    ok &= copy_setting(api, "user_folder", s->user_folder, user_folder);
    ok &= copy_setting(api, "content_folder", s->content_folder,
        content_folder);
    files_module.broken = !ok;
    if (!ok)
        return (false);
    take_halves(api, s->read_bytes);
    files_module.content = (char *)(base + api->alloc(api, FILES_PATH));
    files_module.user = (char *)(base + api->alloc(api, FILES_PATH));
    find_content(api, content_folder);
    find_user(api, user_folder);
    return (true);
}

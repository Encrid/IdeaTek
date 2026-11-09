/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_pick.h"

bool has_extension(const res_loader_view_t *view,
    const res_text_t *extension)
{
    const char *at = view->extensions.at;
    const char *end = at + view->extensions.len;
    const char *nl;
    res_text_t line;
    bool found = false;

    while (at < end) {
        nl = memchr(at, '\n', (size_t)(end - at));
        line.at = at;
        line.len = (uint32_t)((nl ? nl : end) - at);
        found |= same_text(&line, extension);
        at = (nl ? nl : end) + 1;
    }
    return (found);
}

bool fits_magic(const res_loader_view_t *v, const res_pick_ask_t *ask)
{
    bool short_enough = v->magic.len <= ask->bytes.len;

    return (ask->has_bytes && short_enough
        && !memcmp(v->magic.at, ask->bytes.at, v->magic.len));
}

bool fits_extension(const res_loader_view_t *v, const res_pick_ask_t *ask)
{
    bool typed = !ask->type.len || same_text(&v->type, &ask->type);

    return (typed && has_extension(v, &ask->extension));
}

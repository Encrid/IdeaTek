/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_pick.h"

static void pick_named(const res_pick_ask_t *ask, res_pick_t *out)
{
    uint32_t hits = 0;
    uint32_t pick = 0;
    bool typed = ask->type.len > 0;
    bool same;
    bool wrong;

    for (uint32_t i = 0; i < ask->count; i++) {
        same = same_text(&ask->views[i].name, &ask->name);
        hits += same;
        pick = same ? i : pick;
    }
    wrong = hits == 1 && typed && !same_text(&ask->views[pick].type,
        &ask->type);
    out->loader = hits == 1 && !wrong ? ask->views[pick].index : RES_NONE;
    out->error = hits == 0 ? IDEATEK_RESOURCE_ERROR_NO_LOADER
        : IDEATEK_RESOURCE_ERROR_TWO_LOADERS;
    out->error = wrong ? IDEATEK_RESOURCE_ERROR_WRONG_TYPE : out->error;
}

static bool pick_typed(const res_pick_ask_t *ask, res_pick_t *out)
{
    uint32_t hits = 0;
    uint32_t pick = 0;
    bool match;

    for (uint32_t i = 0; i < ask->count; i++) {
        match = same_text(&ask->views[i].type, &ask->type);
        hits += match;
        pick = match ? i : pick;
    }
    out->loader = hits == 1 ? ask->views[pick].index : RES_NONE;
    return ((ask->type.len > 0) & (hits == 1));
}

static void tally_result(const res_pick_ask_t *ask, const res_tally_t *tally,
    res_pick_t *out)
{
    uint32_t pick = tally->hits == 1 ? tally->by_ext : tally->by_magic;
    bool found = (tally->hits == 1) | (tally->magic == 1);

    out->needs_bytes = (tally->hits >= 2) & !ask->has_bytes;
    out->loader = found ? ask->views[pick].index : RES_NONE;
    out->error = tally->hits == 0 ? IDEATEK_RESOURCE_ERROR_NO_LOADER
        : IDEATEK_RESOURCE_ERROR_TWO_LOADERS;
}

static void pick_by_extension(const res_pick_ask_t *ask, res_pick_t *out)
{
    res_tally_t tally = { 0, 0, 0, 0 };
    bool fit;

    for (uint32_t i = 0; i < ask->count; i++) {
        fit = fits_extension(&ask->views[i], ask);
        tally.hits += fit;
        tally.by_ext = fit ? i : tally.by_ext;
        fit = fit && fits_magic(&ask->views[i], ask);
        tally.magic += fit;
        tally.by_magic = fit ? i : tally.by_magic;
    }
    tally_result(ask, &tally, out);
}

void pick_loader(const res_pick_ask_t *ask, res_pick_t *out)
{
    bool found;

    out->loader = RES_NONE;
    out->error = IDEATEK_RESOURCE_ERROR_NONE;
    out->needs_bytes = false;
    if (ask->name.len)
        pick_named(ask, out);
    else if (!pick_typed(ask, out))
        pick_by_extension(ask, out);
    found = (out->loader != RES_NONE) | out->needs_bytes;
    out->error = found ? IDEATEK_RESOURCE_ERROR_NONE : out->error;
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "resources_fold.h"

void fold_start(res_fold_t *fold)
{
    *fold = (res_fold_t){ .error = IDEATEK_RESOURCE_ERROR_NONE };
}

void fold_add(res_fold_t *fold, ideatek_load_state_t state,
    ideatek_resource_error_t error)
{
    bool keep = fold->error != IDEATEK_RESOURCE_ERROR_NONE;

    fold->count++;
    fold->loaded += state == IDEATEK_LOAD_STATE_LOADED;
    fold->failed += state == IDEATEK_LOAD_STATE_FAILED;
    fold->error = (ideatek_resource_error_t)(keep * fold->error
        + !keep * error);
}

ideatek_load_state_t fold_state(const res_fold_t *fold)
{
    bool failed = fold->failed > 0;
    bool loaded = !failed & (fold->loaded == fold->count);
    bool loading = !failed & !loaded;

    return ((ideatek_load_state_t)(failed * IDEATEK_LOAD_STATE_FAILED
        + loaded * IDEATEK_LOAD_STATE_LOADED
        + loading * IDEATEK_LOAD_STATE_LOADING));
}

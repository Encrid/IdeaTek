/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RESOURCES_FOLD_H
    #define RESOURCES_FOLD_H

    #include <stdint.h>

    #include "ideatek_resources.h"

typedef struct {
    uint32_t count;
    uint32_t loaded;
    uint32_t failed;
    ideatek_resource_error_t error;
} res_fold_t;

void fold_start(res_fold_t *fold);
void fold_add(res_fold_t *fold, ideatek_load_state_t state,
    ideatek_resource_error_t error);
ideatek_load_state_t fold_state(const res_fold_t *fold);

#endif

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef RESOURCES_META_H
    #define RESOURCES_META_H

    #include <stdint.h>

    #include "resources_paths.h"

typedef enum : uint8_t {
    META_LOAD = 0,
    META_PROCESS = 1,
    META_IGNORE = 2
} res_action_t;

typedef struct {
    res_action_t action;
    res_text_t loader;
    res_text_t settings;
    uint32_t line;
} res_meta_t;

void trim(res_text_t *line);
int read_word(res_text_t line, res_text_t *word);
bool read_action(const res_text_t *word, res_meta_t *meta);
res_text_t line_at(const res_text_t *bytes, uint32_t at, uint32_t *end);
void meta_start(const res_text_t *bytes, res_meta_t *meta);
void take_rest(const res_text_t *bytes, res_meta_t *meta, uint32_t end);
bool meta_read(const res_text_t *bytes, res_meta_t *meta);

#endif

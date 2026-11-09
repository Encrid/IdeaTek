/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_meta.h"

static int read_key(res_text_t line, res_meta_t *meta, bool *seen)
{
    res_text_t word;
    int key;
    bool header;

    trim(&line);
    header = line.len == 10 && !memcmp(line.at, "[settings]", 10);
    if (header)
        return (2);
    if (!line.len || line.at[0] == '#')
        return (1);
    key = read_word(line, &word);
    if (key < 0 || seen[key])
        return (0);
    seen[key] = true;
    if (key == 0)
        return (read_action(&word, meta));
    meta->loader = word;
    return ((word.len > 0) & (word.len < RES_NAME));
}

bool meta_read(const res_text_t *bytes, res_meta_t *meta)
{
    bool seen[2] = { false, false };
    uint32_t at = 0;
    uint32_t end;
    res_text_t line;
    int verdict = 1;

    meta_start(bytes, meta);
    while (at < bytes->len && verdict != 2) {
        line = line_at(bytes, at, &end);
        verdict = read_key(line, meta, seen);
        if (!verdict)
            return (false);
        if (verdict == 2)
            take_rest(bytes, meta, end);
        at = end + 1;
        meta->line++;
    }
    meta->line = 0;
    return (true);
}

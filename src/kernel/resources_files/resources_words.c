/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "resources_meta.h"

static const char *const KEYS[2] = { "action", "loader" };
static const char *const ACTIONS[3] = { "load", "process", "ignore" };

void trim(res_text_t *line)
{
    while (line->len && strchr(" \t", line->at[0])) {
        line->at++;
        line->len--;
    }
    while (line->len && strchr(" \t\r", line->at[line->len - 1]))
        line->len--;
}

static int find_key(res_text_t *line)
{
    size_t len;
    bool fits;

    for (int k = 0; k < 2; k++) {
        len = strlen(KEYS[k]);
        fits = line->len > len && !memcmp(line->at, KEYS[k], len)
            && strchr(" \t=", line->at[len]);
        if (!fits)
            continue;
        line->at += len;
        line->len -= (uint32_t)len;
        return (k);
    }
    return (-1);
}

static bool read_quoted(res_text_t line, res_text_t *word)
{
    const char *quote = line.len >= 2 && line.at[0] == '"'
        ? memchr(line.at + 1, '"', line.len - 1) : nullptr;

    word->at = line.at + 1;
    word->len = quote ? (uint32_t)(quote - line.at - 1) : 0;
    return (quote && quote == line.at + line.len - 1);
}

int read_word(res_text_t line, res_text_t *word)
{
    int key = find_key(&line);

    if (key < 0)
        return (-1);
    trim(&line);
    if (!line.len || line.at[0] != '=')
        return (-1);
    line.at++;
    line.len--;
    trim(&line);
    return (read_quoted(line, word) ? key : -1);
}

bool read_action(const res_text_t *word, res_meta_t *meta)
{
    int action = -1;
    size_t len;

    for (int a = 0; a < 3; a++) {
        len = strlen(ACTIONS[a]);
        action = word->len == len && !memcmp(word->at, ACTIONS[a], len) ? a
            : action;
    }
    meta->action = (res_action_t)(action < 0 ? 0 : action);
    return (action >= 0);
}

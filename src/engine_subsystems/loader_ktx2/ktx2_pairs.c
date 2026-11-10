/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "ktx2_pure.h"

static ktx2_bytes_t trim(ktx2_bytes_t text)
{
    while (text.len && (text.at[0] == ' ' || text.at[0] == '\t')) {
        text.at++;
        text.len--;
    }
    while (text.len && memchr(" \t\r", text.at[text.len - 1], 3))
        text.len--;
    return (text);
}

static ktx2_bytes_t take_line(ktx2_bytes_t *rest)
{
    const uint8_t *end = memchr(rest->at, '\n', rest->len);
    ktx2_bytes_t line = { rest->at, end ? (uint64_t)(end - rest->at)
        : rest->len };

    rest->at += line.len + (end != nullptr);
    rest->len -= line.len + (end != nullptr);
    return (trim(line));
}

static void split(ktx2_bytes_t line, ktx2_pair_t *pair)
{
    const uint8_t *equal = memchr(line.at, '=', line.len);
    uint64_t left = equal ? (uint64_t)(equal - line.at) : line.len;

    pair->key = trim((ktx2_bytes_t){ line.at, left });
    pair->value = trim((ktx2_bytes_t){ line.at + left + (equal != nullptr),
        line.len - left - (equal != nullptr) });
}

bool ktx2_next_pair(ktx2_bytes_t *rest, ktx2_pair_t *pair)
{
    ktx2_bytes_t line;

    while (rest->len) {
        line = take_line(rest);
        if (line.len && line.at[0] != '#') {
            split(line, pair);
            return (true);
        }
    }
    return (false);
}

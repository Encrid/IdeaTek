/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <string.h>

#include "gltf_inside.h"

uint64_t hash_text(const char *text, size_t len, uint64_t seed)
{
    uint64_t h = seed ? seed : 0xcbf29ce484222325ull;

    for (size_t i = 0; i < len; i++) {
        h ^= (uint8_t)text[i];
        h *= 0x100000001b3ull;
    }
    return (h);
}

static int hex_of(char c)
{
    if (c >= '0' && c <= '9')
        return (c - '0');
    if (c >= 'a' && c <= 'f')
        return (c - 'a' + 10);
    if (c >= 'A' && c <= 'F')
        return (c - 'A' + 10);
    return (-1);
}

static uint32_t copy_uri(const char *uri, char *out, uint32_t at)
{
    int high;
    int low;

    while (*uri && at < PATH_ROOM - 1) {
        high = uri[0] == '%' ? hex_of(uri[1]) : -1;
        low = high >= 0 ? hex_of(uri[2]) : -1;
        out[at++] = low >= 0 ? (char)(high * 16 + low) : *uri;
        uri += low >= 0 ? 3 : 1;
    }
    out[at] = '\0';
    return (at);
}

uint32_t join_path(const char *folder, uint32_t folder_len, const char *uri,
    char *out)
{
    memcpy(out, folder, folder_len);
    return (copy_uri(uri, out, folder_len));
}

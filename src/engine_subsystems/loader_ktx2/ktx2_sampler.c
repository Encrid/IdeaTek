/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "ktx2_pure.h"

static const char *const WORDS[2][3] = {
    { "nearest", "linear", "" },
    { "repeat", "mirrored_repeat", "clamp_to_edge" },
};

static const struct {
    const char *name;
    uint32_t words;
    size_t offset;
} KEYS[7] = {
    { "min_filter", 0, offsetof(ideatek_sampler_t, min_filter) },
    { "mag_filter", 0, offsetof(ideatek_sampler_t, mag_filter) },
    { "mip_filter", 0, offsetof(ideatek_sampler_t, mip_filter) },
    { "wrap_u", 1, offsetof(ideatek_sampler_t, wrap_u) },
    { "wrap_v", 1, offsetof(ideatek_sampler_t, wrap_v) },
    { "wrap_w", 1, offsetof(ideatek_sampler_t, wrap_w) },
    { "max_anisotropy", 2, offsetof(ideatek_sampler_t, max_anisotropy) },
};

static const ideatek_sampler_t DEFAULTS = {
    .max_anisotropy = 1.0f,
    .min_filter = IDEATEK_FILTER_LINEAR,
    .mag_filter = IDEATEK_FILTER_LINEAR,
    .mip_filter = IDEATEK_FILTER_LINEAR,
};

static bool read_word(ktx2_bytes_t value, uint32_t words, uint8_t *pick)
{
    bool quoted = value.len >= 2 && value.at[0] == '"'
        && value.at[value.len - 1] == '"';
    ktx2_bytes_t inner = quoted ? (ktx2_bytes_t){ value.at + 1,
        value.len - 2 } : value;

    for (uint8_t k = 0; quoted && k < 3; k++) {
        if (WORDS[words][k][0] && ktx2_same(inner, WORDS[words][k])) {
            *pick = k;
            return (true);
        }
    }
    return (false);
}

static bool read_number(ktx2_bytes_t value, float *number)
{
    char text[32] = "";
    char *end = nullptr;

    if (value.len == 0 || value.len >= sizeof(text))
        return (false);
    memcpy(text, value.at, value.len);
    *number = strtof(text, &end);
    return (*end == '\0' && *number >= 1.0f && *number <= 16.0f);
}

static uint32_t apply_pair(const ktx2_pair_t *pair, ideatek_sampler_t *s)
{
    uint8_t *field;
    uint8_t pick = 0;
    float number = 0.0f;

    for (uint32_t k = 0; k < 7; k++) {
        if (!ktx2_same(pair->key, KEYS[k].name))
            continue;
        field = (uint8_t *)s + KEYS[k].offset;
        if (KEYS[k].words == 2 && read_number(pair->value, &number))
            memcpy(field, &number, sizeof(number));
        else if (KEYS[k].words < 2 && read_word(pair->value, KEYS[k].words,
            &pick))
            *field = pick;
        else
            return (KTX2_BAD_VALUE);
        return (KTX2_APPLIED);
    }
    return (KTX2_UNKNOWN_KEY);
}

void ktx2_read_sampler(ktx2_bytes_t text, ktx2_sampler_read_t *out)
{
    const uint8_t *start = text.at;
    ktx2_pair_t pair;
    uint32_t kind;

    *out = (ktx2_sampler_read_t){ .sampler = DEFAULTS };
    while (ktx2_next_pair(&text, &pair)) {
        kind = apply_pair(&pair, &out->sampler);
        if (kind == KTX2_APPLIED || out->count == KTX2_MAX_FAULTS)
            continue;
        out->faults[out->count++] = (ktx2_fault_t){ kind,
            (uint32_t)(pair.key.at - start), (uint32_t)pair.key.len };
    }
}

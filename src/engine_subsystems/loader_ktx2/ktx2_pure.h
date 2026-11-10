/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef KTX2_PURE_H
    #define KTX2_PURE_H

    #include <stdint.h>

    #include "ideatek_rendering.h"

constexpr uint32_t KTX2_HEAD = 80;
constexpr uint32_t KTX2_LEVEL = 24;
constexpr uint32_t KTX2_MAX_LEVELS = 32;
constexpr uint32_t KTX2_DFD_MIN = 28;
constexpr uint32_t KTX2_MAX_FAULTS = 8;

constexpr uint32_t KTX2_UNKNOWN_KEY = 0;
constexpr uint32_t KTX2_BAD_VALUE = 1;
constexpr uint32_t KTX2_APPLIED = 2;

typedef struct {
    const uint8_t *at;
    uint64_t len;
} ktx2_bytes_t;

typedef struct {
    uint64_t at;
    uint64_t len;
} ktx2_range_t;

typedef struct {
    uint32_t format;
    uint32_t width;
    uint32_t height;
    uint32_t depth;
    uint32_t layers;
    uint32_t faces;
    uint32_t levels;
    uint32_t scheme;
    uint32_t dfd_at;
    uint32_t dfd_len;
    uint32_t block[3];
    uint32_t block_bytes;
    uint64_t texel_bytes;
    ktx2_range_t level[KTX2_MAX_LEVELS];
} ktx2_file_t;

typedef struct {
    uint32_t kind;
    uint32_t at;
    uint32_t len;
} ktx2_fault_t;

typedef struct {
    ideatek_sampler_t sampler;
    ktx2_fault_t faults[KTX2_MAX_FAULTS];
    uint32_t count;
} ktx2_sampler_read_t;

typedef struct {
    ktx2_bytes_t key;
    ktx2_bytes_t value;
} ktx2_pair_t;

uint32_t ktx2_u32(ktx2_bytes_t file, uint64_t at);
uint64_t ktx2_u64(ktx2_bytes_t file, uint64_t at);
bool ktx2_same(ktx2_bytes_t text, const char *word);

const char *ktx2_read_head(ktx2_bytes_t file, ktx2_file_t *f);
const char *ktx2_check_shape(const ktx2_file_t *f);
const char *ktx2_read_block(ktx2_bytes_t file, ktx2_file_t *f);
const char *ktx2_check_levels(ktx2_bytes_t file, ktx2_file_t *f);
const char *ktx2_read(ktx2_bytes_t file, ktx2_file_t *f);

bool ktx2_level_bytes(const ktx2_file_t *f, uint32_t level, uint64_t *bytes);
void ktx2_copy(ktx2_bytes_t file, const ktx2_file_t *f, uint8_t *to);
ideatek_dimension_t ktx2_dimension(const ktx2_file_t *f);

bool ktx2_next_pair(ktx2_bytes_t *rest, ktx2_pair_t *pair);
void ktx2_read_sampler(ktx2_bytes_t text, ktx2_sampler_read_t *out);

#endif

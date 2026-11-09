/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef AUDIO_VALUES_H
    #define AUDIO_VALUES_H

    #include <stdint.h>

    #include "ideatek_audio.h"

constexpr uint32_t AUDIO_RATE_LEAST = 8000;
constexpr uint32_t AUDIO_RATE_MOST = 384000;
constexpr uint32_t AUDIO_AHEAD_MOST = 65536;
constexpr uint32_t AUDIO_CHANNELS_MOST = 8;
constexpr uint32_t AUDIO_FRAMES_LEAST = 64;
constexpr uint32_t AUDIO_FRAMES_MOST = 65536;

typedef struct {
    const char *field;
    uint64_t value;
    const char *range;
} audio_fault_t;

typedef struct {
    const char *field;
    uint64_t least;
    uint64_t most;
    const char *range;
} audio_range_t;

bool check_request(const ideatek_audio_request_t *r, audio_fault_t *fault);
bool check_frames(uint32_t frames);
uint32_t frames_wanted(uint32_t ahead, uint32_t queued);
uint32_t frames_over(uint32_t frames, uint32_t ahead);

#endif

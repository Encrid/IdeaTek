/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include "audio_values.h"

static const audio_range_t RANGES[5] = {
    { "direction", 0, 1, "0 to 1" },
    { "target", 0, 1, "0 to 1" },
    { "rate", AUDIO_RATE_LEAST, AUDIO_RATE_MOST, "8000 to 384000" },
    { "ahead", 1, AUDIO_AHEAD_MOST, "1 to 65536" },
    { "channels", 1, AUDIO_CHANNELS_MOST, "1 to 8" },
};

bool check_request(const ideatek_audio_request_t *r, audio_fault_t *fault)
{
    const uint64_t values[5] = { r->direction, r->target, r->rate, r->ahead,
        r->channels };

    for (size_t i = 0; i < 5; i++) {
        if (values[i] >= RANGES[i].least && values[i] <= RANGES[i].most)
            continue;
        fault->field = RANGES[i].field;
        fault->value = values[i];
        fault->range = RANGES[i].range;
        return (false);
    }
    return (true);
}

bool check_frames(uint32_t frames)
{
    return ((frames == 0) | ((frames >= AUDIO_FRAMES_LEAST)
        & (frames <= AUDIO_FRAMES_MOST)));
}

uint32_t frames_wanted(uint32_t ahead, uint32_t queued)
{
    bool over = queued >= ahead;

    return (!over * (ahead - queued * !over));
}

uint32_t frames_over(uint32_t frames, uint32_t ahead)
{
    bool over = frames > ahead;

    return (over * (frames - ahead * over));
}

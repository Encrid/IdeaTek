/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "audio_values.h"

int main(int argc, char **argv)
{
    ideatek_audio_request_t r = {};
    audio_fault_t fault = { "none", 0, "none" };

    if (argc < 3)
        return (2);
    if (!strcmp(argv[1], "request") && argc >= 7) {
        r.direction = (ideatek_audio_direction_t)atoi(argv[2]);
        r.target = (ideatek_audio_target_t)atoi(argv[3]);
        r.rate = (uint32_t)strtoul(argv[4], nullptr, 0);
        r.ahead = (uint32_t)strtoul(argv[5], nullptr, 0);
        r.channels = (uint8_t)atoi(argv[6]);
        printf("request %d %s %llu [%s]\n", check_request(&r, &fault),
            fault.field, (unsigned long long)fault.value, fault.range);
    }
    if (!strcmp(argv[1], "frames"))
        printf("frames %d\n", check_frames((uint32_t)strtoul(argv[2],
            nullptr, 0)));
    if (!strcmp(argv[1], "wanted") && argc >= 4)
        printf("wanted %u\n", frames_wanted((uint32_t)atoi(argv[2]),
            (uint32_t)atoi(argv[3])));
    if (!strcmp(argv[1], "over") && argc >= 4)
        printf("over %u\n", frames_over((uint32_t)atoi(argv[2]),
            (uint32_t)atoi(argv[3])));
    return (0);
}

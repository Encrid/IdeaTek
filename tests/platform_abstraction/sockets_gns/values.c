/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sockets_values.h"

static uint32_t unhex(const char *text, uint8_t *out, uint32_t size)
{
    uint32_t len = (uint32_t)strlen(text) / 2;
    unsigned value;

    len = len < size ? len : size;
    for (uint32_t i = 0; i < len; i++) {
        sscanf(text + 2 * i, "%2x", &value);
        out[i] = (uint8_t)value;
    }
    return (len);
}

static int words(int argc, char **argv)
{
    const char *const *table = ALLOW_WORDS;
    uint32_t count = ALLOW_COUNT;

    if (argc < 4)
        return (2);
    if (!strcmp(argv[2], "unencrypted")) {
        table = UNENCRYPTED_WORDS;
        count = UNENCRYPTED_COUNT;
    } else if (!strcmp(argv[2], "ice")) {
        table = ICE_WORDS;
        count = ICE_COUNT;
    } else if (!strcmp(argv[2], "log")) {
        table = LOG_WORDS;
        count = LOG_COUNT;
    }
    printf("%u\n", word_number(argv[3], table, count));
    return (0);
}

static int records(const char *mode, const char *hex)
{
    uint8_t bytes[1024];
    uint32_t len = unhex(hex, bytes, sizeof(bytes));
    sockets_record_t r = {};
    uint32_t used;

    if (!strcmp(mode, "record")) {
        used = read_message(bytes, len, &r);
        if (used)
            printf("conn %llu len %u lane %u flags %u [%.*s]\n",
                (unsigned long long)r.connection, r.length, r.lane, r.flags,
                (int)r.length, (const char *)r.bytes);
    } else if (!strcmp(mode, "session")) {
        used = read_session_message(bytes, len, &r);
        if (used)
            printf("sess %llu channel %d len %u flags %u [%.*s]\n",
                (unsigned long long)r.connection, r.channel, r.length,
                r.flags, (int)r.length, (const char *)r.bytes);
    } else {
        used = read_signal(bytes, len, &r);
        if (used)
            printf("signal %u [%.*s]\n", r.length, (int)r.length,
                (const char *)r.bytes);
    }
    if (!used)
        printf("0\n");
    return (0);
}

static int writes(int argc, char **argv, bool session)
{
    uint8_t out[256];
    sockets_record_t r = {};
    uint32_t room;
    uint32_t used;

    if (argc < 7 + !session)
        return (2);
    r.connection = strtoull(argv[2], nullptr, 0);
    if (session) {
        r.channel = atoi(argv[3]);
        r.flags = (uint8_t)atoi(argv[4]);
        r.bytes = (const uint8_t *)argv[5];
        room = (uint32_t)atoi(argv[6]);
    } else {
        r.number = atoll(argv[3]);
        r.lane = (uint16_t)atoi(argv[4]);
        r.flags = (uint8_t)atoi(argv[5]);
        r.bytes = (const uint8_t *)argv[6];
        room = (uint32_t)atoi(argv[7]);
    }
    r.length = (uint32_t)strlen((const char *)r.bytes);
    room = room < sizeof(out) ? room : sizeof(out);
    used = session ? write_session_message(out, room, &r)
        : write_message(out, room, &r);
    printf("%u\n", used);
    return (0);
}

int main(int argc, char **argv)
{
    if (argc < 3)
        return (2);
    if (!strcmp(argv[1], "word"))
        return (words(argc, argv));
    if (!strcmp(argv[1], "ice"))
        printf("%d\n", ice_mask(argv[2]));
    else if (!strcmp(argv[1], "status"))
        printf("%d\n", status_of(atoi(argv[2])));
    else if (!strcmp(argv[1], "flags"))
        printf("%d\n", flags_of(atoi(argv[2])));
    else if (!strcmp(argv[1], "inrecord"))
        return (writes(argc, argv, false));
    else if (!strcmp(argv[1], "insession"))
        return (writes(argc, argv, true));
    else
        return (records(argv[1], argv[2]));
    return (0);
}

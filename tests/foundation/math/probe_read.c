/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "probe.h"

double read_f64(const char *text)
{
    return (strtod(text, nullptr));
}

float read_f32(const char *text)
{
    return (strtof(text, nullptr));
}

void show_f64(double value)
{
    uint64_t bits;

    memcpy(&bits, &value, sizeof(bits));
    if (value != value)
        printf(" nan");
    else
        printf(" %016llx", (unsigned long long)bits);
}

void show_bits(double value)
{
    uint64_t bits;

    memcpy(&bits, &value, sizeof(bits));
    printf(" %016llx", (unsigned long long)bits);
}

void show_f32(float value)
{
    uint32_t bits;

    memcpy(&bits, &value, sizeof(bits));
    if (value != value)
        printf(" nan");
    else
        printf(" %08lx", (unsigned long)bits);
}

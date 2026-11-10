/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "space_pure.h"

static float read_float(const char *text)
{
    if (!strcmp(text, "nan"))
        return (NAN);
    if (!strcmp(text, "inf"))
        return (INFINITY);
    if (!strcmp(text, "-0"))
        return (-0.0f);
    return (strtof(text, nullptr));
}

int main(int argc, char **argv)
{
    ideatek_transform_t t = { .rotation = { 0.0f, 0.0f, 0.0f, 1.0f },
        .scale = { 1.0f, 1.0f, 1.0f } };
    ideatek_world_transform_t a = {};
    ideatek_world_transform_t b = {};

    if (argc == 5 && !strcmp(argv[1], "ok")) {
        t.position.x = read_float(argv[2]);
        t.scale.y = read_float(argv[3]);
        t.rotation.w = read_float(argv[4]);
        printf("ok %d\n", is_transform_ok(&t));
    }
    if (argc == 4 && !strcmp(argv[1], "same")) {
        a.origin.x = read_float(argv[2]);
        b.origin.x = read_float(argv[3]);
        printf("same %d\n", is_same_world(&a, &b));
    }
    return (0);
}

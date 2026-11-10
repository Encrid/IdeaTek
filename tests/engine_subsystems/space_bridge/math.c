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

static float clean(float v)
{
    return (fabsf(v) < 0.0005f ? 0.0f : v);
}

static ideatek_transform_t read_transform(char **v)
{
    float angle = strtof(v[3], nullptr) * 3.14159265f / 360.0f;

    return ((ideatek_transform_t){ .position = { strtod(v[0], nullptr),
        strtod(v[1], nullptr), strtod(v[2], nullptr) },
        .rotation = { 0.0f, sinf(angle), 0.0f, cosf(angle) },
        .scale = { strtof(v[4], nullptr), strtof(v[5], nullptr),
        strtof(v[6], nullptr) } });
}

static void show_world(const char *what, const ideatek_world_transform_t *w)
{
    printf("%s %.3f %.3f %.3f basis", what, clean((float)w->origin.x),
        clean((float)w->origin.y), clean((float)w->origin.z));
    for (int i = 0; i < 9; i++)
        printf(" %.3f", clean(w->basis[i]));
    printf("\n");
}

static void show_transform(const ideatek_transform_t *t)
{
    printf("local %.3f %.3f %.3f rotation %.3f %.3f %.3f %.3f scale %.3f %.3f "
        "%.3f\n", t->position.x, t->position.y, t->position.z,
        clean(t->rotation.x), clean(t->rotation.y), clean(t->rotation.z),
        clean(t->rotation.w), t->scale.x, t->scale.y, t->scale.z);
}

static void run(int argc, char **argv)
{
    ideatek_transform_t a = read_transform(argv + 2);
    ideatek_transform_t b = argc >= 16 ? read_transform(argv + 9) : a;
    ideatek_world_transform_t wa;
    ideatek_world_transform_t wb;
    ideatek_world_transform_t out;
    ideatek_transform_t local;

    get_matrix(&a, &wa);
    get_matrix(&b, &wb);
    if (!strcmp(argv[1], "matrix"))
        show_world("matrix", &wa);
    if (!strcmp(argv[1], "compose")) {
        compose(&wa, &wb, (uint8_t)atoi(argv[16]), &out);
        show_world("compose", &out);
    }
    if (!strcmp(argv[1], "local") && get_local(&wa, &wb, &local))
        show_transform(&local);
    else if (!strcmp(argv[1], "local"))
        printf("local none\n");
}

int main(int argc, char **argv)
{
    float basis[9];
    ideatek_transform_t t = {};

    if (argc == 11 && !strcmp(argv[1], "split")) {
        for (int i = 0; i < 9; i++)
            basis[i] = strtof(argv[2 + i], nullptr);
        split_basis(basis, &t.rotation, &t.scale);
        show_transform(&t);
        return (0);
    }
    if (argc >= 9)
        run(argc, argv);
    return (0);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>

#include "math_mat4.h"
#include "probe.h"

static mat4f_t read_m(char **v)
{
    float numbers[16];

    for (int i = 0; i < 16; i++)
        numbers[i] = read_f32(v[i]);
    return (mat4f_load(numbers));
}

static void show_m(mat4f_t m)
{
    float numbers[17];

    numbers[16] = -1.0f;
    mat4f_store(m, numbers);
    for (int i = 0; i < 16; i++)
        show_f32(numbers[i]);
    if (numbers[16] != -1.0f)
        printf(" overrun");
}

static void run_load(char **v)
{
    show_m(read_m(v));
}

static void run_get_transpose(char **v)
{
    show_m(mat4f_get_transpose(read_m(v)));
}

static void run_turn(char **v)
{
    vec4f_t in = { read_f32(v[16]), read_f32(v[17]), read_f32(v[18]),
        read_f32(v[19]) };
    vec4f_t out = mat4f_turn(read_m(v), in);

    show_f32(out.x);
    show_f32(out.y);
    show_f32(out.z);
    show_f32(out.w);
}

static void run_get_product(char **v)
{
    show_m(mat4f_get_product(read_m(v), read_m(v + 16)));
}

static void run_get_inverse(char **v)
{
    mat4f_t inverse = read_m(v);
    bool done = mat4f_get_inverse(read_m(v), &inverse);

    printf(" %d", (int)done);
    show_m(inverse);
}

static void run_make_perspective(char **v)
{
    mat4f_lens_t lens = { read_f32(v[0]), read_f32(v[1]), read_f32(v[2]),
        read_f32(v[3]), atoi(v[4]) != 0, atoi(v[5]) != 0 };

    show_m(mat4f_make_perspective(&lens));
}

static void run_make_frustum(char **v)
{
    mat4f_frustum_t frustum = { read_f32(v[0]), read_f32(v[1]),
        read_f32(v[2]), read_f32(v[3]), read_f32(v[4]), read_f32(v[5]),
        atoi(v[6]) != 0, atoi(v[7]) != 0 };

    show_m(mat4f_make_frustum(&frustum));
}

static void run_make_orthographic(char **v)
{
    mat4f_box_t box = { read_f32(v[0]), read_f32(v[1]), read_f32(v[2]),
        read_f32(v[3]), read_f32(v[4]), read_f32(v[5]), atoi(v[6]) != 0,
        atoi(v[7]) != 0 };

    show_m(mat4f_make_orthographic(&box));
}

static void run_make_view(char **v)
{
    rigid_t camera = { .rotation = { read_f64(v[0]), read_f64(v[1]),
        read_f64(v[2]), read_f64(v[3]) }, .origin = read_vec3d(v + 4) };

    show_m(mat4f_make_view(&camera, read_vec3d(v + 7)));
}

static void run_make_model(char **v)
{
    float numbers[9];
    affine_t place;

    for (int i = 0; i < 9; i++)
        numbers[i] = read_f32(v[i]);
    place.basis = mat3f_load(numbers);
    place.origin = read_vec3d(v + 9);
    show_m(mat4f_make_model(&place, read_vec3d(v + 12)));
}

static void run_get_planes(char **v)
{
    vec4f_t planes[6];

    mat4f_get_planes(read_m(v), planes);
    for (int i = 0; i < 6; i++) {
        show_f32(planes[i].x);
        show_f32(planes[i].y);
        show_f32(planes[i].z);
        show_f32(planes[i].w);
    }
}

static const probe_entry_t ENTRIES[] = {
    { "mat4f_load", 16, run_load },
    { "mat4f_get_transpose", 16, run_get_transpose },
    { "mat4f_turn", 20, run_turn },
    { "mat4f_get_product", 32, run_get_product },
    { "mat4f_get_inverse", 16, run_get_inverse },
    { "mat4f_make_perspective", 6, run_make_perspective },
    { "mat4f_make_frustum", 8, run_make_frustum },
    { "mat4f_make_orthographic", 8, run_make_orthographic },
    { "mat4f_make_view", 10, run_make_view },
    { "mat4f_make_model", 15, run_make_model },
    { "mat4f_get_planes", 16, run_get_planes },
};

const probe_group_t MAT4_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>

#include "math_mat3.h"
#include "probe.h"

static mat3f_t read_m(char **v)
{
    float numbers[9];

    for (int i = 0; i < 9; i++)
        numbers[i] = read_f32(v[i]);
    return (mat3f_load(numbers));
}

static void show_m(mat3f_t m)
{
    float numbers[10] = { 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f };

    mat3f_store(m, numbers);
    for (int i = 0; i < 9; i++)
        show_f32(numbers[i]);
    if (numbers[9] != -1.0f)
        printf(" overrun");
}

static void show_q(quatf_t q)
{
    show_f32(q.x);
    show_f32(q.y);
    show_f32(q.z);
    show_f32(q.w);
}

static quatf_t read_q(char **v)
{
    quatf_t out = { read_f32(v[0]), read_f32(v[1]), read_f32(v[2]),
        read_f32(v[3]) };

    return (out);
}

static void run_load(char **v)
{
    show_m(read_m(v));
}

static void run_get_column(char **v)
{
    show_vec3f(mat3f_get_column(read_m(v), atoi(v[9])));
}

static void run_make_columns(char **v)
{
    show_m(mat3f_make_columns(read_vec3f(v), read_vec3f(v + 3),
        read_vec3f(v + 6)));
}

static void run_get_transpose(char **v)
{
    show_m(mat3f_get_transpose(read_m(v)));
}

static void run_turn(char **v)
{
    show_vec3f(mat3f_turn(read_m(v), read_vec3f(v + 9)));
}

static void run_get_product(char **v)
{
    show_m(mat3f_get_product(read_m(v), read_m(v + 9)));
}

static void run_get_determinant(char **v)
{
    show_f32(mat3f_get_determinant(read_m(v)));
}

static void run_get_inverse(char **v)
{
    mat3f_t inverse = read_m(v);
    bool done = mat3f_get_inverse(read_m(v), &inverse);

    printf(" %d", (int)done);
    show_m(inverse);
}

static void run_get_rotation(char **v)
{
    show_q(mat3f_get_rotation(read_m(v)));
}

static void run_make_rotation(char **v)
{
    show_m(mat3f_make_rotation(read_q(v)));
}

static void run_make_basis(char **v)
{
    show_m(mat3f_make_basis(read_q(v), read_vec3f(v + 4)));
}

static void run_get_frame(char **v)
{
    show_m(mat3f_get_frame(read_m(v)));
}

static void run_split(char **v)
{
    quatf_t rotation;
    vec3f_t scale;

    mat3f_split(read_m(v), &rotation, &scale);
    show_q(rotation);
    show_vec3f(scale);
}

static const probe_entry_t ENTRIES[] = {
    { "mat3f_load", 9, run_load },
    { "mat3f_get_column", 10, run_get_column },
    { "mat3f_make_columns", 9, run_make_columns },
    { "mat3f_get_transpose", 9, run_get_transpose },
    { "mat3f_turn", 12, run_turn },
    { "mat3f_get_product", 18, run_get_product },
    { "mat3f_get_determinant", 9, run_get_determinant },
    { "mat3f_get_inverse", 9, run_get_inverse },
    { "mat3f_get_rotation", 9, run_get_rotation },
    { "mat3f_make_rotation", 4, run_make_rotation },
    { "mat3f_make_basis", 7, run_make_basis },
    { "mat3f_get_frame", 9, run_get_frame },
    { "mat3f_split", 9, run_split },
};

const probe_group_t MAT3F_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};

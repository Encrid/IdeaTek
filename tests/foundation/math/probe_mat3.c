/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>
#include <stdlib.h>

#include "math_mat3.h"
#include "probe.h"

static mat3d_t read_m(char **v)
{
    double numbers[9];

    for (int i = 0; i < 9; i++)
        numbers[i] = read_f64(v[i]);
    return (mat3d_load(numbers));
}

static void show_m(mat3d_t m)
{
    double numbers[10] = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0 };

    mat3d_store(m, numbers);
    for (int i = 0; i < 9; i++)
        show_f64(numbers[i]);
    if (numbers[9] != -1.0)
        printf(" overrun");
}

static void show_q(quatd_t q)
{
    show_f64(q.x);
    show_f64(q.y);
    show_f64(q.z);
    show_f64(q.w);
}

static quatd_t read_q(char **v)
{
    quatd_t out = { read_f64(v[0]), read_f64(v[1]), read_f64(v[2]),
        read_f64(v[3]) };

    return (out);
}

static void run_load(char **v)
{
    show_m(read_m(v));
}

static void run_get_column(char **v)
{
    show_vec3d(mat3d_get_column(read_m(v), atoi(v[9])));
}

static void run_make_columns(char **v)
{
    show_m(mat3d_make_columns(read_vec3d(v), read_vec3d(v + 3),
        read_vec3d(v + 6)));
}

static void run_get_transpose(char **v)
{
    show_m(mat3d_get_transpose(read_m(v)));
}

static void run_turn(char **v)
{
    show_vec3d(mat3d_turn(read_m(v), read_vec3d(v + 9)));
}

static void run_get_product(char **v)
{
    show_m(mat3d_get_product(read_m(v), read_m(v + 9)));
}

static void run_get_determinant(char **v)
{
    show_f64(mat3d_get_determinant(read_m(v)));
}

static void run_get_inverse(char **v)
{
    mat3d_t inverse = read_m(v);
    bool done = mat3d_get_inverse(read_m(v), &inverse);

    printf(" %d", (int)done);
    show_m(inverse);
}

static void run_get_rotation(char **v)
{
    show_q(mat3d_get_rotation(read_m(v)));
}

static void run_make_rotation(char **v)
{
    show_m(mat3d_make_rotation(read_q(v)));
}

static void run_make_basis(char **v)
{
    show_m(mat3d_make_basis(read_q(v), read_vec3d(v + 4)));
}

static void run_get_frame(char **v)
{
    show_m(mat3d_get_frame(read_m(v)));
}

static void run_split(char **v)
{
    quatd_t rotation;
    vec3d_t scale;

    mat3d_split(read_m(v), &rotation, &scale);
    show_q(rotation);
    show_vec3d(scale);
}

static const probe_entry_t ENTRIES[] = {
    { "mat3d_load", 9, run_load },
    { "mat3d_get_column", 10, run_get_column },
    { "mat3d_make_columns", 9, run_make_columns },
    { "mat3d_get_transpose", 9, run_get_transpose },
    { "mat3d_turn", 12, run_turn },
    { "mat3d_get_product", 18, run_get_product },
    { "mat3d_get_determinant", 9, run_get_determinant },
    { "mat3d_get_inverse", 9, run_get_inverse },
    { "mat3d_get_rotation", 9, run_get_rotation },
    { "mat3d_make_rotation", 4, run_make_rotation },
    { "mat3d_make_basis", 7, run_make_basis },
    { "mat3d_get_frame", 9, run_get_frame },
    { "mat3d_split", 9, run_split },
};

const probe_group_t MAT3D_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};

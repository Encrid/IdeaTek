/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <stdio.h>

#include "math_place.h"
#include "probe.h"

static rigid_t read_rigid(char **v)
{
    rigid_t out = { .rotation = { read_f64(v[0]), read_f64(v[1]),
        read_f64(v[2]), read_f64(v[3]) }, .origin = read_vec3d(v + 4) };

    return (out);
}

static affine_t read_affine(char **v)
{
    float numbers[9];
    affine_t out;

    for (int i = 0; i < 9; i++)
        numbers[i] = read_f32(v[i]);
    out.basis = mat3f_load(numbers);
    out.origin = read_vec3d(v + 9);
    return (out);
}

static void show_rigid(const rigid_t *place)
{
    show_f64(place->rotation.x);
    show_f64(place->rotation.y);
    show_f64(place->rotation.z);
    show_f64(place->rotation.w);
    show_vec3d(place->origin);
}

static void show_affine(const affine_t *place)
{
    float numbers[9];

    mat3f_store(place->basis, numbers);
    for (int i = 0; i < 9; i++)
        show_f32(numbers[i]);
    show_vec3d(place->origin);
}

static void run_rigid_compose(char **v)
{
    rigid_t parent = read_rigid(v);
    rigid_t local = read_rigid(v + 7);

    rigid_compose(&parent, &local, &parent);
    show_rigid(&parent);
}

static void run_rigid_invert(char **v)
{
    rigid_t place = read_rigid(v);

    rigid_invert(&place, &place);
    show_rigid(&place);
}

static void run_rigid_move(char **v)
{
    rigid_t place = read_rigid(v);

    show_vec3d(rigid_move(&place, read_vec3d(v + 7)));
}

static void run_rigid_move_back(char **v)
{
    rigid_t place = read_rigid(v);

    show_vec3d(rigid_move_back(&place, read_vec3d(v + 7)));
}

static void run_affine_compose(char **v)
{
    affine_t parent = read_affine(v);
    affine_t local = read_affine(v + 12);

    affine_compose(&parent, &local, &parent);
    show_affine(&parent);
}

static void run_affine_invert(char **v)
{
    affine_t place = read_affine(v);
    bool done = affine_invert(&place, &place);

    printf(" %d", (int)done);
    show_affine(&place);
}

static void run_affine_move(char **v)
{
    affine_t place = read_affine(v);

    show_vec3d(affine_move(&place, read_vec3d(v + 12)));
}

static void run_mat3f_get_wide(char **v)
{
    affine_t place = read_affine(v);
    double numbers[9];

    mat3d_store(mat3f_get_wide(place.basis), numbers);
    for (int i = 0; i < 9; i++)
        show_f64(numbers[i]);
}

static const probe_entry_t ENTRIES[] = {
    { "rigid_compose", 14, run_rigid_compose },
    { "rigid_invert", 7, run_rigid_invert },
    { "rigid_move", 10, run_rigid_move },
    { "rigid_move_back", 10, run_rigid_move_back },
    { "affine_compose", 24, run_affine_compose },
    { "affine_invert", 12, run_affine_invert },
    { "affine_move", 15, run_affine_move },
    { "mat3f_get_wide", 12, run_mat3f_get_wide },
};

const probe_group_t PLACE_PROBES = {
    ENTRIES, sizeof(ENTRIES) / sizeof(ENTRIES[0])
};

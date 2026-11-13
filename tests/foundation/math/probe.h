/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef MATH_PROBE_H
    #define MATH_PROBE_H

    #include <stddef.h>

    #include "math_vec3.h"

typedef void (*probe_run_t)(char **values);

typedef struct probe_entry {
    const char *name;
    int count;
    probe_run_t run;
} probe_entry_t;

typedef struct probe_group {
    const probe_entry_t *entries;
    size_t count;
} probe_group_t;

extern const probe_group_t SCALAR_PROBES;
extern const probe_group_t ROOT_PROBES;
extern const probe_group_t VEC3_PROBES;
extern const probe_group_t VEC3_SHAPE_PROBES;
extern const probe_group_t VEC3_NUMBER_PROBES;
extern const probe_group_t VEC4_PROBES;
extern const probe_group_t TRIG_PROBES;
extern const probe_group_t DISTANCE_PROBES;

double read_f64(const char *text);
float read_f32(const char *text);
void show_f64(double value);
void show_f32(float value);
void show_bits(double value);
vec3d_t read_vec3d(char **values);
vec3f_t read_vec3f(char **values);
void show_vec3d(vec3d_t value);
void show_vec3f(vec3f_t value);

#endif

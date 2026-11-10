/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#ifndef SPACE_PURE_H
    #define SPACE_PURE_H

    #include <stdint.h>

    #include "ideatek_space.h"

typedef struct {
    uint64_t *slots;
    uint32_t room;
    uint32_t count;
} space_set_t;

bool is_transform_ok(const ideatek_transform_t *t);
bool is_same_world(const ideatek_world_transform_t *a,
    const ideatek_world_transform_t *b);

void get_matrix(const ideatek_transform_t *t, ideatek_world_transform_t *out);
void multiply_basis(const float *a, const float *b, float *out);
void compose(const ideatek_world_transform_t *parent,
    const ideatek_world_transform_t *local, uint8_t absolute,
    ideatek_world_transform_t *out);
bool get_local(const ideatek_world_transform_t *parent,
    const ideatek_world_transform_t *world, ideatek_transform_t *out);

void split_basis(const float basis[9], ideatek_quat_t *rotation,
    ideatek_vec3_t *scale);
ideatek_quat_t get_rotation(const float m[9]);
void get_rotation_matrix(const ideatek_quat_t *r, float m[9]);

void start_set(space_set_t *set, uint64_t *slots, uint32_t room);
bool set_put(space_set_t *set, uint64_t id);
bool set_has(const space_set_t *set, uint64_t id);

#endif

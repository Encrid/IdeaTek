/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline vec3_t mat3_fill_unit(vec3_t column, vec3_t next, vec3_t last)
{
    real_t length = vec3_get_length(column);

    if (length == (real_t)0)
        return (vec3_get_unit(vec3_get_cross(next, last)));
    return (column / length);
}

static inline mat3_t mat3_get_frame(mat3_t m)
{
    vec3_t x = mat3_get_column(m, 0);
    vec3_t y = mat3_get_column(m, 1);
    vec3_t z = mat3_get_column(m, 2);

    x = mat3_fill_unit(x, y, z);
    y = mat3_fill_unit(y - x * vec3_get_dot(y, x), z, x);
    z = z - x * vec3_get_dot(z, x);
    z = mat3_fill_unit(z - y * vec3_get_dot(z, y), x, y);
    return (mat3_make_columns(x, y, z));
}

static inline void mat3_split(mat3_t m, quat_t *rotation, vec3_t *scale)
{
    mat3_t frame = mat3_get_frame(m);
    vec3_t x = mat3_get_column(frame, 0);
    vec3_t y = mat3_get_column(frame, 1);
    vec3_t z = mat3_get_column(frame, 2);
    real_t depth = vec3_get_length(mat3_get_column(m, 2));

    if (mat3_get_determinant(m) < (real_t)0) {
        z = -z;
        depth = -depth;
    }
    scale->x = vec3_get_length(mat3_get_column(m, 0));
    scale->y = vec3_get_length(mat3_get_column(m, 1));
    scale->z = depth;
    *rotation = mat3_get_rotation(mat3_make_columns(x, y, z));
}

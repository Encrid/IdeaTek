/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline vec3_t mat3_turn(mat3_t m, vec3_t v)
{
    vec3_t out;

    out.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    out.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    out.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;
    return (out);
}

static inline mat3_t mat3_get_product(mat3_t a, mat3_t b)
{
    mat3_t out;

    for (int column = 0; column < 3; column++) {
        out[0][column] = a[0][0] * b[0][column] + a[0][1] * b[1][column]
            + a[0][2] * b[2][column];
        out[1][column] = a[1][0] * b[0][column] + a[1][1] * b[1][column]
            + a[1][2] * b[2][column];
        out[2][column] = a[2][0] * b[0][column] + a[2][1] * b[1][column]
            + a[2][2] * b[2][column];
    }
    return (out);
}

static inline real_t mat3_get_determinant(mat3_t m)
{
    vec3_t x = mat3_get_column(m, 0);
    vec3_t y = mat3_get_column(m, 1);
    vec3_t z = mat3_get_column(m, 2);

    return (vec3_get_dot(x, vec3_get_cross(y, z)));
}

static inline bool mat3_get_inverse(mat3_t m, mat3_t *inverse)
{
    vec3_t x = mat3_get_column(m, 0);
    vec3_t y = mat3_get_column(m, 1);
    vec3_t z = mat3_get_column(m, 2);
    vec3_t rows[3] = { vec3_get_cross(y, z), vec3_get_cross(z, x),
        vec3_get_cross(x, y) };
    real_t determinant = vec3_get_dot(x, rows[0]);

    if (determinant == (real_t)0)
        return (false);
    for (int row = 0; row < 3; row++) {
        (*inverse)[row][0] = rows[row].x / determinant;
        (*inverse)[row][1] = rows[row].y / determinant;
        (*inverse)[row][2] = rows[row].z / determinant;
    }
    return (true);
}

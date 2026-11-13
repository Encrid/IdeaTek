/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline vec3_t mat3_get_column(mat3_t m, int column)
{
    vec3_t out = { m[0][column], m[1][column], m[2][column] };

    return (out);
}

static inline mat3_t mat3_make_columns(vec3_t x, vec3_t y, vec3_t z)
{
    mat3_t out;

    for (int row = 0; row < 3; row++) {
        out[row][0] = x[row];
        out[row][1] = y[row];
        out[row][2] = z[row];
    }
    return (out);
}

static inline mat3_t mat3_get_transpose(mat3_t m)
{
    mat3_t out;

    for (int row = 0; row < 3; row++)
        for (int column = 0; column < 3; column++)
            out[row][column] = m[column][row];
    return (out);
}

static inline mat3_t mat3_load(const real_t *numbers)
{
    mat3_t out;

    for (int column = 0; column < 3; column++)
        for (int row = 0; row < 3; row++)
            out[row][column] = numbers[column * 3 + row];
    return (out);
}

static inline void mat3_store(mat3_t m, real_t *numbers)
{
    for (int column = 0; column < 3; column++)
        for (int row = 0; row < 3; row++)
            numbers[column * 3 + row] = m[row][column];
}

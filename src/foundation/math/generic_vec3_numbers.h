/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
static inline vec3_t vec3_load(const real_t *numbers)
{
    vec3_t out;

    out.x = numbers[0];
    out.y = numbers[1];
    out.z = numbers[2];
    return (out);
}

static inline void vec3_store(vec3_t a, real_t *numbers)
{
    numbers[0] = a.x;
    numbers[1] = a.y;
    numbers[2] = a.z;
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_physics.h>

int main(void)
{
    ideatek_physics_material_t value = { .friction = 0.6f, .density = 1000.0f };

    if (value.density != 1000.0f)
        return (1);
    return (sizeof(ideatek_physics_material_t_desc) > 1 ? 0 : 1);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_effects.h>

int main(void)
{
    ideatek_particle_emitter_t value = { .lifetime_min = 1.0f, .lifetime_max = 1.0f };

    if (value.lifetime_max != 1.0f)
        return (1);
    return (sizeof(ideatek_particle_emitter_t_desc) > 1 ? 0 : 1);
}

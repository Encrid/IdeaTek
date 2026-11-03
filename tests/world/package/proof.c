/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_world.h>

int main(void)
{
    ideatek_entity_t entity = { .id = 1 };

    if (entity.id == 0)
        return (1);
    return (sizeof(ideatek_entity_t_desc) > 1 ? 0 : 1);
}

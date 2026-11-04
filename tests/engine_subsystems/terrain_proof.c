/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_terrain.h>

int main(void)
{
    ideatek_terrain_t value = { .chunk_cells = 16 };

    if (value.chunk_cells != 16)
        return (1);
    return (sizeof(ideatek_terrain_t_desc) > 1 ? 0 : 1);
}

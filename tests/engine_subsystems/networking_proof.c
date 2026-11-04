/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_networking.h>

int main(void)
{
    ideatek_replicated_t value = { .prefab = 7 };

    if (value.prefab != 7)
        return (1);
    return (sizeof(ideatek_replicated_t_desc) > 1 ? 0 : 1);
}

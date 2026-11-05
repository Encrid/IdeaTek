/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_changes.h>

int main(void)
{
    ideatek_change_tag_t value = { .period = 0.0f, .durability = IDEATEK_DURABILITY_PERIODIC };

    if (value.durability != IDEATEK_DURABILITY_PERIODIC)
        return (1);
    return (sizeof(ideatek_change_tag_t_desc) > 1 ? 0 : 1);
}

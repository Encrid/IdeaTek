/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_input_mapping.h>

int main(void)
{
    ideatek_input_dead_zone_t value = { .lower = 0.2f, .upper = 1.0f };

    if (value.upper != 1.0f)
        return (1);
    return (sizeof(ideatek_input_dead_zone_t_desc) > 1 ? 0 : 1);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_navigation.h>

int main(void)
{
    ideatek_nav_area_t value = { .travel_cost = 1.0f };

    if (value.travel_cost != 1.0f)
        return (1);
    return (sizeof(ideatek_nav_area_t_desc) > 1 ? 0 : 1);
}

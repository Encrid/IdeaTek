/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_fields.h>

int main(void)
{
    ideatek_wind_t value = { .speed = 4.0f };

    if (value.speed != 4.0f)
        return (1);
    return (sizeof(ideatek_wind_t_desc) > 1 ? 0 : 1);
}

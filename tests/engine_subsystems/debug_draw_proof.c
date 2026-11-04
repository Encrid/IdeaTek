/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_debug_draw.h>

int main(void)
{
    ideatek_debug_view_t value = { .groups = 1 };

    if (value.groups != 1)
        return (1);
    return (sizeof(ideatek_debug_view_t_desc) > 1 ? 0 : 1);
}

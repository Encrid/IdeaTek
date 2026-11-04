/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_persistence.h>

int main(void)
{
    ideatek_saved_t value = { .groups = 1 };

    if (value.groups != 1)
        return (1);
    return (sizeof(ideatek_saved_t_desc) > 1 ? 0 : 1);
}

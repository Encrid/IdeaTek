/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_matter.h>

int main(void)
{
    ideatek_matter_family_t family = { .specific_heat = 1000.0f };

    if (family.specific_heat != 1000.0f)
        return (1);
    return (sizeof(ideatek_matter_t_desc) > 1 ? 0 : 1);
}

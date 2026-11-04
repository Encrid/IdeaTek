/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_states.h>

int main(void)
{
    ideatek_state_change_t change = { .from = 1, .to = 2 };

    if (change.to != 2)
        return (1);
    return (sizeof(ideatek_state_change_t_desc) > 1 ? 0 : 1);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_frame.h>

int main(void)
{
    ideatek_tick_t tick = { .step = 1.0 / 60.0 };

    if (tick.step <= 0.0)
        return (1);
    return (sizeof(ideatek_tick_t_desc) > 1 ? 0 : 1);
}

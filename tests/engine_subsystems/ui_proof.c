/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_ui.h>

int main(void)
{
    ideatek_ui_root_t value = { .scale = 1.0f };

    if (value.scale != 1.0f)
        return (1);
    return (sizeof(ideatek_ui_root_t_desc) > 1 ? 0 : 1);
}

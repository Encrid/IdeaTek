/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_accessibility.h>

int main(void)
{
    ideatek_accessibility_state_t state = { .increase_contrast = -1 };

    if (state.theme != IDEATEK_THEME_UNKNOWN)
        return (1);
    return (sizeof(ideatek_accessibility_state_t_desc) > 1 ? 0 : 1);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_window.h>

int main(void)
{
    ideatek_window_request_t request = { .width = 1280, .height = 720 };

    if (request.mode != IDEATEK_WINDOW_MODE_WINDOWED)
        return (1);
    return (sizeof(ideatek_window_request_t_desc) > 1 ? 0 : 1);
}

/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_input.h>
#include <ideatek_keyboard.h>
#include <ideatek_text.h>
#include <ideatek_mouse.h>
#include <ideatek_gamepad.h>
#include <ideatek_touch.h>

int main(void)
{
    ideatek_mouse_request_t request = { .cursor = IDEATEK_CURSOR_DEFAULT };

    if (request.mode != IDEATEK_MOUSE_MODE_NORMAL)
        return (1);
    return (sizeof(ideatek_mouse_request_t_desc) > 1 ? 0 : 1);
}

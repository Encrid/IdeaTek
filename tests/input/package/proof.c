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
#include <ideatek_pen.h>
#include <ideatek_sensor.h>

int main(void)
{
    ideatek_text_request_t request = { .width = 640, .height = 32 };

    if (request.type != IDEATEK_TEXT_TYPE_TEXT)
        return (1);
    return (sizeof(ideatek_text_request_t_desc) > 1 ? 0 : 1);
}

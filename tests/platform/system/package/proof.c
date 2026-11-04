/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_system.h>

int main(void)
{
    ideatek_message_request_t request = { .title = { .len = 0 } };

    if (request.level != IDEATEK_MESSAGE_LEVEL_ERROR)
        return (1);
    return (sizeof(ideatek_message_request_t_desc) > 1 ? 0 : 1);
}

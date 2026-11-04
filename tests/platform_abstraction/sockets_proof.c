/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_sockets.h>

int main(void)
{
    ideatek_listen_request_t value = { .port = 27015 };

    if (value.port != 27015)
        return (1);
    return (sizeof(ideatek_listen_request_t_desc) > 1 ? 0 : 1);
}

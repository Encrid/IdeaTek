/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_sockets.h>

int main(void)
{
    ideatek_connection_request_t request = {
        .action = IDEATEK_CONNECTION_ACTION_CLOSE };

    if (request.address.port != 0)
        return (1);
    return (sizeof(ideatek_connection_state_t_desc) > 1 ? 0 : 1);
}

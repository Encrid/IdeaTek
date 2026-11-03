/*
** Copyright (C) 2026 Enkrid
**
** Under the GNU Affero General Public License, version 3 only: see license.
*/
#include <ideatek_resources.h>

int main(void)
{
    ideatek_resource_t resource = { .state = IDEATEK_LOAD_STATE_LOADED };

    if (resource.state != IDEATEK_LOAD_STATE_LOADED)
        return (1);
    return (sizeof(ideatek_resource_t_desc) > 1 ? 0 : 1);
}
